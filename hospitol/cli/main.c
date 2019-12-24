#include "doctor.h"
#include "root.h"
//int sockfd;
//int num;
void printf_root1()
{
	printf("\t1.登陆\t\n");
	printf("\t2.退出\t\n");
	printf("请选择\n");
}
void printf_root2()
{
	printf("\t1.管理医生\t\n");
	printf("\t2.管理护士\t\n");
	printf("\t3.管理病人\t\n");
	printf("\t4.退出\t\n");
	printf("请选择\n");
}
void printf_nures1()
{
	printf("\t1.查看个人信息\t\n");	
	printf("\t2.修改密码\t\n");	
	printf("\t3.查看所有患者信息\t\n");	
	printf("\t4.查看所有待执行医嘱\t\n");	
	printf("\t5.执行医嘱\t\n");	
	printf("\t6.退出\t\n");	
	printf("请选择\n");
}
void printf_sicker1()
{
	printf ("\t1.挂号\t\n");
	printf ("\t2.查看个人信息\t\n");
	printf ("\t3.查看医嘱\t\n");
	printf ("\t4.修改密码\t\n");
	printf ("\t5.退出\t\n");
	printf ("请选择\n");
}
void printf_menu1()
{
	printf ("\t1.医生\t\n");
	printf ("\t2.病人\t\n");
	printf ("\t3.护士\t\n");
	printf ("\t4.管理员\t\n");
	printf ("\t5.退出\t\n");
	printf ("\t请选择\t\n");
}
void printf_doctor1()
{
	printf("\t1.注册\t\n");
	printf("\t2.登录\t\n");
	printf("\t3.退出\t\n");
	printf("\t请选择\t\n");
}

void printf_doctor2()
{
	printf("\t1.查看个人信息\t\n");
	printf("\t2.修改个人密码\t\n");
	printf("\t3.编写医嘱\t\n");//9,10
	printf("\t4.修改医嘱\t\n");//11,12
	printf("\t5.删除医嘱\t\n");//13,14
	printf("\t6.查看全部患者\t\n");//15,16
	printf("\t7.退出\t\n");
	printf("\t请选择\t\n");
}
void *handler(void *arg)
{
	int *p = (int *)arg;
	int fd = *p;
	int ret;
	char buff[1024] = {0};

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

		if(msghead.msgtype == 2)
		{
			register_result_doctor(msghead.result);
		}
		else if(msghead.msgtype == 4)
		{
			login_result_doctor(msghead.result);
		}
		else if(msghead.msgtype == 6)
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}
		}
		else if(msghead.msgtype == 8)
		{
			turn_result_doctor(msghead.result);
		}
		else if(msghead.msgtype == 10)
		{
			yizhu_result_doctor(msghead.result);
		}
		else if(msghead.msgtype == 12)
		{
			yizhu_update_result_doctor(msghead.result);
		}
		else if(msghead.msgtype == 14)
		{
			yizhu_delete_result_doctor(msghead.result);
		}
		else if(msghead.msgtype == 16)//医生查看所有病人信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}
		}
		else if(msghead.msgtype == 22)
		{
			register_result_sicker(msghead.result);
		}
		else if(msghead.msgtype == 24)
		{
			login_result_sicker(msghead.result);
		}
		else if(msghead.msgtype == 26)
		{
			guahao_result_sicker(msghead.result);
		}
		else if(msghead.msgtype == 28)//病人查看自己信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}
		}
		else if(msghead.msgtype == 30)//病人查看自己已执行医嘱
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}
		}
		else if(msghead.msgtype == 32)
		{
			turn_result_sicker(msghead.result);
		}
		else if(msghead.msgtype == 52)
		{
			register_result_nures(msghead.result);
		}
		else if(msghead.msgtype == 54)
		{
			login_result_nures(msghead.result);
		}
		else if(msghead.msgtype == 56)//护士查看自己信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 58)
		{
			turn_result_nures(msghead.result);
		}
		else if(msghead.msgtype == 60)//护士查看所有病人信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 62)//护士查看所有未执行医嘱
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 64)//执行医嘱
		{
			doyizhu_result_nures(msghead.result);
		}
		else if(msghead.msgtype == 102)//管理员登陆
		{
			mylogin_result_root(msghead.result);
		}
		else if(msghead.msgtype == 104)//查看所有基本信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 106)//修改医生信息
		{
			update_result_root_doctor(msghead.result);
		}
		else if(msghead.msgtype == 108)//删除医生
		{
			delete_result_root_doctor(msghead.result);
		}
		else if(msghead.msgtype == 110)//医生密码初始化
		{
			passwd_result_root_doctor(msghead.result);
		}
		else if(msghead.msgtype == 112)//查看医生详细信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 114)//查看该医生的病人医嘱
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 116)//按科室查看医生
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 204)//查看所有基本信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 206)//修改护士信息
		{
			update_result_root_nures(msghead.result);
		}
		else if(msghead.msgtype == 208)//删除护士
		{
			delete_result_root_nures(msghead.result);
		}
		else if(msghead.msgtype == 210)//护士密码初始化
		{
			passwd_result_root_nures(msghead.result);
		}
		else if(msghead.msgtype == 212)//查看护士详细信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 214)//查看该护士的病人医嘱
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 216)//按护理等级查看护士
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}
		}
		else if(msghead.msgtype == 304)//查看所有基本信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 308)//删除病人
		{
			delete_result_root_sicker(msghead.result);
		}
		else if(msghead.msgtype == 310)//病人密码初始化
		{
			passwd_result_root_sicker(msghead.result);
		}
		else if(msghead.msgtype == 312)//查看病人详细信息
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}
		else if(msghead.msgtype == 314)//查看该病人医嘱
		{
			if(msghead.len == sizeof(msghead))
			{
				printf ("no data\n");
			}
			else
			{
				printf("%s\n",buff + sizeof(msghead));
			}

		}

	}
	return NULL;
}
int main(int argc, char **argv)
{
	sockfd = socket(AF_INET,SOCK_STREAM,0);
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
	fun();
	close(sockfd);
	return 0;

}

