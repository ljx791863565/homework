#include "anet.h"

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

//Unix域套接字定义
//struct sockaddr_un server_sockaddr ;
//struct sockaddr_un cli_sockaddr ;
#include <sys/un.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

static void anetSetError(char *err, const char *fmt, ...)
{
	va_list ap;
	if (!err) {
		return;
	}
	va_start(ap, fmt);
	vsnprintf(err, ANET_ERR_LEN, fmt, ap);
	va_end(ap);
}

//SO_REUSERADDR 允许重用本地地址和端口 int
//充许绑定已被使用的地址（或端口号）
//即断开连接后不进入TIME_WAIT状态 直接可用
static int anetCreateSocket(char *err, int domain)
{
	int fd, option = 1;
	if (fd = socket(domain, SOCK_STREAM, 0) == -1) {
		anetSetError(err, "socket creating: %s", strerror(errno));
		return ANET_ERR;
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1) {
		anetSetError(err, "setsockopt SO_REUSEADDR: %s", strerror(errno));
		return ANET_ERR;
	}
	return fd;
}

#define ANET_CONNECT_NONE 0			//普通connect模式
#define ANET_CONNECT_NONBLOCK 1		//nonblocking模式

static int anetTcpGenericConnect(char *err, char *addr, int port, int flags)
{
	int fd;
	struct sockaddr_in sa;
	if ((fd = anetCreateSocket(err, AF_INET)) == ANET_ERR) {
		return ANET_ERR;
	}
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (inet_aton(addr, &sa.sin_addr) == 0) {
		struct hostent *he = gethostbyname(addr);
		if (he == NULL) {
			anetSetError(err, "can't resolve: %s", addr);
			close(fd);
			return ANET_ERR;
		}
		memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
	}
	if (flags & ANET_CONNECT_NONBLOCK) {
		if (anetNonBlock(err, fd) != ANET_OK) {
			return ANET_ERR;
		}
	}
	if (connect(fd, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
		if (errno == EINPROGRESS && (flags & ANET_CONNECT_NONBLOCK)) {
			return fd;
		}
		anetSetError(err, "connect: %s", strerror(errno));
		close(fd);
		return ANET_ERR;
	}
	return fd;
}

int anetTcpConnect(char *err, char *addr, int port)
{
	return anetTcpGenericConnect(err, addr, port, ANET_CONNECT_NONE);
}

int anetTcpNonBlockConnect(char *err, char *addr, int port)
{
	return anetTcpGenericConnect(err, addr, port, ANET_CONNECT_NONBLOCK);
}

static int anetUnixGenericConnect(char *err, char *path, int flags)
{
	int fd;
	struct sockaddr_un sa;
	if ((fd = anetCreateSocket(err, AF_LOCAL)) == ANET_ERR) {
		return ANET_ERR;
	}
	sa.sun_family = AF_LOCAL;
	strncpy(sa.sun_path, path, sizeof(sa.sun_path)-1);
	if (flags & ANET_CONNECT_NONBLOCK) {
		if (anetNonBlock(err, fd) != ANET_OK) {
			return ANET_ERR;
		}
	}
	if (connect(fd, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
		if (errno == EINPROGRESS && (flags & ANET_CONNECT_NONBLOCK)) {
			return fd;
		}
		anetSetError(err, "connect: %s", strerror(errno));
		close(fd);
		return ANET_ERR;
	}
	return fd;
}

int anetUnixConnect(char *err, char *path)
{
	return anetUnixGenericConnect(err, path, ANET_CONNECT_NONE);
}

int anetUnixNonBlockConnect(char *err, char *path)
{
	return anetUnixGenericConnect(err, path, ANET_CONNECT_NONBLOCK);
}


//每次想buf读取count个字节
//返回读取到的字节数 
//出错时返回-1
//中断时返回0
int anetRead(int fd, char *buf, int count)
{
	int nread, totlen = 0;
	while (totlen != count) {
		nread = read(fd, buf, count-totlen);
		if (nread == 0) {
			return totlen;
		}
		if (nread == -1) {
			return -1;
		}
		totlen += nread;
		buf += nread;
	}
	return totlen;
}

int anetWrite(int fd, char *buf, int count)
{
	int nwritten, totlen;
	while (totlen != count) {
		nwritten = write(fd, buf, count-totlen);
		if (nwritten == 0) {
			return totlen;
		}
		if (nwritten == -1) {
			return -1;
		}
		totlen += nwritten;
		buf += nwritten;
	}
	return totlen;
}

static int anetListen(char *err, int fd, struct sockaddr *sa, socklen_t len)
{
	if (bind(fd, sa, len) == -1) {
		anetSetError(err, "bind: %s", strerror(errno));
		close(fd);
		return ANET_ERR;
	}
	if (listen(fd, 511) == -1) {
		anetSetError(err, "listen: %s", strerror(errno));
		close(fd);
		return ANET_ERR;
	}
	return ANET_OK;
}

int anetTcpServer(char *err, int port, char *bindaddr)
{
	int fd;
	struct sockaddr_in sa;
	if ((fd = anetCreateSocket(err, AF_INET)) == ANET_ERR) {
		return ANET_ERR;
	}
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bindaddr && inet_aton(bindaddr, &sa.sin_addr) == 0) {
		anetSetError(err, "invalid bind address");
		close(fd);
		return ANET_ERR;
	}
	if (anetListen(err, fd, (struct sockaddr*)&sa, sizeof(sa)) == ANET_ERR) {
		return ANET_ERR;
	}
	return fd;
}

int anetUnixServer(char *err, char *path, mode_t perm)
{
	int fd;
	struct sockaddr_un sa;
	if ((fd = anetCreateSocket(err, AF_LOCAL)) == ANET_ERR) {
		return ANET_ERR;
	}
	memset(&sa, 0, sizeof(sa));
	sa.sun_family = AF_LOCAL;
	strncpy(sa.sun_path, path, sizeof(sa.sun_path)-1);
	if (anetListen(err, fd, (struct sockaddr*)&sa, sizeof(sa)) == ANET_ERR) {
		return ANET_ERR;
	}
	if (perm) {
		chmod(sa.sun_path, perm);
	}
	return fd;
}

static int anetGenericAccept(char *err, int fd, struct sockaddr *sa, socklen_t *len)
{
	int newFd;
	while (1) {
		newFd = accept(fd, sa, len);
		if (newFd == -1) {
			if (errno == EINTR) {
				continue;
			}else {
				anetSetError(err, "accept: %s", strerror(errno));
				return ANET_ERR;
			}
		}
		// newFd != -1 accept 成功 跳出while
		break;
	}
	return newFd;
}

int anetTcpAccept(char *err, int serversock, char *ip, int *port)
{
	int fd; 
	struct sockaddr_in sa;
	socklen_t salen = sizeof(sa);
	memset(&sa, 0, salen);
	if ((fd = anetGenericAccept(err, fd, (struct sockaddr*)&sa, &salen)) == ANET_ERR) {
		return ANET_ERR;
	}
	if (ip) {
		strcpy(ip, inet_ntoa(sa.sin_addr));
	}
	if (port) {
		*port = ntohs(sa.sin_port);
	}
	return fd;
}
int anetUnixAccept(char *err, int serversock)
{
	int fd;
	struct sockaddr_in sa;
	socklen_t salen =sizeof(sa);
	memset(&sa, 0, salen);
	if ((fd = anetGenericAccept(err, serversock, (struct sockaddr*)&sa, &salen)) == ANET_ERR) {
		return ANET_ERR;
	}
	return fd;
}

//设置fd为nonblocking模式
int anetNonBlock(char *err, int fd)
{
	int flags;
	if ((flags = fcntl(fd, F_GETFL)) == -1) {
		anetSetError(err, "fcntl(F_GETFL): %s", strerror(errno));
		return ANET_ERR;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		anetSetError(err, "fcntl(F_SETFL,O_NONBLOCK): %s", strerror(errno));
		return ANET_ERR;
	}
	return ANET_OK;
}

//IPPROTO_TCP IPv4接口
//TCP_NODELAY 不使用Nagle算法 
int anetTcpNoDelay(char *err, int fd)
{
	int option = 1;
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &option, sizeof(option)) == -1) {
		anetSetError(err, "setsockopt TCP_NODELAY: %s", seterror(errno));
		return ANET_ERR;
	}
	return ANET_OK;
}

