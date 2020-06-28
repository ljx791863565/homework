#ifndef __COMMON_H__
#define __COMMON_H__

#include "unp.h"
typedef struct msg
{
	int fd;
	char buf[BUFSIZE];
}MSG_T;

typedef struct msghead
{
	int len;
	int msgtype;
	int result;
}MSGHEAD_T;

#endif //__COMMON_H__
