#include "sicker.h"
#include "root.h"
int flag3;
typedef struct doctor
{
	char ID[20];
	char name[20];
	char passwd[20];
	int class;
	char sex[10];
	char ZZXM[20];
	int num;
}DOCTOR_T;
typedef struct nures
{
	char ID[20];
	char name[20];
	char passwd[20];
	int class;
	char sex[20];
	char grade[5];
	int num;
	int YZGS;
}NURES_T;
void fun_sicker()
{
	while(1)
	{
		printf_doctor1();
		scanf("%d",&num);
		if(num == 1)
		{
			myRegister_sicker(sockfd);
		}
		else if(num == 2)
		{
			while(1)
			{
				flag3 = 0;
				myLogin_sicker(sockfd);
				while(flag3 == 0);
				if(flag3 == 1)
				{
					break;
				}
			}
			while(1)
			{
				printf_sicker1();
				scanf("%d",&num);
				if(num == 1)
				{
					myGuahao_sicker(sockfd);
				}
				else if(num == 2)
				{
					mySelect_sicker(sockfd);
				}
				else if(num == 3)
				{
					myLook_sicker(sockfd);
				}
				else if(num == 4)
				{
					myTurn_sicker(sockfd);
				}
				else if(num == 5)
				{
					printf("正在退出\n");
					sleep(1);
					return;
				}
				else
				{
					printf("输入有误重新输入\n");
				}
			}
		}
		else if(num == 3)
		{
			printf("正在退出\n");
			sleep(1);
			return;
		}
		else
		{
			printf("输入有误重新输入\n");
		}
	}
}

void turn_result_sicker(int result)
{
	if(result == 0)
	{
		printf ("修改密码成功\n");
	}
	else
	{
		printf ("修改密码失败\n");
	}
}

void myGuahao_sicker(int fd)//挂号
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;
	DOCTOR_T doct;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	memset(&doct, 0, sizeof(doct));
	memset(&nur, 0, sizeof(nur));
	printf("输入姓名:\n");
	scanf("%s", sick.name);
	printf("输入要挂医生姓名\n");
	scanf("%s",doct.name);
	printf("输入要挂护士姓名\n");
	scanf("%s",nur.name);

	
	len = sizeof(msghead) + sizeof(sick) + sizeof(nur) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 25; //guahao

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &sick, sizeof(sick));
	memcpy(buff + sizeof(msghead) + sizeof(sick), &doct, sizeof(doct));
	memcpy(buff + sizeof(msghead) + sizeof(sick) + sizeof(doct), &nur, sizeof(nur));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("s_guahao_ write");
		return;

	}

}

void guahao_result_sicker(int result)
{
	if(result == 0)
	{
		printf("sicker guahao ok\n");
	}
	else
	{
		printf("sicker guahao fail\n");
	}
}
void register_result_sicker(int result)
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

void login_result_sicker(int result)
{
	if(result == 0)
	{
		printf("登录成功\n");
		flag3 = 1;
	}
	else if(result == 1)
	{
		printf("输入账号有误\n");
		flag3 = -1;
	}
	else
	{
		printf("输入的密码有误\n");
		flag3 = -1;
	}

	return;
}

void myTurn_sicker(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;

	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	char buf[20];
	printf("输入姓名:\n");
	scanf("%s", sick.name);
	printf("输入新密码:\n");
	scanf("%s", sick.passwd);
	printf ("再次输入密码\n");
	scanf("%s",buf);
	if(strcmp(buf,sick.passwd) != 0)
	{
		printf ("两次密码输入不相同\n");
		return;
	}

	
	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 31; //turn

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &sick, sizeof(sick));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("d_turn_ write");
		return;

	}

}

void myRegister_sicker(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;

	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	printf("输入姓名:\n");
	scanf("%s", sick.name);
	printf("输入性别:\n");
	scanf("%s", sick.sex);
	printf("输入年龄:\n");
	scanf("%d", &sick.age);

	strcpy(sick.passwd,"123456");
	
	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 21; //register

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &sick, sizeof(sick));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("s_regi_ write");
		return;

	}

}

void myLogin_sicker(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;

	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	printf("输入姓名:\n");
	scanf("%s", sick.name);
	printf("输入密码:\n");
	scanf("%s", sick.passwd);
	
	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 23; //登录

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &sick, sizeof(sick));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{

	}

}

void mySelect_sicker(int fd)//查看个人信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;
	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	printf("输入姓名:\n");
	scanf("%s", sick.name);
	
	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 27; //查看自己信息

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &sick, sizeof(sick));


	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}

void myLook_sicker(int fd)//查看已执行医嘱
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;
	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	printf("输入姓名:\n");
	scanf("%s", sick.name);
	
	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 29; //查看自己信息

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &sick, sizeof(sick));


	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}

