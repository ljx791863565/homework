#ifndef __ROOT_H__
#define __ROOT_H__
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
typedef struct root
{
	char ID[20];
	char passwd[20];
}ROOT_T;


int sockfd;
int num;


extern void fun();
extern void fun_doctor();
extern void fun_nures();
extern void fun_sicker();
extern void fun_root();
extern void fun_root_doctor();
extern void fun_root_doctor_1();
extern void fun_root_nures();
extern void fun_root_nures_1();
extern void fun_root_sicker();
extern void fun_root_sicker_1();
extern void update_result_root_doctor(int result);
extern void delete_result_root_doctor(int result);
extern void passwd_result_root_doctor(int result);
extern void mylogin_result_root(int result);
extern void mySelect_passwd_doctor_root(int fd);
extern void myUpdate_doctor_root(int fd);
extern void mylogin_root(int fd);
extern void mySelect_root(int fd);
extern void myDelete_doctor_root(int fd);
extern void mySelect_doctor_root(int fd);


extern void mySelect_sick_doctor_root(int fd);
extern void mySelect_class_doctor_root(int fd);
extern void update_result_root_nures(int result);
extern void delete_result_root_nures(int result);
extern void passwd_result_root_nures(int result);
extern void myUpdate_nures_root(int fd);
extern void myDelete_nures_root(int fd);
extern void mySelect_nures_root(int fd);
extern void myDelete_sick_nures_root(int fd);
extern void mySelect_grade_doctor_root(int fd);


extern void delete_result_root_sicker(int result);
extern void passwd_result_root_sicker(int result);
extern void mySelect_passwd_sicker_root(int fd);
extern void mySelect_root_sicker(int fd);
extern void myDelete_sicker_root(int fd);
extern void mySelect_sicker_root(int fd);
extern void mySelect_sick_sicker_root(int fd);
#endif
