#include "common.h"
#include "unp.h"
#include "student.h"

int sockfd;
void *headler(void *arg)
{
	int *p = (int *)arg;
	int fd = *p;
	char buf[BUFSIZE];
	int ret;
	MSGHEAD_T msghead;

	while (1) {
		memset(buf, 0, BUFSIZE);
		ret = read(fd, buf, BUFSIZE);
		if (ret <= 0) {
			perror("client read");
			return;
		}

		memset(&msghead, 0, sizeof(msghead));
		memcpy(&msghead, buf, sizeof(msghead));

		msghead.len = ntohl(msghead.len);
		msghead.msgtype = ntohl(msghead.msgtype);
		msghead.result = ntohl(msghead.result);

		switch (msghead.msgtype)
		{
			case 2:
				student_register_result(msghead.result);
				break;
			case 4:
				student_login_result(msghead.result);
			default:
				break;
		}
	}
}

int main(int argc, const char *argv[])
{

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		close(sockfd);
		return -1;
	}

	struct sockaddr_in sockAdderss;
	sockAdderss.sin_family = AF_INET;
	sockAdderss.sin_port = htons(atoi(argv[2]));
	sockAdderss.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sockfd, (struct sockaddr *)&sockAdderss, sizeof(sockAdderss));
	if (ret < 0) {
		perror("connect");
		close(sockfd);
		return -1;
	}

	pthread_t pid;
	ret = pthread_create(&pid, NULL, headler, &sockfd);
	if (ret < 0) {
		perror("pthread_create");
		return -1;
	}

	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	
	int option;
	while (1) {
		main_menu();
		scanf("%d", &option);

		if (option == 1) {
			student();
		}
//		else if (option == 2) {
//			teacher();
//		}
//		else if (option == 3) {
//			root();
//		}
		else if (option == 4) {
			printf("正在退出...\n");
			sleep(1);
			return;
		}else{
			printf("输入有误，请重新输入...\n");
		}
	}
	return 0;
}
