#include "root.h"
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

void delete_result_root_sicker(int result)
{
	if(result == 0)
	{
		printf ("删除病人成功\n");
	}
	else
	{
		printf ("删除病人失败\n");
	}
}
void passwd_result_root_sicker(int result)//医生密码初始化
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


void mySelect_passwd_sicker_root(int fd)//病人密码初始化
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;

	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	printf("输入病人姓名:\n");
	scanf("%s",sick.name);
	
	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 309; //passwd

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
		perror("r_passwd_ write");
		return;

	}

}

void mySelect_root_sicker(int fd)//查看病人们的基本信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead);
	msghead.len = len;
	msghead.msgtype = 303; //查询

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
void myDelete_sicker_root(int fd)//删除病人
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;

	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	printf("输入要删除病人姓名\n");
	scanf("%s",sick.name);

	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 307; //删除

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &sick ,sizeof(sick));


	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}
void mySelect_sicker_root(int fd)//查看病人详细信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;
	memset(&sick, 0, sizeof(sick));
	printf("输入要查看病人姓名\n");
	scanf("%s",sick.name);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 311; //查询

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
void mySelect_sick_sicker_root(int fd)//查看该病人医嘱
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	YIZHU_T yz;
	memset(&yz, 0, sizeof(yz));
	printf("输入要查看病人姓名\n");
	scanf("%s",yz.name);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(yz);
	msghead.len = len;
	msghead.msgtype = 313; //查询

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
