#ifndef __SERVER2_H__
#define __SERVER2_H__


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



#endif
