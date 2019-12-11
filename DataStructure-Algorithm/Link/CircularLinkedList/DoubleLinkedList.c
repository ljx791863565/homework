#include "DoubleLinkedList.h"

void display(Node *head)
{
	assert(head != NULL);
	while(head != NULL){
		printf("%d ", head->data);
		head = head->next;
	}
	printf("\n");
}

void reversalDisplay(Node *head)
{
	assert(head != NULL);
	Node *tmp = head;
	while (tmp->next != NULL){
		tmp = tmp->next;
	}

	while (tmp != NULL){
		printf("%d ",tmp->data);
		tmp = tmp->prep;
	}
	printf("\n");
}
Node *frontInster(Node *head, int data)
{
	Node *tmp = (Node *)malloc(sizeof(Node *));
	if (tmp == NULL){
		perror("malloc");
		return NULL;
	}
	tmp->data = data;
	tmp->next = NULL;
	tmp->prep = NULL;

	if (head == NULL){
		head = tmp;
		return head;
	}
	tmp->next = head;
	head->prep = tmp;
	head = tmp;
	return head;
}

Node *backInster(Node *head, int data)
{	
	Node *tmp = (Node *)malloc(sizeof(Node *));
	if (tmp == NULL){
		perror("malloc");
		return NULL;
	}
	tmp->data = data;
	tmp->next = NULL;
	tmp->prep = NULL;

	if (head == NULL){
		head = tmp;
		return head;
	}
	Node *tail = head;
	while (tail->next != NULL){
		tail = tail->next;
	}
	tail->next = tmp;
	tmp->prep = tail;
	return head;
}

Node *searchData(Node *head, int data)
{
	assert(head != NULL);
	while (head->next != NULL){
		if (head->data == data){
			return head;		//此时找到的是第一个出现的数据如果有多个的话
		}
		head = head->next;
	}
	return NULL;		
}

Node *deleteData(Node *head, int data)
{
	assert(head != NULL);
	Node *q = NULL;
	Node *p = NULL;
	if (head->data == data){
		p = head->next;
		free(head);
		head = p;
		p->prep = NULL;
	}
	q = head;
	p = head->next;
	while (p){
		if (p->data == data){
			p->prep->next = p->next;
			p->next->prep = p->prep;
			free(p);
			p = q->next;
			continue;
		}
		q = p;
		p = p->next;
	}

	return head;
}

//冒泡排序 O(n^2)
void sortList(Node *head)
{
	assert(head != NULL);
	Node *p = head;
	Node *q = NULL;
	while (p != q){
		while(p->next != q){
			if (p->data > p->next->data){
				//只交换节点的值
				int tmp = p->data;
				p->data = p->next->data;
				p->next->data = tmp;
			}
			p = p->next;
		}
		q = p;	//此时p指向最后一个节点，相当于不再比较最后一个节点了
		p = head;	//继续从头开始再一次循环
		
	}
}

