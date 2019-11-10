#ifndef __SERVER_H__
#define __SERVER_H__


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
#include "doctor.h"

#define MAXLISTEN 100


	
typedef struct msg
{
	int fd;
	char buff[1024];
}MSG_T;




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



#endif
