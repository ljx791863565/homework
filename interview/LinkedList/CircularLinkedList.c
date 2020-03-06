#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct Node
{
	ElementType data;
	struct Node *next;
	struct Node *prec;
}Node;

//在head后插入
Node *backInster(Node *head, ElementType n)
{
	Node *p = (Node *)malloc(sizeof(Node));
	if (NULL == p){
		perror("malloc");
		return head;
	}
	p->data = n;
	if (NULL == head){
		p->next = p;
		p->prec = p;
		head = p;
		return head;
	}
	p->next = head->next;
	p->prec = head;
	head->next->prec = p;
	head->next = p;
	return head;
}

//在head前插入
Node *frontInster(Node *head, ElementType n)
{
	Node *p = (Node *)malloc(sizeof(Node));
	if (p == NULL){
		perror("malloc");
		return head;
	}
	p->data = n;

	if (NULL == head){
		p->next = p;
		p->prec = p;
		head = p;
		return head;
	}
	p->next = head;
	head->prec->next = p;
	p->prec = head->prec;
	head->prec = p;

	return head;
}

Node *select(Node *head, ElementType n)
{
	if (NULL == head){
		printf("空链表\n");
		return NULL;
	}
	if (head->data == n){
		return head;
	}
	Node *t = head->next;
	while (t != head){
		if (t->data == n){
			return t;
		}
		t = t->next;
	}
	return NULL;
}
Node *delete(Node *head, ElementType n)
{
	if (NULL == head){
		printf("空链表\n");
		return NULL;
	}
	Node *t = select(head, n);
	if (NULL == t){
		printf("没找到\n");
		return head;
	}
	if (t == head){
		head = head->next;
		t->next->prec = t->prec;
		t->prec->next = t->next;
		free(t);
		return head;
	}
	t->next->prec = t->prec;
	t->prec->next = t->next;
	free(t);
	return head;
}

//在data后面插入newData
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
	}
	Node *p = (Node*)malloc(sizeof(Node));
	if (NULL == p){
		perror("malloc");
		return head;
	}
	p->data = newData;
	t->next->prec = p;
	p->next = t->next;
	p->prec = t;
	t->next = p;
	
	return head;
}
int update(Node *head, ElementType data, ElementType newData)
{
	if (NULL == head){
		printf("空链表\n");
		return 1;
	}
	Node *t = select(head, data);
	t->data = newData;
	return 0;

}
//此处返回链表长度
int len(Node *head)
{
	if (NULL == head){
		return 0;
	}
	int n = 1;
	Node *t = head->next;
	while (t != head){
		n++;
		t = t->next;
	}
	return n;
}


//顺序打印
void print(Node *head)
{
	if (NULL == head){
		printf("空链表\n");
		return ;
	}
	printf("%d ", head->data);
	Node *t = head->next;
	while (t != head){
		printf("%d ", t->data);
		t = t->next;
	}
	printf("\n");
}
//逆序打印
void inversionPrint(Node *head)
{
	if (NULL == head){
		printf("空链表\n");
		return ;
	}
	printf("%d ", head->data);
	Node *t = head->prec;
	while (t != head){
		printf("%d ", t->data);
		t = t->prec;
	}
	printf("\n");
}
int main()
{
	Node *head = NULL;
	head = backInster(head, 1);
	head = backInster(head, 3);
	head = backInster(head, 5);
	head = backInster(head, 7);
	head = backInster(head, 9);
	head = backInster(head, 11);
	head = backInster(head, 13);
	
	head = frontInster(head, 2);
	head = frontInster(head, 4);
	head = frontInster(head, 6);
	head = frontInster(head, 8);
	head = frontInster(head, 10);
	head = frontInster(head, 12);
	head = frontInster(head, 14);
	
	Node *t = select(head, 8);
	if (NULL == t){
		printf("链表为空或没找到\n");
	}else{
		printf("找到了 %d\n", t->data);
	}

	int ret = update(head, 6, 66);
	if (ret != 0){
		printf("修改不成功\n");
	}else{
		printf("修改成功\n");
		print(head);
	}

	head = inster(head, 10, 100);
	printf("在10后插入了100\n");
	print(head);

	head = delete(head, 8);
	printf("删除8 后\n");
	print(head);

	printf("逆序打印\n");
	inversionPrint(head);

	return 0;
}
