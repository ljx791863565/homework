#include "ziplist.h"
//ziplist 压缩表 一系列特殊编码的内存块构成的链表
//每个节点可以保存一个长度受限制的字符数组(没有\0结尾)或整数
//字符数组	-- 长度 <= 63 (2^6-1)
//			-- 长度 <= 16383 (2^14 -1)
//			-- 长度 <= 4294967295 (2^32 -1)
//整数		-- 4位长 
//Ziplist 是为内存占用而特别优化的双链表
//Ziplist 可以保存字符串或者整数 其中整数以整数类型来编码和保存的
//
unsigned char *ziplistNew(void);
unsigned char *ziplistPush(unsigned char *zl, unsigned char *s, unsigned int slen, int where);
unsigned char *ziplistIndex(unsigned char *zl, int index);
unsigned char *ziplistNext(unsigned char *zl, unsigned char *p);
unsigned char *ziplistPrev(unsigned char *zl, unsigned char *p);
unsigned int ziplistGet(unsigned char *p, unsigned char *savl, unsigned int *slen, long long *lval);
unsigned char *ziplistInsert(unsigned char *zl, unsigned char *p, unsigned char *s, unsigned int slen);
unsigned char *ziplistDelete(unsigned char *zl, unsigned char **p);
unsigned char *ziplistDeleteRange(unsigned char *zl, unsigned int index, unsigned int num);
unsigned int ziplistCompare(unsigned char *p, unsigned char *s, unsigned int slen);
unsigned char *ziplistFind(unsigned char *p, unsigned char *vstr, unsigned int vlen, unsigned int skip);
unsigned int ziplistLen(unsigned char *zl);
size_t ziplistBlobLen(unsigned char *zl);


