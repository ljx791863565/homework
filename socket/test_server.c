#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_BUFF 1024
#define MAX_LISTEN 10
#define MAX_EPOLL 256

int main(int argc, char **argv)
{
	if (argc > 3){
		printf("argc number error\n");
		return -1;
	}

	char buf[MAX_BUFF];
	int sockfd;
	int ret = 0;
	struct sockaddr_in cliAddr;
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_port = htons(atoi(argv[2]));
	cliAddr.sin_addr.s_addr = inet_addr(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("socket");
		return -1;
	}

	ret = bind(sockfd, (struct sockaddr*)&cliAddr, sizeof(cliAddr));
	if (ret < 0){
		perror("bind");
		return -1;
	}

	ret = listen(sockfd, MAX_LISTEN);
	if (ret < 0){
		perror("listen");
		return -1;
	}

	struct sockaddr_in serAddr;
	socklen_t len = sizeof(serAddr);
	int newFd = accept(sockfd, (struct sockaddr*)&serAddr, &len);
	if (newFd < 0){
		perror("accept");
		return -1;
	}
	printf("a new client connectted, ip: %s port :%d\n",inet_ntoa(serAddr.sin_addr), ntohs(serAddr.sin_port));
	pthread_t pid;
	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(newFd, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return -1;
		}
		printf("server recv:%s\n", buf);
		if (!strcmp(buf, "hello")){
			sprintf(buf, "hello client\n");
			ret = write(newFd, buf, sizeof(buf));
			if (ret < 0){
				perror("write");
				return -1;
			}
		}
	}
	return 0;
}

