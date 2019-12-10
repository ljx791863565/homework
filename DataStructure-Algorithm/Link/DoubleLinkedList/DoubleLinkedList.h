/*****************************
Copyright(c), 2019, zbt
Author:ljx
Version:1.0.0
Date:2019.11.26
* ****************************/
#ifndef __DOUBLE_LINKED_LIST_H__
#define __DOUBLE_LINKED_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
typedef struct Node{
	int data;
	struct Node* prep;
	struct Node* next;
}Node;
extern Node *display(Node *head);
extern Node *reversalDisplay(Node *head);
extern Node *frontInster(Node *head, int data);
extern Node *backInster(Node *head, int data);
extern Node *searchData(Node *head, int data);
extern Node *deleteData(Node *head, int data);
extern Node *sortList(Node *head);
#endif
