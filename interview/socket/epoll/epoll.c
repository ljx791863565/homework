#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

int start_up(char *ip, short port)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0){
		perror("socket");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local))){
		perror("bind");
		exit(3);
	}

	if (listen(listen_sock, 5) < 0){
		perror("listen");
		exit(4);
	}
	return listen_sock;
}

void Usage(const char *proc)
{
	printf("Usage: %s [ip][port]\n", proc);
}

int main(int argc, char *argv[])
{
	if (argc != 3){
		Usage(argv[0]);
		exit(1);
	}
	int listen_sock = start_up(argv[1], atoi(argv[2]));
	int epfd = epoll_create(256);
	if (epfd < 0){
		perror("epoll_create");
		exit(5);
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);
	struct epoll_event ready_events[64];
	int timeout = -1;

	while (1){
		int num = epoll_wait(epfd, ready_events, 64, timeout);
		if (num < 0){
			perror("epoll_wait");
			return -1;
		}else if (num == 0){
			printf("timeout...\n");
		}else{
			int i = 0;
			for (; i<num; i++){
				//listen_sock就绪
				int new_fd = ready_events[i].data.fd;
				if (new_fd == listen_sock && (ready_events[i].events & EPOLLIN)){
					struct sockaddr_in client;
					socklen_t len = sizeof(client);
					new_fd = accept(listen_sock, (struct sockaddr *)&client, &len);
					if (new_fd < 0){
						perror("accept");
						return -1;
					}else{
						printf("get a new connect : %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
						ev.events = EPOLLIN;
						ev.data.fd = new_fd;
						epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &ev);
					}
				}else if (ready_events[i].events & EPOLLIN){
					char buf[1024];
					ssize_t s = read(new_fd, buf, sizeof(buf));
					if (s < 0){
						perror("rean");
						return -1;
					}else if (s == 0){
						printf("client closed...\n");
						epoll_ctl(epfd, EPOLL_CTL_DEL, new_fd, NULL);
						close(new_fd);
					}else{
						buf[s] = 0;
						printf("client# %s\n", buf);
						ev.events = EPOLLOUT | EPOLLET;
						ev.data.fd = new_fd;
						epoll_ctl(epfd, EPOLL_CTL_MOD, new_fd, &ev);
					}

				}else if (ready_events[i].events & EPOLLOUT){
					char buf[1024];
					strcpy(buf, "HTTP/1.0 200 OK\r\n\r\n<html><h1>hello world !||</h1></html>");
					write(new_fd, buf, strlen(buf));
					epoll_ctl(epfd, EPOLL_CTL_DEL, new_fd, NULL);
					close(new_fd);
				}
			}
		}
	}
	return 0;
}
