#include "doctor.h"
#include "root.h"
int flag1 = 0;
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
void fun_doctor()
{
	while(1)
	{
		printf_doctor1();
		scanf("%d",&num);
		if(num == 1)
		{
			myRegister_doctor(sockfd);
		}
		else if(num == 2)
		{
			while(1)
			{
				flag1 = 0;
				myLogin_doctor(sockfd);
				while(flag1 == 0);
			//	printf("flag1 = %d\n",flag1);
				if(flag1 == 1)
				{
					break;
				}
			}
			while(1)
			{
				printf_doctor2();
				scanf("%d",&num);
				if(num == 1)
				{
					mySelect_doctor(sockfd);
				}
				else if(num == 2)
				{
					myTurn_doctor(sockfd);
				}
				else if(num == 3)
				{
					myYizhu_doctor(sockfd);
				}
				else if(num == 4)
				{
					myYizhu_update_doctor(sockfd);
				}
				else if(num == 5)
				{
					myYizhu_delete_doctor(sockfd);
				}
				else if(num == 6)
				{
					mySelect_all_doctor(sockfd);
				}
				else if(num == 7)					
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
void login_result_doctor(int result)
{
	if(result == 0)
	{
		printf("Dr:登录成功\n");
		flag1 = 1;
	}
	else if(result == 1)
	{
		printf("输入账号有误\n");
		flag1 = -1;
	}
	else
	{
		printf("输入的密码有误\n");
		flag1 = -1;
	}
//	printf("191 flag1 = %d\n",flag1);

	return;
}

void turn_result_doctor(int result)
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
void yizhu_delete_result_doctor(int result)
{
	if(result == 0)
	{
		printf("医嘱已删除\n");
	}
	else
	{
		printf("医嘱删除失败\n");
	}
}

void yizhu_update_result_doctor(int result)
{
	if(result == 0)
	{
		printf("医嘱已修改\n");
	}
	else
	{
		printf("医嘱修改失败\n");
	}

	return;
}
void yizhu_result_doctor(int result)
{
	if(result == 0)
	{
		printf("医嘱已上传\n");
	}
	else
	{
		printf("医嘱填写失败\n");
	}

	return;
}
void register_result_doctor(int result)
{
	if(result == 0)
	{
		printf("Dr:注册成功\n");
	}
	else
	{
		printf("Dr:注册失败\n");
	}

	return;
}

void myTurn_doctor(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;

	memset(&msghead, 0, sizeof(msghead));
	memset(&doct, 0, sizeof(doct));
	char buf[20];
	printf("输入姓名:\n");
	scanf("%s", doct.name);
	printf("输入新密码:\n");
	scanf("%s", doct.passwd);
	printf ("再次输入密码\n");
	scanf("%s",buf);
	if(strcmp(buf,doct.passwd) != 0)
	{
		printf ("两次密码输入不相同\n");
		return;
	}

	
	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 7; //turn

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
		perror("d_turn_ write");
		return;

	}

}
void myRegister_doctor(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;

	memset(&msghead, 0, sizeof(msghead));
	memset(&doct, 0, sizeof(doct));
	printf("输入ID:\n");
	scanf("%s", doct.ID);
	printf("输入姓名:\n");
	scanf("%s", doct.name);
	printf("输入性别:\n");
	scanf("%s", doct.sex);
	printf("输入所在科室:\n");
	scanf("%d", &doct.class);
	printf("输入主治项目:\n");
	scanf("%s", doct.ZZXM);
	doct.num = 0;

	strcpy(doct.passwd,"123456");
	
	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 1; //register

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
		perror("d_regi_ write");
		return;

	}

}

void myYizhu_delete_doctor(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	YIZHU_T yz;

	memset(&msghead, 0, sizeof(msghead));
	memset(&yz, 0, sizeof(yz));
	printf("输入要删除医嘱的病人ID:\n");
	scanf("%d", &yz.sickID);

	
	len = sizeof(msghead) + sizeof(yz);
	msghead.len = len;
	msghead.msgtype = 13; //医嘱删除

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &yz, sizeof(yz));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("d_regi_ write");
		return;

	}

}
void myYizhu_doctor(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	SICKER_T sick;
	YIZHU_T yz;

	memset(&msghead, 0, sizeof(msghead));
	memset(&sick, 0, sizeof(sick));
	memset(&yz, 0, sizeof(yz));
	printf("输入病人ID:\n");
	scanf("%d", &yz.sickID);
	printf("输入病人姓名:\n");
	scanf("%s", yz.name);
	printf("输入病人性别:\n");
	scanf("%s", yz.sex);
	printf("输入病人年龄:\n");
	scanf("%d", &yz.age);
	printf("输入主治医师姓名:\n");
	scanf("%s", yz.doctor);
	printf("输入护理护士姓名\n");
	scanf("%s",yz.nures);
	printf("输入入院时间\n");
	scanf("%s",yz.time);
	printf("输入诊断建议\n");
	scanf("%s",yz.ZD);
	printf("医嘱状态:未执行\n");//1 == 已执行 0 == 未执行
	yz.State = 0;


	
	len = sizeof(msghead) + sizeof(yz) +sizeof(sick);
	msghead.len = len;
	msghead.msgtype = 9; //医嘱填写

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &yz, sizeof(yz));
	memcpy(buff + sizeof(msghead) + sizeof(yz), &sick, sizeof(sick));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("d_yizhu_ write");
		return;

	}

}
void myYizhu_update_doctor(int fd)//修改医嘱
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	YIZHU_T yz;

	memset(&msghead, 0, sizeof(msghead));
	memset(&yz, 0, sizeof(yz));
	printf("输入护理护士的修改:\n");
	scanf("%s", yz.nures);
	printf("输入要修改医嘱病人ID:\n");
	scanf("%d", &yz.sickID);
	printf("输入入院时间修改\n");
	scanf("%s",yz.time);
	printf("输入诊断建议修改\n");
	scanf("%s",yz.ZD);
	printf("输入目前医嘱状态\n");//1 == 已执行 0 == 未执行
	scanf("%d",&yz.State);


	
	len = sizeof(msghead) + sizeof(yz);
	msghead.len = len;
	msghead.msgtype = 11; //医嘱修改

	//zijiezhuanhuan
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//zuzhuang fasongxinxi
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &yz, sizeof(yz));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{	
		perror("d_yizhu_ write");
		return;

	}

}

void myLogin_doctor(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;

	memset(&msghead, 0, sizeof(msghead));
	memset(&doct, 0, sizeof(doct));
	printf("输入ID:\n");
	scanf("%s", doct.ID);
	printf("输入密码:\n");
	scanf("%s", doct.passwd);
	
	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 3; //登录

	//字节序转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的消息
	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &doct, sizeof(doct));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{

	}

}

void mySelect_doctor(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;

	memset(&msghead, 0, sizeof(msghead));
	memset(&doct, 0, sizeof(doct));
	printf("输入姓名\n");
	scanf("%s",doct.name);

	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 5; //查询

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
void mySelect_all_doctor(int fd)
{
	int len;//zongchangdu
	MSGHEAD_T msghead;
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));
	printf("输入姓名\n");
	scanf("%s",doct.name);

	memset(&msghead, 0, sizeof(msghead));

	len = sizeof(msghead) + sizeof(doct);
	msghead.len = len;
	msghead.msgtype = 15; //查询

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
