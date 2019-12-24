#ifndef _COMMUNAL_H_
#define _COMMUNAL_H_


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
//#include <mysql/mysql.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include "threadpool.h"
#define MAX_BUF 1024
#define MAX_LISTEN 5
#define MAX_EVENT 1024
#define PORT "12345"

typedef struct msghead
{
	int len;
	int msg_type;
	int result;
}MSG_HEAD_T;

typedef struct msgbody
{
	int fd;
	char buf[MAX_BUF];
}MSG_T;

typedef struct doctor
{
	char ID[20];
	char name[20];
	char passwd[20];
	int age;
	char sex[10];
	char lever[20];		//级别 科室主任 主治医生 副主治医生 实习医生
	char department[20];	//科室 外科 内科等
	int room;			//办公室 301
}DOCTOR_T;

extern void doctor_register(int fd, char *buf);
extern void doctor_register_result(int result);


extern void send_result(int fd, int result, int type);

#endif
