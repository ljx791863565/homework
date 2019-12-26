#ifndef __LIST_H__
#define __LIST_H__ 

#include <stdio.h>
#include <stdlib.h>
struct Nodt;
typedef struct Node *PtrToNode;
typedef PtrToNode List;		//表头元素
typedef PtrToNode Position;	//除表头外元素
typedef int ElementType;

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position p, List L);
Position Find(ElementType x, List L);
void Delete(ElementType x, List L);
Position FindPrevious(ElementType x, List L);
void Inster(ElementType x, List L, Position p);
void InsterFront(ElementType x, List L);
void InsterBack(ElementType x, List L);
void DeleteList(List L);
Position Header(List L);
Position First(List L);
Position Tail(List L);
Position Advance(Position p);
ElementType Retrieve(Position p);
void PrintList(List L);
struct Node
{
	ElementType Element;
	struct Node *next;
};
#endif
