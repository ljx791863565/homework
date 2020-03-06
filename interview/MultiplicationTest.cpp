/*
   小数点后没有位数限制的高精度乘法算法
   思路：
   先把小数变成整数，记录位数，相乘后还原成小数
   */

#include <iostream>
#include <string.h>

using namespace std;
#define MAX 10000
struct Node
{
	int data;
	Node *next;
};

void output(Node *head)
{
	if (!head->next && !head->data)
		return ;
	output(head->next);
	cout << head->data;
}

void Mul(char *a, char *b, int pos)
{
	char *ap = a;
	char *bp = b;
	Node *head = 0;
	Node *p, *q = head, *p1;
	int temp = 0, temp1, bbit;

	while (*bp){
		p = q->next;
		p1 = q;
		bbit = *bp - 48;
		while (*ap || temp){
			if (!p){
				p = new Node;
				p->data = 0;
				p->next = NULL;
				p1->next = p;
			}
			if (*ap == 0)
				temp1 = temp;
			else{
				temp1 = (p1->data) + (*ap - 48)*bbit + temp;
				ap ++;
			}
			p1->data = temp1 % 10;
			temp = temp1 / 10;
			p1 = p;
			p = p->next;
		}
		ap = a;
		bp ++;
		q = q->next;
	}
	p = head;
	output(p);
	cout << endl;
	
	while (head){
		p = head->next;
		delete head;
		head = p;
	}
}

int main()
{
	cout << "请输入两个数"  << endl;
	char test1[MAX];
	char test2[MAX];
	char *p;
	int pos = 0;
	cin.getline(test1, MAX, '\n');
	cin.getline(test2, MAX, '\n');
	if (p = strchr(test1, '.')){
		pos += strlen(test1) - (p - test1) - 1;
		do {
			p ++;
			*(p - 1) = *p;
		}while (*p);
	}
	if (p = strchr(test2, '.')){
		pos += strlen(test2) - (p - test2) - 1;
		do {
			p++;
			*(p - 1) = *p;
		}while (*p);
	}
	Mul(strrev(test1), strrev(test2), pos);

	return 0;
}
