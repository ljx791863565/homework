#include "communal.h"

void *handler(void *arg)
{
	int fd = *((int *)arg);
	char buf[MAX_BUF];
	int ret;

	MSG_HEAD_T msghead;
	memset(&msghead, 0, sizeof(msghead));

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		ret = read(fd, buf, sizeof(buf));
		if (ret <= 0){
			perror("read");
			return NULL;
		}
		memcpy(&msghead, buf, sizeof(msghead));

		msghead.len = ntohl(msghead.len);
		msghead.msg_type = ntohl(msghead.msg_type);
		msghead.result = ntohl(msghead.result);

		if (msghead.msg_type == 2){
			doctor_register_result(msghead.result);
		}
	}
}
int main(int argc, char **argv)
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
			perror("socket");
			return -1;
	}

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret;
	ret = connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(ret < 0)
	{
		perror("connect");
		close(sockfd);
		return -1;
	}

	pthread_t pid;

	ret = pthread_create(&pid,NULL,handler,&sockfd);
	char buff[1024] = {0};
//	fun();
	close(sockfd);
	return 0;
}