//SO_KEEPALIVE 保持连接
//检测对方主机是否崩溃，避免（服务器）永远阻塞于TCP连接的输入。 
//设置该选项后，如果2小时内在此套接口的任一方向都没有数据交换 TCP就自动给对方 发一个保持存活探测分节(keepalive probe)。
//这是一个对方必须响应的TCP分节.它会导致以下三种情况： 
//对方接收一切正常：以期望的 ACK响应。2小时后，TCP将发出另一个探测分节。 
//对方已崩溃且已重新启动：以RST响应。套接口的待处理错误被置为ECONNRESET，套接 口本身则被关闭。 
//对方无任何响应：源自berkeley的TCP发送另外8个探测分节，相隔75秒一个，试图得到 一个响应。
//在发出第一个探测分节11分钟15秒后若仍无响应就放弃。套接口的待处理错 误被置为ETIMEOUT，套接口本身则被关闭。
//如ICMP错误是“host unreachable (主机不可达)”，说明对方主机并没有崩溃，但是不可达，这种情况下待处理错误被置为 EHOSTUNREACH。
int anetTcpKeepAlive(char *err, int fd)
{
	int option = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &option, sizeof(option)) == -1) {
		anetSetError(err, "setsockopt SO_KEEPALIVE: %s", seterror(errno));
		return ANET_ERR;
	}
	return ANET_OK;
}
int anetPeerToString(int fd, char *ip, int port);

//根据host获得ip 保存在ipbuf中
int anetResolve(char *err, char *host, char *ipbuf)
{
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	if (inet_aton(host, &sa.sin_addr) == 0) {
		struct hostent *he = gethostbyname(host);
		if (he == NULL){
			anetSetError(err, "can't resolve: %s", host);
			return ANET_ERR;
		}
		memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
	}
	strcpy(ipbuf, inet_ntoa(sa.sin_addr));
	return ANET_OK;
}

//SOL_SOCKET: 基本套接口
//SO_SNDBUF 发送缓冲区大小
//设置发送缓冲区的保留大小buffsize
//在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节
//(异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据
//和接收数据量比较大，可以设置socket缓冲区，而避免了send(),recv()不断的循环收发
int anetSetSendBuffer(char *err, int fd, int buffsize)
{
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &buffsize, sizeof(buffsize)) == -1) {
		anetSetError(err, "setsockopt SO_SNDBUF: %s", strerror(errno));
		return ANET_ERR;
	}
	return ANET_OK;
}

