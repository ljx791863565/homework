#ifndef _LINEAR_LIST_H_
#define _LINEAR_LIST_H_

#define LIST_INIT_SIZE 100			//初始分配线性表储存空间
#define LISTINCREMENT 10			//线性表储存空间分配增量

typedef ElemType int;

typedef struct LinearList {
	ElemType	*elem;			//数据域
	int			length;			//当前线性表长度
	int			listsize;		//线性表当前分配的存储空间
} SqList;

extern int InitList(SqList &L);
extern int ListInsert(SqList &L, int pos, ElemType e);
extern void DestoryList(SqList &L);

#endif
