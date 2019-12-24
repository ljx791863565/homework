#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#define MAXLISTEN 100

typedef struct msghead
{
	int len;
	int msgtype;
	int result;
}MSGHEAD_T;

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

typedef struct msg
{
	int fd;
	char buff[1024];
}MSG_T;

void send_result(int fd, int result, int msgtype)
{
	MSGHEAD_T msghead;
	int len;

	len = sizeof(msghead);
	msghead.len = len;
	msghead.msgtype = msgtype;
	msghead.result = result;

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	msghead.result = htonl(msghead.result);

	int ret;

	ret = write(fd, &msghead, sizeof(msghead));
	if(ret < 0)
	{}

	return;
}
void myYizhu_doctor(int fd, char * buff)//医嘱填写
{
	SICKER_T sick;
	YIZHU_T yz;
	NURES_T nur;

	memset(&sick, 0, sizeof(sick));
	memset(&yz, 0, sizeof(yz));
	memset(&nur, 0, sizeof(nur));

	memcpy(&yz, buff + sizeof(MSGHEAD_T), sizeof(yz));
	memcpy(&sick, buff + sizeof(MSGHEAD_T) + sizeof(yz), sizeof(sick));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 10);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "insert into yizhu values('%d','%s','%s','%d','%s','%s','%s','%s','%d');", yz.sickID,yz.name,yz.sex,yz.age,yz.doctor,yz.nures,yz.time,yz.ZD,yz.State);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("insert ok\n");
		send_result(fd, 0, 10);
	}
	else
	{
		printf("insert fail:%s\n", sql2);
		send_result(fd, 1, 10);
	}
	char sql3[1024] = {0};
	sprintf(sql3,"update sicker set atTime = '%s' where name = '%s';",yz.time,yz.name);
	ret = mysql_query(&db, sql3);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 10);
	}
	else
	{
		printf("update fail:154 %s\n", sql3);
		send_result(fd, 1, 10);
	}
	char sql4[1024] = {0};
	sprintf(sql4,"update sicker set CBZD = '%s' where name = '%s';",yz.ZD,yz.name);
	ret = mysql_query(&db, sql4);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 10);
	}
	else
	{
		printf("update fail:167 %s\n", sql4);
		send_result(fd, 1, 10);
	}
	char sql5[1024] = {0};
	sprintf(sql5,"update nures set YZGS = YZGS + 1 where name = '%s';",yz.nures);
	ret = mysql_query(&db, sql5);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 10);
	}
	else
	{
		printf("update fail:167 %s\n", sql5);
		send_result(fd, 1, 10);
	}

	mysql_close(&db);

	return;

}
void myYizhu_update_doctor(int fd, char * buff)//医嘱修改
{
	SICKER_T sick;
	YIZHU_T yz;
	NURES_T nur;

	memset(&sick, 0, sizeof(sick));
	memset(&yz, 0, sizeof(yz));
	memset(&nur, 0, sizeof(nur));

	memcpy(&yz, buff + sizeof(MSGHEAD_T), sizeof(yz));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 12);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "update yizhu set time = '%s',ZD= '%s',State = '%d' where sickerID = '%d';",yz.time,yz.ZD,yz.State,yz.sickID);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("insert ok\n");
		send_result(fd, 0, 12);
	}
	else
	{
		printf("insert fail:%s\n", sql2);
		send_result(fd, 1, 12);
	}

	if(yz.State == 1)
	{
		char sql5[1024] = {0};
		sprintf(sql5, "update nures set YZGS = YZGS - 1 where name = '%s';",yz.nures);

		ret = mysql_query(&db, sql5);
		if(ret == 0)
		{
			printf("insert ok\n");
			send_result(fd, 0, 12);
		}
		else
		{
			printf("insert fail:%s\n", sql5);
			send_result(fd, 1, 12);
		}

	}
	else if(yz.State == 0)
	{
		char sql6[1024] = {0};
		sprintf(sql6, "update nures set YZGS = YZGS + 1 where name = '%s';",yz.nures);

		ret = mysql_query(&db, sql6);
		if(ret == 0)
		{
			printf("insert ok\n");
			send_result(fd, 0, 12);
		}
		else
		{
			printf("insert fail:%s\n", sql6);
			send_result(fd, 1, 12);
		}
	}
	char sql3[1024] = {0};
	sprintf(sql3,"update sicker set atTime = '%s' where ID = '%d';",yz.time,yz.sickID);
	ret = mysql_query(&db, sql3);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 12);
	}
	else
	{
		printf("update fail:255 %s\n", sql3);
		send_result(fd, 1, 12);
	}
	char sql4[1024] = {0};
	sprintf(sql4,"update sicker set CBZD = '%s' where ID = '%d';",yz.ZD,yz.sickID);
	ret = mysql_query(&db, sql4);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 12);
	}
	else
	{
		printf("update fail:268 %s\n", sql4);
		send_result(fd, 1, 12);
	}

	mysql_close(&db);

	return;

}

