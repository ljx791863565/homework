#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
using namespace std;
#define BUFSIZE 1024
#define INT_PORT 8090

int main(int argc, char *argv[])
{
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(INT_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int sockFd = socket(AF_INET, SOCK_STREAM, 0);	
	if (sockFd < 0){
		perror("socket");
		return -1;
	}
	if (bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		perror("bind");
		return -1;
	}
	if (listen(sockFd, 5) < 0){
		perror("listen");
		return -1;
	}
	int clientFd;
	sockaddr_in clientAddr;
	socklen_t cLen = sizeof(clientAddr);
	memset(&clientAddr, 0, sizeof(clientAddr));
	
	char buf[BUFSIZE];
	while (1){
		if ((clientFd = accept(sockFd, (sockaddr*)&clientAddr, &cLen)) < 0){
			perror("accept");
			return -1;
		}
		cout << "connect from : " << inet_ntoa(clientAddr.sin_addr) << endl;
		
		int ret;
		while (1){
			memset(buf, 0, sizeof(buf));
			ret = read(clientFd, buf, sizeof(buf));
			cout << inet_ntoa(clientAddr.sin_addr)  << " : " << buf << endl;

			if (ret == 0 || (ret > 0 && (strcmp("QUIT", buf) == 0))){
				cout << "client quit " << endl;
				close(clientFd);
				break;
			}
			write(clientFd, buf, ret);
		}
	}
	close(sockFd);
	return 0;
}
