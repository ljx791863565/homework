#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
#define BUFSIZE 1024
#define INT_PORT 8080
int main(int argc, char *argv[])
{
	if (argc != 2){
		cout << "number of algrment ertor" << endl;
	}
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(INT_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	int clientFd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientFd < 0){
		perror("clientFd");
		return -1;
	}
	if (connect(clientFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		perror("connect");
		return -1;
	}
	int ret;
	char buf[BUFSIZE];
	while (1){
		memset(buf, 0, sizeof(buf));
		cout << "INPUT MSG " << endl;
		ret = read(0, buf, sizeof(buf));
		if (strcmp(buf, "QUIT") == 0){
			write(clientFd, buf, ret);
			close(clientFd);
			return 0;
		}
		write(clientFd, buf, ret);
		memset(buf, 0, sizeof(buf));
		read(clientFd, buf, sizeof(buf));
		cout << "server : " << buf;
	}
	close(clientFd);
	return 0;
}

