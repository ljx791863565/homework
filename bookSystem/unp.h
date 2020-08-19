#ifndef __UNP__H
#define __UNP__H
#include "config.h"
#include <stdio.h>		//输入输出流
#include <stdlib.h>		//系统调用
#include <unistd.h>		//提供通用的文件 目录 程序及进程操作的函数
#include <errno.h>		//perror()
#include <fcntl.h>		//提供文件控制函数 fopen()
#include <string.h>		//提供c字符串操作函数

//涉及到文件操作及时间操作时加入如下头文件
#include <time.h>		//提供时间相关函数
#include <utime.h>
#include <sys/time.h>	//linux 系统日期时间，与平台相关时需要加入的
#include <sys/stat.h>	
#include <sys/file.h>

#include <sys/types.h>	//提供数据类型的定义 int...
#include <sys/socket.h> //提供socket函数及数据结构  socket bind connect listen accept
#include <arpa/inet.h>	//提供IP地址转换函数 inet_addr()
#include <netdb.h>		//提供设置及获取域名的函数 
						//通过IP地址获得主机有关网络信息
						//honstent* gethostbyaddr(const void *addr, size_t len, int type)
#include <netinet/in.h>	//定义数据结构 struct sockaddr_in 

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>	//提供对IO控制的函数
#endif 

#include <sys/epoll.h>
#include <sys/un.h>		//Unix domain sockets

//  涉及到用户权限及密码验证时用到的头文件
//编译时加入 -lcrypt链接加密算法库
//#include <crypt.h>		//提供使用DES加密算法的加密函数
#include <pwd.h>		//提供对/etc/passwd文件访问的函数
//#include <shadow.h>		//提供对/etc/shadow文件访问的函数

#ifdef HAVE_PTHREAD_H
#include <pthread.h>	//提供多线程操作函数	//编译时需要连接-lpthread函数库
#endif 

#ifdef HAVE_POOL_H 
#include <sys/poll.h>	//IO复用
#endif

#include <signal.h>		//提供信号操作函数
#include <sys/wait.h>	//进程等待
#include <sys/ipc.h>	//进程间通讯（IPC）
#include <sys/shm.h>	//共享内存

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif 

#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h> //sysctl() 检索系统信息和允许适当的进程设置系统信息
#endif 

#ifdef __osf__
#undef recv
#undef send
#define recv(a,b,c,d) recvfrom(a,b,c,d,0,0)
#define send(a,b,c,d) sendto(a,b,c,d,0,0)
#endif 


// Define  bzero() as a macro if it's not in standard C library
#ifndef HAVE_BZERO
#define bzero(str, n) memset(str, 0, n)
#endif

#define SA struct sockaddr
#define BUFSIZE 1024	//缓存数据的大小
#define LISTENQ 1024	
#define MAXLINE 4096
#define SERVER_PORT 8189

typedef void Sigfunc(int);		//signal handlers

typedef struct MSGHEAD
{
	int len;	//消息长度
	int msgtype; //消息类型
	int result_type;	//返回消息类型
} MSGHEAD_T;

typedef struct MSG
{
	int fd;
	char buf[BUFSIZE];
} MSG_T;

#define max(a,b) ((a) < (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

extern void err_dump(const char *, ...);
extern void err_msg(const char *, ...);
extern void err_quit(const char *, ...);
extern void err_ret(const char *, ...);
extern void err_sys(const char *, ...);

extern int Accept(int, struct sockaddr *, socklen_t *);
extern int Socket(int, int, int);
extern void Bind(int, const struct sockaddr *, socklen_t);
extern void Connect(int, const struct sockaddr *, socklen_t );
extern void Listen(int, int);
extern ssize_t Recv(int, void *, size_t, int);
extern void Send(int, void *, size_t, int);
extern void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);
extern int Epoll_create(int);
extern int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
#endif //__UNP__H
