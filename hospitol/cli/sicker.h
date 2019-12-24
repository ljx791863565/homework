#ifndef __SICKER_H__
#define __SICKER_H__
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

typedef struct msghead
{
	int len;//头长度
	int msgtype;//消息类型
	int result;//返回消息类型
}MSGHEAD_T;




extern void register_result_sicker(int result);
extern void login_result_sicker(int result);
extern void turn_result_sicker(int result);
extern void myRegister_sicker(int fd);
extern void myLogin_sicker(int fd);
extern void myGuahao_sicker(int fd);
extern void turn_result_nures(int result);
extern void mySelect_sicker(int fd);
extern void myLook_sicker(int fd);
extern void myTurn_sicker(int fd);


#endif
