#ifndef __USER_H__
#define __USER_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct User
{
	char id[20];
	char passwd[20];
} user_t;

typedef struct User_info
{
	user_t user;
	char name[20];
	char sfz[20];
	char sex[4];
	double balance;
} user_info_t;


#endif  //__USER_H__

