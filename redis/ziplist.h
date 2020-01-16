#ifndef _ZIPLIST_H_
#define _ZIPLIST_H_ 

#define ZIPLIST_HEAD 0
#define ZIPLIST_TAIL 1

//创建一个新的ziplist	
unsigned char *ziplistNew(void);

//将一个包含给定值的新entry push进ziplist的表头或者表尾 O(N^2)
unsigned char *ziplistPush(unsigned char *zl, unsigned char *s, unsigned int slen, int where);

unsigned char *ziplistIndex(unsigned char *zl, int index);
unsigned char *ziplistNext(unsigned char *zl, unsigned char *p);
unsigned char *ziplistPrev(unsigned char *zl, unsigned char *p);
unsigned int ziplistGet(unsigned char *p, unsigned char **savl, unsigned int *slen, long long *lval);

//将一个包含给定值的新entry插入指定地址 O(N^2)
unsigned char *ziplistInsert(unsigned char *zl, unsigned char *p, unsigned char *s, unsigned int slen);

//删除给定地址上的节点 O(N^2)
unsigned char *ziplistDelete(unsigned char *zl, unsigned char **p);

//范围删除 根据给定的index和num指导删除 O(N^2)
unsigned char *ziplistDeleteRange(unsigned char *zl, unsigned int index, unsigned int num);
unsigned int ziplistCompare(unsigned char *p, unsigned char *s, unsigned int slen);

//在ziplist上查找给定值的节点
unsigned char *ziplistFind(unsigned char *p, unsigned char *vstr, unsigned int vlen, unsigned int skip);

//返回节点数量 zllen
unsigned int ziplistLen(unsigned char *zl);

//以字节为单位返回ziplist总共占用内存
size_t ziplistBlobLen(unsigned char *zl);

#endif
