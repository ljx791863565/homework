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

void update_result_root_nures(int result)
{
	if(result == 0)
	{
		printf ("修改护士信息成功\n");
	}
	else
	{
		printf ("修改护士信息失败\n");
	}
}
void delete_result_root_nures(int result)
{
	if(result == 0)
	{
		printf ("删除护士成功\n");
	}
	else
	{
		printf ("删除护士失败\n");
	}
}
void passwd_result_root_nures(int result)//护士密码初始化
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


void mySelect_passwd_nures_root(int fd)//护士密码初始化
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&nur, 0, sizeof(nur));
	printf("输入护士姓名:\n");
	scanf("%s", nur.name);
	
	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 209; //passwd

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
		perror("r_passwd_ write");
		return;

	}

}
void myUpdate_nures_root(int fd)//修改护士基本信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&nur, 0, sizeof(nur));
	printf("输入修改护士姓名\n");
	scanf("%s",nur.name);
	printf("输入医生科室修改\n");
	scanf("%d",&nur.class);
	printf("输入护士护理等级修改\n");
	scanf("%s",nur.grade);


	
	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 205; //

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
		perror("root_up_ write");
		return;

	}

}


void mySelect_root_nures(int fd)//查看护士们的基本信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead);
	msghead.len = len;
	msghead.msgtype = 203; //查询

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
void myDelete_nures_root(int fd)//删除护士
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;

	memset(&msghead, 0, sizeof(msghead));
	memset(&nur, 0, sizeof(nur));
	printf("输入要删除护士姓名\n");
	scanf("%s",nur.name);

	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 207; //删除

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &nur ,sizeof(nur));


	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		printf ("write");
		return;
	}
}
void mySelect_nures_root(int fd)//查看护士详细信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;
	memset(&nur, 0, sizeof(nur));
	printf("输入要查看护士姓名\n");
	scanf("%s",nur.name);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 211; //查询

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
void mySelect_sick_nures_root(int fd)//查看该护士的病人医嘱
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	YIZHU_T yz;
	memset(&yz, 0, sizeof(yz));
	printf("输入要查看护士姓名\n");
	scanf("%s",yz.nures);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(yz);
	msghead.len = len;
	msghead.msgtype = 213; //查询

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
void mySelect_grade_nures_root(int fd)//按护理等级查看医生详细信息
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	NURES_T nur;
	memset(&nur, 0, sizeof(nur));
	printf("输入护理等级\n");
	scanf("%s",nur.grade);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(nur);
	msghead.len = len;
	msghead.msgtype = 215; //查询

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
