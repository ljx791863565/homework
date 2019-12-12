#include "common.h"

int main(int argc, char **argv)
{
	if (argc != 5){
		fprintf(stderr, "Usage: %s -p server_port -i server_ip\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int sockfd;
	struct sockaddr_in serverAddr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf_r[MAX_BUF], buf_w[MAX_BUF];
	int server_port;
	char server_ip[16];
	memset(server_ip, 0, sizeof(server_ip));

	int c = -1;
	while ((c = getopt(argc, argv, "p:i:")) != -1){
		switch (c)
		{
			case 'p':
				server_port = atoi(optarg);
				break;
			case 'i':
				strncpy(server_ip, optarg, 15);
				break;
			default:
				break;
		}
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("socket");
		return -1;
	}
	printf("sockfd:%d\n", sockfd);
	memset(serverAddr, 0 ,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(server_port);
	serverAddr.sin_addr.s_addr = inet_addr(server_ip);

	int ret = connect(sockfd, (struct sockaddr *)*serverAddr, addrlen);
	if (ret < 0){
		perror("connect");
		close(sockfd);
		return -1;
	}

	fd_set readfds;
	struct timeval tv;
	int maxfd = sockfd;
	int rc, wc;

	while (1){
select_label:
		maxfd = ((maxfd > sockfd) ? maxfd : sockfd);
		tv.sec = 1;
		tv.usec = 0;
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		FD_SET(sockfd, &readfds);
		ret = select(maxfd+1, &readfds, NULL, NULL, &tv);

		switch(ret)
		{
			case -1:
				if (errno == EINTR){
					goto select_label;
				}else{
					close(sockfd);
					perror("select");
					return -1;
				}
				break;
			case 0:
				break;
			default:
				if (FD_ISSET(STDIN_FILENO, &readfds)){
					memset(buf_r, '\0', MAX_BUF);
					wc = send(sockfd, buf_r, MAX_BUF, 0);
					if (wc < 0){
						close(sockfd);
						perror("send");
						return -1;
					}else if (wc == 0){
						close(sockfd);
						FD_CRL(sockfd, &readfds);
						printf("server is shut down");
					}else {
						buf_r[MAX_BUF -1] = '\0';
					}		
				}
				break;
		}
	}


}
