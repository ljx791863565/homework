#include "../unp.h"

int main(int argc, char *argv[])
{
	int sockFd;
	char recvline[BUFSIZE];
	struct sockaddr_in servAddr;
	if (argc != 2){
		printf("usage: a.out <IPaddress>");
		return -1;
	}
	if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		return -1;
	}
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(13);
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(sockFd, (SA*)&servAddr, sizeof(servAddr)) < 0){
		perror("connect");
		return -1;
	}
	int ret;
	while ((ret = read(sockFd, recvline, BUFSIZE)) > 0){
		recvline[ret] = 0;
		if (fputs(recvline, stdout) == EOF){
			perror("fputs");
			return -1;
		}
	}

	if (ret < 0){
		perror("read");
		return -1;
	}

	return -1;
}
