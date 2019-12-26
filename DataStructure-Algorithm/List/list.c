#include "list.h"

List MakeEmpty(List L)
{	
	Position p = (Position)malloc(sizeof(struct Node));
	p->next = NULL;
	return p;
}
//return true if L is empty
int IsEmpty(List L)
{
	return L->next == NULL;
}

//return true if p is last position in List L
int IsLast(Position p, List L)
{
	return p->next == NULL;
}
Position Find(ElementType x, List L)
{
	Position p;
	 p = L->next;
	 while (p != NULL && p->Element != x){
		p = p->next;
	 }
	 return p;
}
void Delete(ElementType x, List L)
{
	Position p = FindPrevious(x, L);
	Position tmpCell;

	if (!IsLast(p, L)){
		tmpCell = p->next;
		p->next = tmpCell->next;
		free(tmpCell);
	}
}
Position FindPrevious(ElementType x, List L)
{
	Position p;
	p = L;
	while (p->next != NULL && p->next->Element != x){
		p = p->next;
	}
	return p;
}
void Inster(ElementType x, List L, Position p)
{
	Position tmpCell;
	tmpCell = (Position)malloc(sizeof(struct Node));
	if (tmpCell == NULL)
		perror("malloc");
	tmpCell->Element = x;
	tmpCell->next = p->next;
	p->next = tmpCell;
}

void InsterFront(ElementType x, List L)
{
	Position p = Header(L);
	Inster(x, L, p);
}

void InsterBack(ElementType x, List L)
{
	Position p = Tail(L);
	Inster(x, L, p);
}
void DeleteList(List L)
{
	Position p = L->next;
	Position tmpCell;
	while (p != NULL){
		tmpCell = p->next;
		free(p);
		p = tmpCell;
	}
}
Position Header(List L)
{
	return L;
}
Position First(List L)
{
	return L->next;
}
Position Tail(List L)
{
	Position p = L->next;
	while (p->next != NULL)
		p = p->next;
	return p;
}

Position Advance(Position p)
{
	return p->next;
}
ElementType Retrieve(Position p)
{
	if (p != NULL)
		return p->Element;
}
void PrintList(List L)
{
	Position p = L->next;
	while (p != NULL){
		printf("%4d", p->Element);
		p = p->next;
	}
	printf("\n");
}