void myRegister(int fd, char * buff)
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buff + sizeof(MSGHEAD_T), sizeof(doct));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 2);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "insert into doctor values('%s','%s','%s','%d','%s','%s','%d');", doct.ID,doct.name,doct.passwd,doct.class,doct.sex,doct.ZZXM,doct.num);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("insert ok\n");
		send_result(fd, 0, 2);
	}
	else
	{
		printf("insert fail:%s\n", sql2);
		send_result(fd, 1, 2);
	}

	mysql_close(&db);

	return;

}

void myLogin(int fd, char * buff)
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buff + sizeof(MSGHEAD_T), sizeof(doct));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 2);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "select passwd from doctor where ID ='%s';", doct.ID);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 4);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	MYSQL_ROW result;
	rows = mysql_num_rows(res);
	if(rows > 0)
	{
		result = mysql_fetch_row(res);
		if(strcmp(result[0], doct.passwd) == 0)
		{
			send_result(fd, 0, 4);
		}
		else
		{
			send_result(fd, 2, 4);
		}
	}
	else
	{
		send_result(fd, 1, 4);//4表示登录的返回消息类型, 1表示用户名输错
	}

	mysql_close(&db);

	return;

}
void myYizhu_delete(int fd, char * buff)//医嘱删除
{
	YIZHU_T yz;
	memset(&yz, 0, sizeof(yz));

	memcpy(&yz, buff + sizeof(MSGHEAD_T), sizeof(yz));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 14);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "delete from yizhu where sickerID ='%d';", yz.sickID);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("delete ok\n");
	}
	else
	{
		printf("delete fail:%s\n", sql2);
		send_result(fd, 1, 14);
	}


	mysql_close(&db);

	return;

}

void myTurn(int fd, char * buff)
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buff + sizeof(MSGHEAD_T), sizeof(doct));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 8);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "update doctor set passwd = '%s' where name ='%s';", doct.passwd,doct.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("update ok\n");
	}
	else
	{
		printf("update fail:%s\n", sql2);
		send_result(fd, 1, 8);
	}


	mysql_close(&db);

	return;

}

