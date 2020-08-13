#ifndef __ROOT_H__
#define __ROOT_H__

typedef struct Root
{
	char name[20];
	char passwd[20];
} root_t;

extern void root_init(void);

#endif
