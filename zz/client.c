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

void* worker(void *arg)
{
	int fd = *((int *)arg);
	char buf[BUF_SIZE];
	int ret;
	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(fd, buf, sizeof(buf));
		if (ret < 0){
			perror("recv");
			return NULL;
		}
		printf("from server: %s\n", buf);
	}
}
int main(int argc, char **argv)
{
	if (argc != 3){
		printf("usage: ./a.out addr port\n");
		return -1;
	}

	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(atoi(argv[2]));
	clientAddr.sin_addr.s_addr = inet_addr(argv[1]);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("socket");
		return -1;
	}

	int ret = connect(sockfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
	if (ret < 0){
		perror("connect");
		return -1;
	}
	printf("%s connection ok, port : %s\n", argv[1], argv[2]);
	
	pthread_t pid;
	ret = pthread_create(&pid, 0, worker, &sockfd);
	if (ret < 0){
		perror("pthread_create");
		return -1;
	}

	char buf[BUF_SIZE];

	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return -1;
		}

		ret = write(sockfd, buf, ret-1);
		if (ret < 0){
			perror("write");
			return -1;
		}
		printf("client to server: %s\n", buf);
	}

	pthread_join(pid, NULL);
	close(sockfd);
	return 0;

}