void send_select_result(int fd, int msgtype, char *p)
{
	MSGHEAD_T msghead;

	int len;///发送消息的总长度

	len = sizeof(msghead) + strlen(p);

	msghead.len = len;
	msghead.msgtype = msgtype;

	//字节序的转换
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的buff

	char buff[1024] = {0};

	memcpy(buff, &msghead, sizeof(msghead));

	if(p != NULL)
	{
		strcpy(buff + sizeof(msghead), p);
	}

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{}

	return;
}
void mySelect(int fd,char *buf)
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buf + sizeof(MSGHEAD_T), sizeof(doct));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 2);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "select * from doctor where name = '%s';",doct.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 4);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 6);
	}

	int column;
	column = mysql_num_fields(res);
	char buff[1000] = {0};
	memset(buff,0,sizeof(buff));
	int i;
	int j;

	MYSQL_ROW result;
	for(i = 0; i < rows; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j < column; j++)
		{
			sprintf(buff, "%s%10.10s",buff, result[j]);
		}
		sprintf(buff,"%s%s",buff, "\n");
	}



	send_select_result(fd, 6, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void mySelect_all(int fd ,char *buf)
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buf + sizeof(MSGHEAD_T), sizeof(doct));
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 16);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "select * from sicker where doctor = '%s';",doct.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 16);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 16);
	}

	int column;
	column = mysql_num_fields(res);
	char buff[1000] = {0};
	memset(buff,0,sizeof(buff));
	int i;
	int j;

	MYSQL_ROW result;
	for(i = 0; i < rows; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j < column; j++)
		{
			sprintf(buff, "%s%10.10s",buff, result[j]);
		}
		sprintf(buff,"%s%s",buff, "\n");
	}



	send_select_result(fd, 16, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}

void * myworker(void *arg)
{
	MSG_T * p;
	p = (MSG_T *)arg;

	MSGHEAD_T msghead;

	memcpy(&msghead, p->buff, sizeof(msghead));
	//zijiexuzhuanhuan
	msghead.len = ntohl(msghead.len);
	msghead.msgtype = ntohl(msghead.msgtype);

//	printf(" type = %d\n",msghead.msgtype);
	if(msghead.msgtype == 1)//register
	{
		myRegister(p->fd, p->buff);
	}
	else if(msghead.msgtype == 3)//login
	{
		myLogin(p->fd, p->buff);
	}
	else if(msghead.msgtype == 5)//查询
	{
		mySelect(p->fd,p->buff);
	}
	else if(msghead.msgtype == 7)//修改密码
	{
		myTurn(p->fd,p->buff);
	}
	else if(msghead.msgtype == 9)//医嘱填写
	{
		myYizhu_doctor(p->fd,p->buff);
	}
	else if(msghead.msgtype == 11)//医嘱修改
	{
		myYizhu_update_doctor(p->fd,p->buff);
	}
	else if(msghead.msgtype == 13)//医嘱删除
	{
		myYizhu_delete(p->fd,p->buff);
	}
	else if(msghead.msgtype == 15)//查询全部病人
	{
		mySelect_all(p->fd,p->buff);
	}
	else if(msghead.msgtype == 21)//病人注册
	{
		myRegister_sicker(p->fd,p->buff);
	}
	else if(msghead.msgtype == 23)//病人登录
	{
		myLogin_sicker(p->fd,p->buff);
	}
	else if(msghead.msgtype == 25)//挂号
	{
		myGuahao_sicker(p->fd,p->buff);
	}
	else if(msghead.msgtype == 27)
	{
		mySelect_sicker(p->fd,p->buff);//查看自己消息
	}
	else if(msghead.msgtype == 29)
	{
		myLook_sicker(p->fd,p->buff);//查看自己消息
	}
	else if(msghead.msgtype == 31)//病人修改密码
	{
		myTurn_sicker(p->fd,p->buff);
	}
	else if(msghead.msgtype == 51)//护士注册
	{
		myRegister_nures(p->fd, p->buff);
	}
	else if(msghead.msgtype == 53)//护士登陆
	{
		myLogin_nures(p->fd, p->buff);
	}
	else if(msghead.msgtype == 55)//护士查看自己信息
	{
		mySelect_nures(p->fd, p->buff);
	}
	else if(msghead.msgtype == 57)//护士修改密码
	{
		myTurn_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 59)//护士查看所有病人信息
	{
		mySelect_all_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 61)//护士查看所有未执行医嘱
	{
		mySelect_yizhu_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 63)//护士执行医嘱
	{
		myDO_yizhu_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 101)//管理员登陆
	{
		myLogin_root(p->fd,p->buff);
	}
	else if(msghead.msgtype == 103)//查看医生基本信息
	{
		mySelect_root(p->fd);
	}
	else if(msghead.msgtype == 105)//修改医生信息
	{
		myUpdate_root(p->fd,p->buff);
	}
	else if(msghead.msgtype == 107)//删除医生
	{
		myDelete_root_doctor(p->fd,p->buff);
	}
	else if(msghead.msgtype == 109)//医生密码初始化
	{
		mypasswd_root_doctor(p->fd,p->buff);
	}
	else if(msghead.msgtype == 111)//查看医生详细信息
	{
		Select_root_doctor(p->fd,p->buff);
	}
	else if(msghead.msgtype == 113)//查看病人医嘱
	{
		Select_yizhu_root_doctor(p->fd,p->buff);
	}
	else if(msghead.msgtype == 115)//按科室查看医生
	{
		Select_class_root_doctor(p->fd,p->buff);
	}
	else if(msghead.msgtype == 203)//查看护士基本信息
	{
		mySelect_root_nures(p->fd);
	}
	else if(msghead.msgtype == 205)//修改护士信息
	{
		myUpdate_root_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 207)//删除护士
	{
		myDelete_root_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 209)//护士密码初始化
	{
		mypasswd_root_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 211)//查看护士详细信息
	{
		Select_root_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 213)//查看该护士护理病人的医嘱
	{
		Select_yizhu_root_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 215)//按护理等级查看护士
	{
		Select_grade_root_nures(p->fd,p->buff);
	}
	else if(msghead.msgtype == 303)//查看病人基本信息
	{
		mySelect_root_sicker(p->fd);
	}
	else if(msghead.msgtype == 307)//删除病人
	{
		myDelete_root_sicker(p->fd,p->buff);
	}
	else if(msghead.msgtype == 309)//病人密码初始化
	{
		mypasswd_root_sicker(p->fd,p->buff);
	}
	else if(msghead.msgtype == 311)//查看病人详细信息
	{
		Select_root_sicker(p->fd,p->buff);
	}
	else if(msghead.msgtype == 313)//查看该病人的医嘱
	{
		Select_yizhu_root_sicker(p->fd,p->buff);
	}

}

