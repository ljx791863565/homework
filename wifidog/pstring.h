#ifndef _PSTRING_H_
#define _PSTRING_H_

#include <stddef.h>
#include <stdarg.h>

//pascal-like string
typedef struct pstr {
	char *buf;		//保存字符串
	size_t len;		//字符串长度
	size_t size;	//当前最大可保存字符串长度
} pstr_t;

pstr_t *pstr_new(void);
char *pstr_to_string(pstr_t *);
void pstr_cat(pstr_t *, const char *);
int pstr_append_sprintf(pstr_t *, const char *, ...);
#endif //_PSTRING_H_

