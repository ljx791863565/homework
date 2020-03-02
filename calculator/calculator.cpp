#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <iostream>
using namespace std;

typedef double Elemtype;
typedef char Elemtype_c;

#define STACK_INIT_SIZE 10
#define STACK_INCREMENT 10
#define MAX_BUFFER 10


typedef struct SqStack
{
	Elemtype *base;
	Elemtype *top;
	int stacksize;
}SqStack;

typedef struct SqStack_c
{
	Elemtype_c *base;
	Elemtype_c *top;
	int stacksize;
}SqStack_c;

SqStack *InitStack()
{
	SqStack *s = (SqStack *)malloc(sizeof(SqStack));
	s->base = (Elemtype *)malloc(STACK_INIT_SIZE * sizeof(Elemtype));
	if (!s->base){
		exit(0);
	}
	s->top = s->base;
	s->stacksize = STACK_INIT_SIZE;
	return s;
}

SqStack_c *InitStack_c()
{
	SqStack_c *s = (SqStack_c *)malloc(sizeof(SqStack_c));
	s->base = (Elemtype_c *)malloc(STACK_INIT_SIZE * sizeof(Elemtype_c));
	if (!s->base){
		exit(0);
	}
	s->top = s->base;
	s->stacksize = STACK_INIT_SIZE;
	return s;
}

bool Push(SqStack *s, Elemtype e)
{
	if (s->top - s->base >= s->stacksize){
		s->base = (Elemtype *)realloc(s->base, (s->stacksize + STACK_INCREMENT) * sizeof(Elemtype));
		if (!s->base){
			return false;
		}
		s->top = s->base + s->stacksize;
		s->stacksize = STACK_INCREMENT;
	}
	*(s->top) = e;
	s->top++;
	return true;
}

bool Push_c(SqStack_c *s, Elemtype_c e)
{
	if (s->top - s->base >= s->stacksize){
		s->base = (Elemtype_c *)realloc(s->base, (s->stacksize + STACK_INCREMENT) * sizeof(Elemtype_c));
		if (!s->base){
			return false;
		}
		s->top = s->base + s->stacksize;
		s->stacksize = STACK_INCREMENT;
	}
	*(s->top) = e;
	s->top++;
	return true;
}

bool Pop(SqStack *s, Elemtype &e)
{
	if (s->top == s->base){
		return false;
	}
	--(s->top);
	e = *(s->top);
	return true;
}

bool Pop_c(SqStack_c *s, Elemtype_c &e)
{
	if (s->top == s->base){
		return false;
	}
	--(s->top);
	e = *(s->top);
	return true;
}

Elemtype GrtTop(SqStack *s)
{
	Elemtype e;
	if (s->top == s->base){
		exit(0);
	} 
	e = *(s->top - 1);
	return e;
}

Elemtype_c GrtTop(SqStack_c *s)
{
	Elemtype_c e;
	if (s->top == s->base){
		exit(0);
	}
	e = *(s->top - 1);
	return e;
}

void ClearStack(SqStack *s)
{
	s->top = s->base;
}

void ClearStack_c(SqStack_c *s)
{
	s->top = s->base;
}

void DestroyStack(SqStack *s)
{
	int len = s->stacksize;
	if (s->base){
		free(s->base);
	}
	s->base = s->top = NULL;
	s->stacksize = 0;
	free(s);
	s = NULL;
}

void DestroyStack_c(SqStack_c *s)
{
	int len = s->stacksize;
	if (s->base){
		free(s->base);
	}
	s->base = s->top = NULL;
	s->stacksize = 0;
	free(s);
	s = NULL;
}

int StackLen(SqStack *s)
{
	return s->top - s->base;
}

int StackLen_c(SqStack_c *s)
{
	return s->top - s->base;
}

void DisplayStack(SqStack *s)
{
	int length;
	Elemtype *initbase = s->base;
	length = s->top - s->base;
	for (int i = 0; i < length; i++){
		cout << *(s->base) << " ";
		s->base++;
	}
	cout << endl;
	s->base = initbase;
}

void DisplayStack_c(SqStack_c *s)
{
	int length;
	Elemtype_c *initbase = s->base;
	length = s->top - s->base;
	for (int i = 0; i < length; i++){
		cout << *(s->base) << " ";
		s->base++;
	}
	cout << endl;
	s->base = initbase;
}

int main()
{
	SqStack_c *cs = InitStack_c();
	char c, e;
	char vect[50];
	int j = 0;
	printf("请输入算术表达式,以#结束\n");
	scanf("%c", &c);
	printf("转换后的逆波兰表达式为: \n");
	while (c != '#'){
		while (isdigit(c) || c == '.'){
			printf("%c", c);
			vect[j++] = c;
			vect[j] = '\0';
			scanf("%c", &c);
			if (!isdigit(c) || c == '.'){
				printf(" ");
				vect[j++] = ' ';
				vect[j] = '\0';
			}
		}
		if (c == ')'){
			Pop_c(cs, e);
			while (e != '('){
				printf("%c", e);
				vect[j++] = e;
				vect[j++] = ' ';
				vect[j] = '\0';
				Pop_c(cs, e);
			}
		}else if (c == '+' || c == '-'){
			if (!StackLen_c(cs)){
				Push_c(cs, c);
			}else {
				do{
					Pop_c(cs, e);
					if (e == '('){
						Push_c(cs, e);
					}else {
						printf("%c", e);
						vect[j++] = e;
						vect[j++] = ' ';
						vect[j] = '\0';
					}
				}while (e != '(' && StackLen_c(cs));
				Push_c(cs, c);
			}
		}else if (c == '*' || c == '/' || c == '('){
			Push_c(cs, c);
		}else if (c == '#'){
			break;
		}else{
			printf("表达式错误。。。\n");
			break;
		}
		scanf("%c", &c);
	}
	while (StackLen_c(cs)){
		Pop_c(cs, e);
		printf("%c", e);
		vect[j++] = e;
		vect[j++] = ' ';
		vect[j] = '\0';
	}
	vect[j-1] = '#';


	SqStack *s = InitStack();
	double d, f;
	char g;
	int i = 0, k = 0;
	char str[MAX_BUFFER];
	g = vect[k++];

	while (g != '#'){
		while (isdigit(g) || g == '.'){
			str[i++] = g;
			str[i] = '\0';
			if (i > MAX_BUFFER){
				printf("出错：输入单个数据过大\n");
				return -1;
			}
			g = vect[k++];
			if (g == ' '){
				d = atof(str);
				Push(s, d);
				i = 0;
				break;
			}
		}
		switch (g)
		{
			case '+':
				Pop(s, f);
				Pop(s, d);
				Push(s, d + f);
				break;
			case '-':
				Pop(s, f);
				Pop(s, d);
				Push(s, d - f);
				break;
			case '*':
				Pop(s, f);
				Pop(s, d);
				Push(s, d * f);
				break;
			case '/':
				Pop(s, f);
				Pop(s, d);
				Push(s, d / f);
				break;
		}
		g = vect[k++];
	}
	Pop(s, d);
	printf("最终结果: %f\n", d);
	return 0;
}
