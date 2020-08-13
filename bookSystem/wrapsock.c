#include "unp.h"

int Accept(int fd, struct sockaddr *sa, socklen_t *psalen)
{
	int n;
again:
	if ( (n = accept(fd, sa, psalen)) < 0) {
#ifdef EPROTO
		if (errno == EPROTO || errno == ECONNABORTED) 
#else 
		if (errno == ECONNABORTED) 
#endif
			goto again;
		else 
			err_sys("accept errno");
	}
	return (n);
}

int Socket(int family, int type, int protocol)
{
	int n;
	if ( (n = socket(family, type, protocol)) < 0) {
		err_sys("Socket error");
	}
	return (n);
}

int Epoll_create(int fd_size)
{
	int n;
	if ( (n = epoll_create(fd_size)) < 0) {
		err_sys("epoll_create error");
	}
	return (n);
}

int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
	int n;
	if ( (n = epoll_ctl(epfd, op, fd, event)) == -1 ){
		err_sys("epoll_ctl error");
	}
	return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0) {
		err_sys("bind error");
	}
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0) {
		err_sys("connect error");
	}
}

void Listen(int fd, int backlog)
{
	char *ptr;
	if ( (ptr = getenv("LISTENQ")) != NULL) {
		backlog = atoi(ptr);
	}
	if (listen(fd, backlog) < 0) {
		err_sys("listen error");
	}
}

ssize_t Recv(int fd, void *ptr, size_t nbytes, int flags)
{
	ssize_t n;
	if ( (n = recv(fd, ptr, nbytes, flags)) < 0) {
		err_sys("recv error");
	}
	return n;
}

void Send(int fd, void *ptr, size_t nbytes, int flags)
{
	if (send(fd, ptr, nbytes, flags) != (ssize_t)nbytes) {
		err_sys("send error");
	}
}

void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
	if (setsockopt(fd, level, optname, optval, optlen) < 0)
		err_sys("setsockopt error");
}

