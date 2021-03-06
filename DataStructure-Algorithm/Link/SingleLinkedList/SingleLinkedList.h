/*****************************
Copyright(c), 2019, zbt
Author:ljx
Version:1.0.0
Date:2019.11.26
* ****************************/
#ifndef __SINGLE_LINKED_LIST_H__
#define __SINGLE_LINKED_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
typedef struct Node{
	int data;
	struct Node* next;
}Node;
extern void display(Node *head);
extern Node *frontInster(Node *head, int data);
extern Node *backInster(Node *head, int data);
extern Node *searchData(Node *head, int data);
extern Node *deleteData(Node *head, int data);
extern Node *sortList(Node *head);
extern Node *reversalList(Node *head);
#endif
