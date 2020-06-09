#include "stuent.h"
#include "../unp.h"
#define MAX_EPOLL 5
#define MAX_LISTEN 5
#define MAX_FD 65535
#define MAX_EPOLL_EVENTS 1000

static int epollfd;
static int sockfd;
int main(int argc, const char *argv[])
{
	if (argc != 3) {
		printf("usage: client ip port\n");
		return -1;
	}

	int ret;
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INhtonl
	address.sin_port = htons(atoi(argv[2]));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);

	ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
	assert(ret >= 0);

	ret = listen(sockfd, MAX_LISTEN);
	assert(ret >= 0);

	epollfd = epoll_create(MAX_EPOLL);
	assert(epollfd != -1);
	
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sockfd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);

	struct epoll_event events[MAX_EPOLL_EVENTS];
	char buf[BUFSIZE];
	


	return 0;
}
