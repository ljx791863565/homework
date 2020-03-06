#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;
#define BUFSIZE 1024
#define INT_PORT 8080
int main(int argc, char *argv[])
{
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(INT_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int clientFd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientFd < 0){
		perror("clientFd");
		return -1;
	}
	if (bind(clientFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		perror("bind");
		return -1;
	}
	if (listen(clientFd, 5) < 0){
		perror("listen");
		return -1;
	}
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	int newFd;
	socklen_t cLen = sizeof(clientAddr);

	while (1){
		if ((newFd = accept(clientFd, (struct sockaddr*)&clientAddr, &cLen)) < 0){
			perror("accept");
			return -1;
		}
		cout << "connect form :" << inet_ntoa(clientAddr.sin_addr) << endl;
		
		pid_t pid = fork();
		if (pid == 1){
			cout << "create fork fail" << endl;
			continue;
		}
		if (pid == 0){
			close(clientFd);
			char buf[BUFSIZE];
			int ret;
			while (1){
				memset(buf, 0, sizeof(buf));
				ret = read(newFd, buf, sizeof(buf));
				if (ret <= 0){
					perror("read");
					close(newFd);
					return -1;
				}else{
					if (strcmp(buf, "QUIT") == 0){
						cout << "client quit" << endl;
						close(newFd);
						return -1;
					}else{
						cout << inet_ntoa(clientAddr.sin_addr) << " : " << buf << endl;
						write(newFd, buf, ret);
					}
				}
			}
		}else{
			close(newFd);
		}
	}
	close(clientFd);
	return 0;
}

