#ifndef _LIONEAR_LIST_H_
#define _LIONEAR_LIST_H_

#define LIST_INT_ISEZ 100
#define LIST_INCREMENT 10
typedef int ElemType;
typedef struct List
{	
	ElemType *elem;
	int length;
	int listsize;
}List;

List *InitList(List &L);
List *InsertList();
#endif
