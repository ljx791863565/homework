#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DOUBLE 1
#define eps_0 1.0e-9
#define rel_error 10e-4

typedef int ElementType;
typedef struct Node
{
	ElementType data;
	struct Node *next;
}Node;

//尾插入
Node *backInster(Node *head, ElementType n)
{
	Node *p = (Node *)malloc(sizeof(Node));
	if (NULL == p){
		perror("malloc");
		return NULL;
	}
	p->data = n;
	p->next = NULL;
	if (NULL == head){
		head = p;
		return head;
	}
	Node *t = head;
	while (t->next){
		t = t->next;
	}
	t->next = p;
	return head;
}

//头插入
Node *frontInster(Node *head, ElementType n)
{
	Node *p = (Node *)malloc(sizeof(Node));
	p->data = n;
	if (NULL == head){
		p->next = head;
		return p;
	}
	p->next = head;
	return p;
}

/*
//条件编译 当elementType为double 或 flout时开放此编译。
//比较两数是否相等，主要是浮点数
//返回0 相等
//返回1 不相等
int isEquals(const ElementType data1, const ElementType data2)
{
	if (fabs(data1 - data2) <= eps_0){
		return 0;
	}else {
		if (fabs(data1) >= 1000.0){
			if (fabs(data1 - data2) <= rel_error * fabs(data1)){
				return 0;
			}
		}
	}
	return 1;
}
*/

//查找data返回该节点的指针
Node* select(Node *head, ElementType data)
{
	Node *t = head;
	if (NULL == head){
		printf("链表为空\n");
		return NULL;
	}
	while (t->next){
		if (t->data == data)
			return t;
		t = t->next;
	}
	return NULL;
}

//在data后插入newData
Node *inster(Node *head, ElementType data ,ElementType newData)
{
	if (NULL == head){
		printf("空链表\n");
		return NULL;
	}
	Node *p = (Node *)malloc(sizeof(Node));
	if (p == NULL){
		perror("malloc");
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
		p->data = newData;
		p->next = t->next;
		t->next = p;
		return head;
	}
}
//修改一个节点的值,成功返回1， 否则返回0
int update(Node *head, ElementType data, ElementType newData)
{
	Node *t = select(head, data);
	if (NULL == t){
		printf("没有找到该数\n");
		return 0;
	}
	t->data = newData;
	return 1;
}

//删除一个节点，用data来查找，如果有几个，删除从头节点开始的第一个
Node *delete(Node* head, ElementType n)
{
	if (NULL == head){
		printf("链表为空,无法删除元素\n");
		return NULL;
	}
	Node *p = select(head, n);
	if (p == head){
		head = head->next;
		free(head);
		return head;
	}
	Node *t = head;
	while (t->next != p){
		t = t->next;
	}
	t->next = p->next;
	free(p);
	return head;
}

//排序函数，采用递增顺序
//此处采用冒泡排序
//O(n^2)
Node *sort(Node *head)
{
	if (NULL == head){
		printf("链表为空\n");
		return NULL;
	}
	Node *tem1 = head;
	Node *tem2 = head;
	//外层循环控制比较次数，此处为链表长度
	for (; tem1->next != NULL; tem1 = tem1->next){
		//内层循环比较相邻两个节点的大小
		for (tem2 = head; tem2->next != NULL; tem2 = tem2->next){
			if (tem2->data > tem2->next->data){
				ElementType temp = tem2->data;
				tem2->data = tem2->next->data;
				tem2->next->data = temp;
			}
		}
	}
	return head;
}
//打印
void print(Node *head)
{
	if (NULL == head){
		printf("no data\n");
		return ;
	}
	Node *t = head;
	while (t){
		printf("%d ", t->data);
		t = t->next;
	}
	printf("\n");
}

int main()
{
	Node *head = NULL;
	print(head);
	
	head = backInster(head, 1);
	head = backInster(head, 2);
	head = backInster(head, 3);
	head = backInster(head, 4);
	head = backInster(head, 5);
	head = backInster(head, 124);

	head = frontInster(head, 1);
	head = frontInster(head, 2);
	head = frontInster(head, 3);
	head = frontInster(head, 4);
	head = frontInster(head, 5);
	head = frontInster(head, 6);
	
	print(head);
	
	Node *temp = NULL;
	Node *t = select(temp, 8);
	if (t == NULL){
		printf("未找到\n");
	}else{
		printf("data = %d\n", t->data);
	}
	int ret = update(head, 5, 8);
	if (ret){
		printf("修改成功\n");
		print(head);
	}else {
		printf("修改失败\n");
	}
	printf("插入99在6后\n");
	head = inster(head, 6, 99);
	print(head);
	head = delete(head, 8);
	print(head);
	
	head = sort(head);
	print(head);
}
