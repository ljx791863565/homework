#include "NetConnection.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
using namespace std;

const int LISTENQ = 1024;
void unix_error(char *msga)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

int open_listenfd(int port)
{
	int listenfd, optval = 1;
	struct sockaddr_in serveraddr;

	if (listenfd = socket(AF_INET, SOCK_STREAM, 0) < 0){
		return -1;
	}

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void* )&optval, sizeof(int)) <0){
		perror("setsockopt");
		return -1;
	}
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
		perror("bind");
		return -1;
	}

	if (listen(listenfd, LISTENQ) < 0){
		perror("listen");
		return -1;
	}

	return listenfd;
}

int Open_listenfd(int port)
{
	int rc;
	if (rc = open_listenfd(port) < 0){
		unix_error("Open_lsitenfd error");
	}

	return rc;
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	int rc;
	if ((rc = accept(s, addr, addrlen)) < 0){
		unix_error("Accept error");
		return -1;
	}

	return rc;
}

void Close(int fd)
{
	int rc;
	if ((rc = close(fd)) < 0){
		unix_error("Close error");
	}
}

NetConnection::NetConnection():
	listenfd(-1), connfd(-1)
{

}

void NetConnection::listen(int port)
{
	listenfd = Open_listenfd(port);
}

int NetConnection::accept()
{
	int clientlen;
	struct sockaddr_in clientaddr;
   clientlen = sizeof(clientaddr);

	connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, reinterpret_cast<socklen_t *>(&clientlen));

	return connfd;
}

void NetConnection::close()
{
	Close(connfd);
}
