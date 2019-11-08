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

#define MAX_BUFF 1024
#define MAX_LISTEN 100
#define MAX_EVENT 20

void* handler(void *arg)
{
	int fd = *(int *)arg;
	char buf[MAX_BUFF];
	int ret;
	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return ;
		}
		ret = write(fd, buf, ret -1);
		if (ret < 0){
			perror("client:read");
			return ;
		}
		printf("server: %s\n", buf);
	}
}
int main(int argc, char **argv)
{
	if (argc > 2){
		printf("argc number error\n");
		return -1;
	}

	char buf[MAX_BUFF];
	int listenfd;
	int ret = 0;
	int epfd; 

	struct epoll_event ev, event[MAX_EVENT];
	struct sockaddr_in cliAddr;
	const char * const local_addr = "127.0.0.1";
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_port = htons(atoi(argv[1]));
	cliAddr.sin_addr.s_addr = inet_addr(local_addr);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0){
		perror("socket");
		return -1;
	}

	int on = 1;
	//error: Address aleeady in use   evert port time_wait 2 min ofter there close
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
	if (ret < 0){
		perror("set socket");
		return -1;
	}

	ret = bind(listenfd, (struct sockaddr*)&cliAddr, sizeof(cliAddr));
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
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;	//set edge-triggered
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
	if (ret < 0){
		perror("epoll_ctl");
		return -1;
	}

	while (1){
		int wait_count;
		wait_count = epoll_wait(epfd, event, MAX_EVENT, -1);
		int i = 0;
		for (i = 0; i < wait_count; i++){
			uint32_t events = event[i].events;
			char host_buf[30];
			char post_buf[30];

			if (events & EPOLLERR || events & EPOLLHUP || (!events & EPOLLIN)){
		printf("epoll has error");
		close(event[i].data.fd);
		continue;
			}else if(listenfd == event[i].data.fd){
				while (1){
					struct sockaddr_in inAddr;
					socklen_t len = sizeof(inAddr);
					int accpFd = accept(listenfd, (struct sockaddr*)&inAddr, &len);
					if (accpFd < 0){
						perror("accept");
						break;
					}
					printf("new connection: host = %s, port = %s\n", inAddr.sin_addr.s_addr, inAddr.sin_port);

					ev.data.fd = accpFd;
					ev.events = EPOLLIN | EPOLLET;
					ret = epoll_ctl(epfd, EPOLL_CTL_ADD, accpFd, &ev);
					if (ret < 0){
						perror("epoll_ctl");
						return 0;
					}
				}
				continue;
			}else {
				int done = 0;
				while (1){
					int ret_len = read(event[i].data.fd, buf, sizeof(buf));
					if (ret_len == -1){
						if (EAGAIN != errno){
							perror("read data");
							done = 1;
						}
						break;
					}else if (!ret_len){
						done = 1;
						break;
					}
					write(1, buf, ret_len);
				}
				if (done){
					printf("closed connection\n");
					close(event[i].data.fd);
				}
			}

		}
	}
	close(epfd);
	return 0;
}
