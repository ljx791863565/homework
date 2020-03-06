#include "../unp.h"

int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("输入的参数有误\n");
		return -1;
	}

	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd <0){
		perror("socket");
		return -1;
	}
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(atoi(argv[2]));
	clientAddr.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(sockfd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0){
		perror("connect");
		return -1;
	}	

	char buf[BUFSIZE];
	int ret;
	while (1){
		printf("connect to %s\n", argv[1]);
		memset(&buf, 0, sizeof(buf));
		ret = read(1, buf, BUFSIZE);
		if (ret < 0){
			perror("read");
			return -1;
		}
		ret = write(sockfd, buf, ret);
		if (ret < 0){
			perror("write");
			return -1;
		}
	}	
}
