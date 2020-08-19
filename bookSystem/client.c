#include "unp.h"
#include "threadpool.h"
static int regi_status = 0;

void *handler(void *arg)
{
	int fd = *((int*)arg);
	char buf[BUFSIZE];
	int ret;
	while (1) {
		bzero(buf, 0);
		ret = read(fd, buf, BUFSIZE);
		if (ret <= 0) {
			continue;
		}
		MSGHEAD_T msghead;
		bzero(&msghead, 0);
		memcpy(&msghead, buf, sizeof(msghead));

		msghead.len = ntohl(msghead.len);
		msghead.msgtype = ntohl(msghead.msgtype);
		msghead.result_type = ntohl(msghead.result_type);

		switch (msghead.msgtype)
		{
			case 2:
				{
					if (msghead.result_type == 0) {
						err_msg("user register ok");
						regi_status = 1;	//注册成功标志
					}else if (msghead.result_type == 1) {
						err_msg("user register fail");
						regi_status = 0
					}
				}
		}
		err_msg("server to client :%s", buf);
	}
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		err_sys("usage : ./cli address");
	}
	
	int sockfd;
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	bzero(&serverAddr, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_aton(argv[1], &serverAddr.sin_addr);

	Connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	pthread_t pid;
	int ret = pthread_create(&pid, NULL, handler, &sockfd);
	if (ret < 0) {
		err_sys("pthread_create error");
	}

	char buf[BUFSIZE];
	while (1) {
		bzero(buf, 0);
		ret = read(0, buf, BUFSIZE);
		if (ret <= 0) {
			continue;
		}
		ret = write(sockfd, buf, ret);
		err_msg("client to server :%s", buf);
	}

	close(sockfd);
	return 0;
}
