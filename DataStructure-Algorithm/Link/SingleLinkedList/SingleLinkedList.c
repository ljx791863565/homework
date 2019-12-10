#include "SingleLinkedList.h"

void display(Node *head)
{
	assert(head != NULL);
	while(head != NULL){
		printf("%d ", head->data);
		head = head->next;
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

	if (head == NULL){
		head = tmp;
		return head;
	}
	tmp->next = head;
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

	if (head == NULL){
		head = tmp;
		return head;
	}
	Node *tail = head;
	while (tail->next != NULL){
		tail = tail->next;
	}
	tail->next = tmp;
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
	Node *prep = NULL;
	Node *p = NULL;
	if (head->data == data){
		p = head->next;
		free(head);
		head = p;
	}
	prep = head;
	p = head->next;
	while (p){
		if (p->data == data){
			prep->next = p->next;
			free(p);
			p = prep->next;
			continue;
		}
		prep = p;
		p = p->next;
	}

	return head;
}

//冒泡排序 O(n^2)
Node *sortList(Node *head)
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
	return head;
}

Node *reversalList(Node *head)
{
	assert(head != NULL);
	Node *p1, *p2, *p3;
	p1 = head;
	p2 = p1->next;
	while (p2){
		p3 = p2->next;
		p2->next = p1;
		p1 = p2;
		p2 = p3;
	}
	head->next = NULL;
	head = p1;
	return head;
}
