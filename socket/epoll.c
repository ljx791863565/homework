#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/epoll.h>

#include "threadpool.h"

#define MAX_BUFF 1024
#define MAX_LISTEN 5
#define MAX_EVENT 1024

void* handler1(void *arg)
{
	int fd = *(int *)arg;
	char buf[MAX_BUFF];
	int ret;
	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return NULL;
		}
		ret = write(fd, buf, ret);
		printf("server to client: %s\n", buf);
	}
}

int main(int argc, char **argv)
{
	if (argc > 2){
		printf("argc number error\n");
		return -1;
	}

	char buf[MAX_BUFF];
	int ret = 0;

	int listenfd;
	int epfd; 
	struct sockaddr_in cliAddr;
	const char * const local_addr = "127.0.0.1";
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_port = htons(atoi(argv[1]));
	cliAddr.sin_addr.s_addr = inet_addr(local_addr);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);		//TCP
	if (listenfd < 0){
		perror("socket");
		return -1;
	}

	int on = 1;
	//error: Address aleeady in use   evert socket in TIME_WAIT
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
	if (ret < 0){
		perror("set socket");
		return -1;
	}

	ret = bind(listenfd, (const struct sockaddr*)&cliAddr, sizeof(cliAddr));
	if (ret < 0){
		perror("bind");
		return -1;
	}

	ret = listen(listenfd, MAX_LISTEN);
	if (ret < 0){
		perror("listen");
		return -1;
	}

	//epoll
	epfd = epoll_create(256);
	if (epfd < 0){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev, event[MAX_EVENT];
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;	//set edge-triggered
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
	if (ret < 0){
		perror("epoll_ctl");
		return -1;
	}

	struct sockaddr_in serAddr;
	socklen_t len = sizeof(serAddr);
	int clientFd, readFd, writeFd;
	
	pool_init(6);

	while (1){
		int wait_count;		//保存通知事件的个数
		wait_count = epoll_wait(epfd, event, MAX_EVENT, 500);
		int i = 0;
		for (i = 0; i < wait_count; i++){
			if (event[i].data.fd == listenfd)	//客户端第一次连接
			{
				clientFd = accept(listenfd, (struct sockaddr *)&serAddr, &len);
				if (clientFd < 0){
					perror("accept");
					continue;
				}
				printf("a new client %s connect ok\n", inet_ntoa(cliAddr.sin_addr));

				ev.data.fd  = clientFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientFd, &ev);
			}else if (event[i].events & EPOLLIN)		//表示有数据读取
			{
				readFd = event[i].data.fd;
				memset(buf, 0, sizeof(buf));
				ret = read(readFd, buf, 1024);
				if (ret <= 0){
					continue;
				}
				printf("client %s: %s\n",inet_ntoa(cliAddr.sin_addr), buf);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = readFd;
				pool_add_worker(handler1, &readFd);

			}

		}
	}
	close(epfd);
	return 0;
}
