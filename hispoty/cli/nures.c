#include "nures.h"
#include "root.h"
int flag2;
typedef struct sicker
{
	int ID;
	char name[20];
	char passwd[20];
	char sex[10];
	int age;
	char doctor[20];
	char nures[20];
	char atTime[20];
	char CBZD[20];
}SICKER_T;
typedef struct yizhu
{
	int sickID;
	char name[20];
	char sex[20];
	int age;
	char doctor[20];
	char nures[20];
	char time[20];
	char ZD[50];
	int State;
}YIZHU_T;
void fun_nures()
{
	while(1)
	{
		printf_doctor1();
		scanf("%d",&num);
		if(num == 1)
		{
			myRegister_nures(sockfd);
		}
		else if(num == 2)
		{
			while(1)
			{
				flag2 =0;
				myLogin_nures(sockfd);
				while(flag2 == 0);
				if(flag2 == 1)
				{
					break;
				}
			}
			while(1)
			{
				printf_nures1();
				scanf("%d",&num);
				if(num == 1)
				{
					mySelect_nures(sockfd);
				}
				else if(num == 2)
				{
					myTurn_nures(sockfd);
				}
				else if(num == 3)
				{
					mySelect_all_nures(sockfd);
				}
				else if(num == 4)
				{
					mySelect_yizhu_nures(sockfd);
				}
				else if(num == 5)
				{
					myDo_yizhu_nures(sockfd);
				}
				else if(num == 6)
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

void turn_result_nures(int result)
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

void register_result_nures(int result)
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
void doyizhu_result_nures(int result)
{
	if(result == 0)
	{
		printf("已执行医嘱\n");
	}
	else
	{
		printf("执行医嘱失败\n");
	}

}
void login_result_nures(int result)
{
	if(result == 0)
	{
		printf("登录成功\n");
		flag2 = 1;
	}
	else if(result == 1)
	{
		printf("输入账号有误\n");
		flag2 = -1;
	}
	else
	{
		printf("输入的密码有误\n");
		flag2 = -1;
	}

	return;
}

void myTurn_nures(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&nur, 0, sizeof(nur));
	char buf[20];
	printf("输入姓名:\n");
	scanf("%s", nur.name);
	printf("输入新密码:\n");
	scanf("%s", nur.passwd);
	printf ("再次输入密码\n");
	scanf("%s",buf);
	if(strcmp(buf,nur.passwd) != 0)
	{
		printf ("两次密码输入不相同\n");
		return;
	}
	printf("%s\n",nur.passwd);

	
	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 57; //turn

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &nur, sizeof(nur));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("n_turn_ write");
		return;

	}

}

void myRegister_nures(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&nur, 0, sizeof(nur));
	printf("输入ID:\n");
	scanf("%s", nur.ID);
	printf("输入姓名:\n");
	scanf("%s", nur.name);
	printf("输入性别:\n");
	scanf("%s", nur.sex);
	printf("输入所在科室:\n");
	scanf("%d", &nur.class);
	printf("输入护理等级:\n");
	scanf("%s", nur.grade);
//	nur.num = 0;

	strcpy(nur.passwd,"123456");
	
	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 51; //register

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &nur, sizeof(nur));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("n_regi_ write");
		return;

	}

}

void myLogin_nures(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&nur, 0, sizeof(nur));
	printf("输入ID:\n");
	scanf("%s", nur.ID);
	printf("输入密码:\n");
	scanf("%s", nur.passwd);
	
	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 53; //登录

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &nur, sizeof(nur));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{

	}

}
void myDo_yizhu_nures(int fd)//执行医嘱
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	YIZHU_T yz;

	memset(&msghead, 0, sizeof(msghead));
	memset(&yz, 0, sizeof(yz));
	printf("输入执行ID:\n");
	scanf("%d", &yz.sickID);
	printf("输入执行护士姓名:\n");
	scanf("%s", yz.nures);
	
	len = sizeof(msghead) + sizeof(yz);
	msghead.len = len;
	msghead.msgtype = 63; //登录

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &yz, sizeof(yz));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{

	}

}

void mySelect_nures(int fd)//护士查看自己信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&nur, 0, sizeof(nur));

	printf ("输入你的ID\n");
	scanf("%s",nur.ID);


	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 55; //查询

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	char buff[1024] = {0};
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &nur, sizeof(nur));


	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}

void mySelect_all_nures(int fd)//护士查看所有患者信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;

	memset(&msghead, 0, sizeof(msghead));
	SICKER_T sick;

	memset(&sick, 0, sizeof(sick));

	printf ("输入你的姓名\n");
	scanf("%s",sick.nures);


	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 59; //查询

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	
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

void mySelect_yizhu_nures(int fd)//护士查看所有未执行医嘱
{
	int len;//zongchangdu
	MSGHEAD_T msghead;

	memset(&msghead, 0, sizeof(msghead));
	YIZHU_T yz;

	memset(&yz, 0, sizeof(yz));

	printf ("输入你的姓名\n");
	scanf("%s",yz.nures);


	len = sizeof(msghead) + sizeof(yz);
	msghead.len = len;
	msghead.msgtype = 61; //查询

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	
	char buff[1024] = {0};
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &yz, sizeof(yz));



	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}