int main(int argc, char ** argv) //./server 8080
{
	if(argc != 2)
	{
		printf("para errno\n");
		return -1;
	}

	int epfd;//epoll文件描述符

	epfd = epoll_create(MAXLISTEN);
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret;

	ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}

	ret = listen(sockfd, MAXLISTEN);
	if(ret < 0)
	{
		perror("listen");
		return -1;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sockfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);//把sockfd添加到epoll监听队列

	struct epoll_event events[20];//保存通知的事件
	int count;//保存通知事件的个数

	char buff[1024];
	int i;
	int clientFd;
	struct sockaddr_in clientAddr;
	int cliLen = sizeof(clientAddr);
	int readFd;	

	MSGHEAD_T msghead;

	pool_init(6);//线程池的创建，初始化

	MSG_T *pmsg;
	while(1)
	{
		count = epoll_wait(epfd, events, 20, 500);
		for(i = 0; i < count; i++)//遍历所有的通知事件
		{
			if(events[i].data.fd == sockfd)//客户端第一次连接
			{
				clientFd = accept(sockfd, (struct sockaddr *)&clientAddr, &cliLen);
				if(clientFd < 0)
				{
					continue;
				}
				printf("a new client connect ok\n");

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = clientFd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientFd, &ev);
			}
			else if(events[i].events & EPOLLIN) //表示管道有数据可以读取
			{
				readFd = events[i].data.fd;
				memset(buff, 0, 1024);

				ret = read(readFd, buff, 1024);
			if(ret < 0)
				{
					continue;
				}
				if(ret == 0)
				{
					continue;
				}

				pmsg = (MSG_T *)malloc(sizeof(MSG_T));
				if(pmsg == NULL)
				{
					//printf...
					return -1;
				}

				memset(pmsg, 0, sizeof(MSG_T));
				pmsg->fd = readFd;
				memcpy(pmsg->buff, buff, ret);

				pool_add_worker(myworker, pmsg);
			}
		}
	}

	return 0;
}
