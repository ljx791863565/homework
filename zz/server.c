#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define MAX_LISTEN 20
#define MAX_EVENT 100
void* worker(void *arg)
{
	int fd = *((int *)arg);
	char buf[BUF_SIZE];
	int ret;
	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return NULL;
		}
		ret = write(fd, buf, ret-1);
		if (ret < 0){
			perror("write");
			return NULL;
		}
		printf("server to client: %s\n", buf);
	}
}
int main(int argc, char **argv)
{
	if (argc != 2){
		printf("usage: ./a.out port\n");
		return -1;
	}

	struct sockaddr_in clientAddr;
	const char const *p = "127.0.0.1";
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(atoi(argv[1]));
	clientAddr.sin_addr.s_addr = inet_addr(p);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("socket");
		return -1;
	}

	int ret = bind(sockfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
	if (ret < 0){
		perror("bind");
		return -1;
	}
	
	ret = listen(sockfd, MAX_LISTEN);
	if (ret < 0){
		perror("listen");
		return -1;
	}

	struct sockaddr_in serverAddr;
	socklen_t serlen;

	int epfd;
	struct epoll_event ev, event[MAX_EVENT];

	epfd = epoll_create(MAX_EVENT);
	if (epfd < 0){
		perror("event_create");
		return -1;
	}
	ev.data.fd = sockfd;
	ev.events = EPOLLIN | EPOLLET;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	if (ret < 0){
		perror("event_ctl");
		return -1;
	}

	int wait_count = 0;
	int i = 0;
	char buf[BUF_SIZE];
	int accpFd, readFd, writeFd;
	pthread_t pid;
	while (1){
		wait_count = epoll_wait(epfd, event, MAX_EVENT, 500);

		for (i = 0; i < wait_count; i++){
			if (event[i].data.fd == sockfd){
				accpFd = accept(sockfd, (struct sockaddr*)&serverAddr, &serlen);
				if (accpFd < 0){
					perror("accept");
					return -1;
				}
				printf("A new client connecttion is ok\n");
				ev.data.fd = accpFd;
				ev.events = EPOLLIN | EPOLLET;
				ret = epoll_ctl(epfd, EPOLL_CTL_ADD, accpFd, &ev);
			}else if (event[i].events & EPOLLIN){
				memset(buf, 0, sizeof(buf));
				readFd = event[i].data.fd;
				ret = read(readFd, buf, sizeof(buf));
				if (ret < 0){
					perror("read");
				}
				printf("from client : %s\n", buf);
				
				ret = pthread_create(&pid, 0, worker, &readFd);
				if (ret < 0){
					perror("pthread_create");
					return -1;
				}
				
			}
		}
	}

	/*

	char buf[BUF_SIZE];

	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(accpFd, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return -1;
		}

		printf("from client: %s\n", buf);
	}
	*/	
	pthread_join(pid, NULL);

	close(epfd);
	close(sockfd);
	return 0;

}
