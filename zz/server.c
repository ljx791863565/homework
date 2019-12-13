#include "unp.h"

int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("Usage : %s ip port \n", argv[0]);
		return -1;
	}
	
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1){
		perror("socket");
		return -1;
	}

	
	int option = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&option, sizeof(option));
	if (ret == -1){
		perror("setsockopt");
		return -1;
	}

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (ret == -1){
		perror("bind");
		return -1;
	}
	
	ret = listen(sockfd, MAX_LISTEN);
	if (ret == -1){
		perror("listen");
		return -1;
	}	

	int epfd = epoll_create(MAX_EVENT);
	if (epfd == -1){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev, event[MAX_EVENT];
	ev.data.fd = sockfd;
	ev.events = EPOLLIN;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	if (ret == -1){
		perror("epoll_ctl");
		return -1;
	}	

	struct sockaddr_in clientAddr;
	bzero(&clientAddr, 0);
	socklen_t cli_len = sizeof(clientAddr);
	int listenFd, readFd, writeFd;
	int wait_count;
	int i;
	char buf[BUFSIZE];
	while (1)
	{
		wait_count = epoll_wait(epfd, event, MAX_EVENT, 500);
		if (wait_count == -1){
			perror("epoll_wait");
			if (errno == EINTR)
				continue;
		}
		for (i = 0; i < wait_count; i++){
			if ((event[i].events & EPOLLERR) || (event[i].events & EPOLLHUP) || (!(event[i].events & EPOLLIN))){
				close(event[i].data.fd);
				continue;	
			}else if (sockfd == event[i].data.fd){
			//	while (1){
					listenFd = accept(sockfd, (struct sockaddr*)&clientAddr, &cli_len);
					if (listenFd == -1){
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
							continue;
						}else {
							perror("accept");
							continue;
						}	
					}
					printf("a new client connected successfully\n");
					ev.data.fd = listenFd;
					ev.events = EPOLLIN | EPOLLET;
					ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenFd, &ev);
			//	}
			}else if (event[i].events & EPOLLIN){
				readFd = event[i].data.fd;
				bzero(buf, 0);
				ret = read(readFd, buf, sizeof(buf));
				if (ret == -1){
					if (errno == ECONNRESET){
						close(readFd);
						epoll_ctl(epfd, EPOLL_CTL_DEL, readFd, &ev);
						event[i].data.fd = -1;
					}
						continue;
				}else if (ret == 0){
					close(readFd);
					epoll_ctl(epfd, EPOLL_CTL_DEL, readFd, &ev);
					event[i].data.fd = -1;
					continue;
				}
				ev.data.fd = readFd;
				ev.events = EPOLLOUT | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, readFd, &ev);
				printf("server read msg:%s\n", buf);
			}else if (event[i].events & EPOLLOUT){
				writeFd = event[i].data.fd;
				bzero(buf, 0);
				ret = read(0, buf, sizeof(buf));
				ret = write(writeFd, buf, sizeof(buf));
				ev.data.fd = writeFd;
				ev.events = EPOLLOUT | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_MOD, writeFd, &ev);
			}	
		}
	}
		close(sockfd);
		close(epfd);
		close(listenFd);
	return 0;

}
