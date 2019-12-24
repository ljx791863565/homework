#ifndef __NURES_H__
#define __NURES_H__
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




extern void register_result_nures(int result);
extern void turn_result_nures(int result);
extern void doyizhu_result_nures(int result);
extern void login_result_nures(int result);
extern void myRegister_nures(int fd);
extern void myLogin_nures(int fd);
extern void mySelect_nures(int fd);
extern void myTurn_nures(int fd);
extern void mySelect_all_nures(int fd);
extern void mySelect_yizhu_nures(int fd);
extern void myDo_yizhu_nures(int fd);


#endif
