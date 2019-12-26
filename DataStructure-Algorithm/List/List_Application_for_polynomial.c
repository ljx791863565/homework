#include <stdio.h>
#include <stdlib.h>
#include "List_Application_for_polynomial.h"

Polynomial initPolynomial(Polynomial head)
{
	head = (Polynomial)malloc(sizeof(struct Node));
	if (head == NULL){
		perror("malloc");
		return NULL;
	}
	head->next = NULL;

	return head;
}

void Insert(int coeff, int expo, Polynomial head)
{
	Polynomial p = (Polynomial)malloc(sizeof(struct Node));
	if (p == NULL){
		perror("malloc");
		return;
	}
	p->coefficient = coeff;
	p->exponent = expo;

	//表空，无节点
	if (head->next == NULL){
		head->next = p;
		p->next = NULL;
	}else {
		Polynomial tmp = head->next;
		Polynomial prev = head;
		while (tmp->next != NULL){
			if (tmp->exponent < expo){
				prev->next = p;
				p->next = tmp;
				return;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		tmp->next = p;
		p->next = NULL;
	}
}
Polynomial Find(int expo, Polynomial head)
{
	if (head->next == NULL)
		return NULL;
	Polynomial tmp = head->next;
	while (tmp != NULL && tmp->exponent != expo){
		tmp = tmp->next;
	}
	return tmp;
}
void Delete(int expo, Polynomial head)
{
	Polynomial p = FindPrevious(expo, head);
	Polynomial tmpCell;

	if (p->next != NULL){
		tmpCell = p->next;
		p->next = tmpCell->next;
		free(tmpCell);
	}
}

void DeleteRepeat(Polynomial head)
{
	if (head->next == NULL)
		return;
	Polynomial tmp = head->next;
	Polynomial prev = head;
	while (tmp->next != NULL){
		if(tmp->exponent == tmp->next->exponent){
			
		}
	}
}

Polynomial FindPrevious(int expo, Polynomial head)
{
	Polynomial p;
	p = head;
	while (p->next != NULL && p->next->exponent != expo){
		p = p->next;
	}
	return p;
}

void PrintPolynomial(Polynomial head)
{
	Polynomial tmp = head->next;
	while (tmp != NULL){
		printf("%dX^%d	", tmp->coefficient, tmp->exponent);
		tmp = tmp->next;
	}
	printf("\n");
}
void MergePolynomial(Polynomial p1, Polynomial p2)
{
	if (p1->next == NULL){
		p1 = p2;
		return;
	}else if (p2->next == NULL){
		return;
	}
	Polynomial tmp = p2;
	while (tmp->next != NULL){
		Insert(tmp->coefficient, tmp->exponent, p1);
		tmp = tmp->next;
	}
	PrintPolynomial(p1);
}
void AddPolynomial(Polynomial p1, Polynomial p2, Polynomial psum)
{
	MergePolynomial(psum, p1);
	MergePolynomial(psum, p2);
}

