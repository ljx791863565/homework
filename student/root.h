#ifndef __ROOT_H__
#define __ROOT_H__

typedef struct Root{
	char name[10];
	char passwd[10];
}Root;

extern void RootLogin();
#endif
