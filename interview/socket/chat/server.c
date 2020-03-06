#include "../unp.h"

int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("输入的参数有误\n");
		return -1;
	}

	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd <0){
		perror("socket");
		return -1;
	}
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		perror("bind");
		return -1;
	}
	if (listen(sockfd, MAXLISTEN) < 0){
		perror("linten");
		return -1;
	}
	int clientFd;
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	int clientLen = sizeof(clientAddr);
	clientFd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);
	if (clientFd <0){
		perror("accept");
		return -1;
	}
	char buf[BUFSIZE];
	int ret;
	while (1){
		printf("a new client connect\n");
		memset(&buf, 0, sizeof(buf));
		ret = read(clientFd, buf, BUFSIZE);
		if (ret < 0){
			perror("read");
			return -1;
		}
		printf("server recv msg from client : %s\n", buf);
		/*
		ret = write(sockfd, buf, ret);
		if (ret < 0){
			perror("write");
			return -1;
		}
		*/
	}	
}
