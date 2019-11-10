#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

typedef struct msghead
{
	int len;
	int msgtype;
	int result;
}MSGHEAD_T;

typedef struct myre
{
	char name[20];
	char passwd[20];
}MYRE_T;

int flag = 0;//界面切换的标志
void register_result(int result)
{
	if(result == 0)
	{
		printf("register ok\n");
	}
	else
	{
		printf("register fail\n");
	}

	return;
}

void login_result(int result)
{
	if(result == 0)
	{
		printf("登录成功\n");
		flag = 1;
	}
	else if(result == 1)
	{
		printf("输入账号有误\n");
		flag = -1;
	}
	else
	{
		printf("输入的密码有误\n");
		flag = -1;
	}

	return;
}
void * handler(void * arg)
{
	int * p = (int *)arg;
	int fd = *p;
	int ret;
	char buff[1024];
	MSGHEAD_T msghead;

	while(1)
	{
		memset(buff, 0, 1024);
		ret = read(fd, buff, 1024);
		if(ret < 0)
		{
			exit(-1);
		}
		if(ret == 0)
		{
			exit(0);
		}

		memset(&msghead, 0, sizeof(msghead));
		memcpy(&msghead, buff, sizeof(msghead));

		msghead.len = ntohl(msghead.len);
		msghead.msgtype = ntohl(msghead.msgtype);
        msghead.result = ntohl(msghead.result);

		if(msghead.msgtype == 2)//注册的返回消息 
		{
			register_result(msghead.result);
		}
		else if(msghead.msgtype == 4)//登录的返回消息
		{
			login_result(msghead.result);
		}
		else if(msghead.msgtype == 6)//查询的返回消息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf("no date\n");
			}
			else
			{
				printf("%s\n", buff+sizeof(msghead));
			}
		}
	}

	return NULL;
}

void myRegister(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	MYRE_T myRe;

	memset(&msghead, 0, sizeof(msghead));
	memset(&myRe, 0, sizeof(myRe));
	printf("please input a name:\n");
	scanf("%s", myRe.name);
	printf("please input a password:\n");
	scanf("%s", myRe.passwd);
	
	len = sizeof(msghead) + sizeof(myRe);
	msghead.len = len;
	msghead.msgtype = 1; //register

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &myRe, sizeof(myRe));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{

	}

}

void myLogin(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	MYRE_T myRe;

	memset(&msghead, 0, sizeof(msghead));
	memset(&myRe, 0, sizeof(myRe));
	printf("please input a name:\n");
	scanf("%s", myRe.name);
	printf("please input a password:\n");
	scanf("%s", myRe.passwd);
	
	len = sizeof(msghead) + sizeof(myRe);
	msghead.len = len;
	msghead.msgtype = 3; //登录

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &myRe, sizeof(myRe));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{

	}

}

void mySelect(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;

	memset(&msghead, 0, sizeof(msghead));
	len = sizeof(msghead);
	msghead.len = len;
	msghead.msgtype = 5; //查询

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);


	int ret;

	ret = write(fd, &msghead, len);
	if(ret < 0)
	{

	}

}

void printf_menu1()
{
	printf("please input your choice!\n");
	printf("1.register\n");
	printf("2.login\n");
}

void printf_menu2()
{
	printf("please input your choice!\n");
	printf("1.select\n");
	//2...
	//3..
}
int main(int argc, char **argv) //./client 127.0.0.1 8080
{
	//1.socket
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	//2.connect

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret;

	ret = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		perror("connect");
		close(sockfd);
		return -1;
	}

	pthread_t pid;

	ret = pthread_create(&pid, NULL, handler, &sockfd);
	char buff[1024];
	int num;
	while(1)
	{
		printf_menu1();
		scanf("%d", &num);
		flag = 0;
		if(num == 1)//register
		{
			myRegister(sockfd);
		}
		else if(num == 2)//login
		{
			myLogin(sockfd);
			while(flag == 0);//等待服务器返回消息
			if(flag == 1)//登录成功
			{
				break;
			}
		}
	}

	while(1)
	{
		printf_menu2();
		scanf("%d", &num);
		if(num == 1)//select
		{
			mySelect(sockfd);
		}
	}

	close(sockfd);

	return 0;
}
