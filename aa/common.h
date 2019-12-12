#ifndef __COMMON_H__
#define __OOMMON_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>

typedef struct user_list
{
	int number;
	int socketfd;
	struct in_addr sin_addr;
	struct user_list *next;
}USER_LIST;

#define MAX_BUF 1024
#endif
