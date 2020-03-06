#include <stdio.h>
#include <stdlib.h>
typedef int ElementType;
typedef struct Node
{
	ElementType data;
	struct Node *prec;
	struct Node *next;
}Node;

Node *backInster(Node *head, ElementType n)
{
	Node *p = (Node*)malloc(sizeof(Node));
	if (NULL == p){
		perror("malloc");
		return NULL;
	}
	p->data = n;
	p->next = NULL;
	p->prec = NULL;
	if (NULL == head){
		head = p;
		p->prec = head;
		return head;
	}
	Node *t = head;
	while (t->next){
		t = t->next;
	}
	t->next = p;
	p->prec = t;
	return head;
}

Node *frontInster(Node *head, ElementType n)
{
	Node *p = (Node*)malloc(sizeof(Node));
	if (NULL == p){
		perror("malloc");
		return NULL;
	}
	p->data = n;
	p->next = NULL;
	p->prec = NULL;
	
	if (NULL == head){
		p->prec = head;
		head = p;
		return head;
	}
//	p->prec = head;
	p->next = head;
	head->prec = p;
	return p;
}

Node* select(Node *head, ElementType data)
{
	if (NULL == head){
		printf("空链表\n");
		return NULL;
	}
	Node *t = head;
	while (t){
		if (t->data == data)
			return t;
		t = t->next;
	}
	return NULL;
}

//在data后插入newData
Node *inster(Node *head, ElementType data, ElementType newData)
{
	if (NULL == head){
		printf("空链表\n");
		return NULL;
	}
	Node *t = select(head, data);
	if (NULL == t){
		printf("没找到\n");
		return head;
	}else if (t->next == NULL){
		head = backInster(head, newData);
		return head;
	}else {
		Node *p = (Node *)malloc(sizeof(Node));
		if (NULL == p){
			perror("malloc");
			return head;
		}
		p->data = newData;
		p->next = t->next;
		t->next->prec = p;
		t->next = p;
		p->prec = t;
		return head;
	}
}

int update(Node *head, ElementType data, ElementType newData)
{
	if (NULL == head){
		printf("空链表\n");
		return 1;
	}
	Node *t = select(head, data);
	if (t == NULL){
		printf("没找到\n");
		return 1;
	}
	t->data = newData;
	return 0;
}

Node *delete(Node *head, ElementType data)
{	
	if (NULL == head){
		printf("空链表\n");
		return NULL;
	}
	Node *t = select(head, data);
	if (t == NULL){
		printf("没找到\n");
		return head;
	}
	if (t == head){
		head = t->next;
		t->next->prec = head;
		free(t);
		return head;
	}else if (t->next == NULL){
		//此处指向最后一个，让最后一个的前一个的next指针指向NULL
		t->prec->next = NULL;
		free(t);
		return head;
	}else {
		t->prec->next = t->next;
		t->next->prec = t->prec;
		free(t);
		return head;
	}
}

Node *sort(Node *head)
{
	if (NULL == head){
		printf("空链表\n");
		return NULL;
	}
	Node *t1 = head;
	Node *t2 = head;
	for (; t1->next != NULL; t1 = t1->next){
		for (t2 = head; t2->next != NULL; t2 = t2->next){
			if (t2->data > t2->next->data){
				ElementType temp = t2->data;
				t2->data = t2->next->data;
				t2->next->data = temp;
			}
		}
	}
	return head;
}

//此处顺序打印
void print(Node *head)
{
	if (NULL == head){
		printf("链表为空\n");
		return;
	}
	Node *t = head;
	while (t){
		printf("%d ", t->data);
		t = t->next;
	}
	printf("\n");
	return ;
}

//逆序打印
void inversionPrint(Node *head)
{
	Node *t = head;
	while (t){
		printf("%d ", t->data);
		t = t->prec;
	}
	printf("\n");
}


int main()
{
	Node *head = NULL;
	print(head);

	head = backInster(head, 1);
	head = backInster(head, 22);
	head = backInster(head, 6);
	head = backInster(head, 12);
	head = backInster(head, 9);
	head = backInster(head, 44);

	head = frontInster(head, 31);
	head = frontInster(head, 2);
	head = frontInster(head, 8);
	head = frontInster(head, 99);
	head = frontInster(head, 64);
	head = frontInster(head, 87);
	head = frontInster(head, 3);

	print(head);
	
	Node *t = select(head, 44);
	if (t == NULL){
		printf("没找到\n");
	}else{
		printf("data = %d\n", t->data);
	}
	update(head, 31, 11);
	print(head);
	update(head, 2, 22);
	print(head);
	
	printf("插入100在44后\n");
	head = inster(head, 44, 100);
	print(head);
	
	head = sort(head);
	printf("排序后\n");
	print(head);
	head = delete(head, 99);
	printf("删除99后\n");
	print(head);
	
	Node *ver = head;
	while (ver->next){
		ver = ver->next;
	}
	inversionPrint(ver);
	return 0;
}
