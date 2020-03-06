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
	if (argc != 2){
		cout << "algrment number error " << endl;
		return -1;
	}
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(INT_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	int sockFd = socket(AF_INET, SOCK_STREAM, 0);	
	if (sockFd < 0){
		perror("socket");
		return -1;
	}
	if (connect(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		perror("connect");
		return -1;
	}
	cout << "connect succeed" << endl;
	string msg;
	char buf[BUFSIZE];
	while (1){
		memset(buf, 0, sizeof(buf));
		getline(cin, msg);
		if (strcmp(msg.c_str(), "QUIT") == 0){
			write(sockFd, msg.c_str(), msg.size()+1);
			close(sockFd);
			return 0;
		}
		write(sockFd, msg.c_str(), msg.size()+1);
		read(sockFd, buf, sizeof(buf));
		cout << "server answer : ";
		cout << buf << endl;
	}
	close(sockFd);
	return 0;
}
