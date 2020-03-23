#include <stdio.h>

typedef struct node{
	int data;
	Node *next;
} Node;

//将链表逆序
//思路参考 https://blog.csdn.net/niuer09/article/details/5961004
Node *reverseList(Node *head)
{
	if (head == NULL || head->next == NULL) {
		return head;
	}

	Node *p1, *p2, *p3;
	p1 = head;
	p2 = p1->next;
	p3 = p2->next;
	//将头结点的下一个指向NULL 用作尾节点
	p1->next = NULL;

	//三个指针从第二个节点开始逆序　一次逆序一个节点
	while (p3 != NULL) {
		p2->next = p1;
		p1 = p2;
		p2 = p3;
		p3 = p3->next;
	}

	p2->next = p1;
	head = p2;
	return head;
}
