#include "unp.h"
#include "threadpool.h"

void *handler(void *arg)
{
	int fd = *((int*)arg);
	char buf[BUFSIZE];
	int ret;
	while (1) {
		bzero(buf, 0);
		ret = read(fd, buf, BUFSIZE);
		if (ret <= 0) {
			continue;
		}
		err_msg("server to client :%s", buf);
		printf("server to client :%s", buf);
	}
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		err_sys("usage : ./cli address");
		printf("usage : ./cli address");
	}
	
	int sockfd;
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	bzero(&serverAddr, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	Connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	pthread_t pid;
	int ret = pthread_create(&pid, NULL, handler, &sockfd);
	if (ret < 0) {
		err_sys("pthread_create error");
		perror("pthread_create error");
	}

	char buf[BUFSIZE];
	printf("this 2");
	while (1) {
		printf("this 1");
		bzero(buf, 0);
		ret = read(0, buf, BUFSIZE);
		if (ret <= 0) {
			continue;
		}
		err_msg("client to server :%s", buf);
		printf("client to server :%s", buf);
	}

	close(sockfd);
	return 0;
}
