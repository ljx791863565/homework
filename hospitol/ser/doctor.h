#ifndef __DOCTOR_H__
#define __DOCTOR_H__
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

typedef struct msghead
{
	int len;//头长度
	int msgtype;//消息类型
	int result;//返回消息类型
}MSGHEAD_T;



extern void register_result_doctor(int result);
extern void login_result_doctor(int result);
extern void turn_result_doctor(int result);
extern void yizhu_delete_result_doctor(int result);
extern void yizhu_update_result_doctor(int result);
extern void yizhu_result_doctor(int result);
extern void myRegister_doctor(int fd);
extern void myLogin_doctor(int fd);
extern void mySelect_doctor(int fd);
extern void myTurn_doctor(int fd);
extern void myYizhu_doctor(int fd);
extern void myYizhu_update_doctor(int fd);
extern void myYizhu_delete_doctor(int fd);
extern void mySelect_all_doctor(int fd);


#endif
