#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

void usage()
{
	printf("Usage: a.out [ip][port]\n");
}
int main(int argc, char *argv[])
{
	if (argc < 3){
		usage();
	}
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("socket");
		return -1;
	}
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(atoi(argv[2]));
	client.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(sockfd, (struct sockaddr *)&client, sizeof(client)) < 0){
		perror("connect");
		return -1;
	}
	int pid = getpid();
	printf("client %d connect to %d\n", pid, inet_addr(argv[1]));

	return 0;
}
