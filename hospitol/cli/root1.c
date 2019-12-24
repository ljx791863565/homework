#include "root.h"
//int sockfd;
int flag4;
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
typedef struct msghead
{
	int len;//头长度
	int msgtype;//消息类型
	int result;//返回消息类型
}MSGHEAD_T;

void fun_root()
{
	while(1)
	{
		printf_root1();
		scanf("%d",&num);
		if(num == 1)
		{
			while(1)
			{
				flag4 = 0;
				mylogin_root(sockfd);
				while(flag4 == 0);
				if(flag4 == 1)
				{
					break;
				}
			}
			while(1)
			{
				printf_root2();
				scanf("%d",&num);
				if(num == 1)
				{
					fun_root_doctor();
				}
				else if(num == 2)
				{
					fun_root_nures();
				}
				else if(num == 3)
				{
					fun_root_sicker();
				}
				else if(num == 4)
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
		else if(num == 2)
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
void update_result_root_doctor(int result)
{
	if(result == 0)
	{
		printf ("修改医生信息成功\n");
	}
	else
	{
		printf ("修改医生信息失败\n");
	}
}
void delete_result_root_doctor(int result)
{
	if(result == 0)
	{
		printf ("删除医生成功\n");
	}
	else
	{
		printf ("删除医生失败\n");
	}
}
void passwd_result_root_doctor(int result)//医生密码初始化
{
	if(result == 0)
	{
		printf("密码已初始化\n");
	}
	else
	{
		printf("密码初始化失败\n");
	}
}


void mylogin_result_root(int result)
{
	if(result == 0)
	{
		printf("root:登录成功\n");
		flag4 = 1;
	}
	else if(result == 1)
	{
		printf("输入账号有误\n");
		flag4 = -1;
	}
	else
	{
		printf("输入的密码有误\n");
		flag4 = -1;
	}

	return;
}
void mySelect_passwd_doctor_root(int fd)//医生密码初始化
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;

	memset(&msghead, 0, sizeof(msghead));
	memset(&doct, 0, sizeof(doct));
	printf("输入医生姓名:\n");
	scanf("%s", doct.name);
	
	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 109; //passwd

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &doct, sizeof(doct));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("r_passwd_ write");
		return;

	}

}
void myUpdate_doctor_root(int fd)//修改医生基本信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;

	memset(&msghead, 0, sizeof(msghead));
	memset(&doct, 0, sizeof(doct));
	printf("输入修改医生姓名\n");
	scanf("%s",doct.name);
	printf("输入医生科室修改\n");
	scanf("%d",&doct.class);
	printf("输入医生主治项目修改\n");
	scanf("%s",doct.ZZXM);


	
	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 105; //医嘱修改

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &doct, sizeof(doct));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("root_up_ write");
		return;

	}

}

void mylogin_root(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	ROOT_T root;

	memset(&msghead, 0, sizeof(msghead));
	memset(&root, 0, sizeof(root));
	printf("输入ID:\n");
	scanf("%s", root.ID);
	printf("输入密码:\n");
	scanf("%s", root.passwd);
	
	len = sizeof(msghead) + sizeof(root);
	msghead.len = len;
	msghead.msgtype = 101; //登录

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &root, sizeof(root));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{

	}

}

void mySelect_root(int fd)//查看医生们的基本信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead);
	msghead.len = len;
	msghead.msgtype = 103; //查询

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);



	int ret;

	ret = write(fd, &msghead, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}
void myDelete_doctor_root(int fd)//删除医生
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;

	memset(&msghead, 0, sizeof(msghead));
	memset(&doct, 0, sizeof(doct));
	printf("输入要删除医生姓名\n");
	scanf("%s",doct.name);

	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 107; //删除

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &doct ,sizeof(doct));


	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}
void mySelect_doctor_root(int fd)//查看医生详细信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));
	printf("输入要查看医生姓名\n");
	scanf("%s",doct.name);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 111; //查询

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);


	char buff[1024] = {0};
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &doct, sizeof(doct));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}
void mySelect_sick_doctor_root(int fd)//查看该医生的病人医嘱
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	YIZHU_T yz;
	memset(&yz, 0, sizeof(yz));
	printf("输入要查看医生姓名\n");
	scanf("%s",yz.doctor);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(yz);
	msghead.len = len;
	msghead.msgtype = 113; //查询

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
void mySelect_class_doctor_root(int fd)//按科室查看医生详细信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));
	printf("输入要查看科室\n");
	scanf("%d",&doct.class);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 115; //查询

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);


	char buff[1024] = {0};
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &doct, sizeof(doct));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}
