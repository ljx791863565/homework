#include "unp.h"
#include "threadpool.h"

#define MAX_FD 1024
#define MAX_EVENT 1024
#define MAX_THREAD 6
#define EPOLL_WAIT_TIME_OUT 500

void *handler(void *arg)
{
	int fd = *((int *)arg);
	char buf[BUFSIZE];
	bzero(buf, 0);
	int ret;
	while (1) {
		ret = read(fd, buf, BUFSIZE);
		if (ret <= 0) {
			continue;
		}
		printf("form client :%s\n", buf);
		ret = write(fd, buf, ret);
		if (ret < 0) {
			err_sys("write error");
		}
		printf("server to client: %s\n",buf);
	}
}

int main(int argc, const char *argv[])
{
	int listenfd, epfd, connfd;
	struct sockaddr_in clientAddr, serverAddr;
	socklen_t clilen = sizeof(clientAddr);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serverAddr, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int on = 1;
	Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	Bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	Listen(listenfd, LISTENQ);

	bzero(&clientAddr, 0);
	epfd = Epoll_create(MAX_FD);

	struct epoll_event ev, events[MAX_EVENT];
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;
	Epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	char buf[BUFSIZE];
	int wait_count;
	int readfd;
	int i, ret;

	pool_init(MAX_THREAD);

	while (1) {
		wait_count = epoll_wait(epfd, events, MAX_EVENT, EPOLL_WAIT_TIME_OUT);
		for (i = 0; i < wait_count; i++) {
			if (events[i].data.fd == listenfd) {
				connfd = Accept(listenfd, (struct sockaddr*)&clientAddr, &clilen);
				printf("a new client ip: %s port: %dconnectted to server\n", 
						inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connfd;
				Epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
			} else if (events[i].events & EPOLLIN) {
				readfd = events[i].data.fd;
				bzero(buf, 0);

				ret = read(readfd, buf, BUFSIZE);
				if (ret <= 0) {
					continue;
				}

				pool_add_worker(handler, &readfd);
			}
		}
	}
	return 0;
}
