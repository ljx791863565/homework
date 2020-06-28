#include "student.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#define MaxSize 100
#define MAX_LANGTH 20
typedef double ElemType;
typedef struct Stack
{
	ElemType *top;
	ElemType *base;
	int Stacksize;
}Stack;
void initStack(Stack *s)
{
	s->base = (ElemType *)malloc(MaxSize *(sizeof(ElemType)));
	if (!s->base)
	{
		exit(0);
	}
	s->top = s->base;
	s->Stacksize = MaxSize;
}
void push(Stack *s, ElemType e)
{
	if (s->top - s->base >= s->Stacksize)
	{
		s->base = (ElemType*)realloc(s->base, s->Stacksize + 10 * (sizeof(ElemType)));
		if (!s->base)
		{
			exit(0);
		}
		s->Stacksize += 10;
	}
	*(s->top) = e;
	s->top++;
}
void pop(Stack *s, ElemType *e)
{
	if (s->top == s->base)
	{
		return;
	}
	*e = *--(s->top);
}

int lenth(Stack s)
{
	int length = s.top - s.base;
	return length;
}

void poland(char *str, char ** strPoland)
{
	
}
int main()
{
	Stack s;
	char c;
	double d, e;
	int i = 0;
	char str[MAX_LANGTH];
	initStack(&s);
	printf("请按后缀（逆波兰表达式）输入，各个数据和运算符之间用空格隔开，输入#停止录入\n");
	scanf("%c", &c);
	while (c != '#')
	{
		while (isdigit(c) || c == '.')//判断是否为数字，若是数字就要转化为double类型
		{
			if (i >= MAX_LANGTH)
			{
				printf("出错，输入单个数据过大\n");
				exit(0);
			}
			str[i++]=c;
			str[i] = '\0';	//为了防止错入		
			scanf("%c", &c);
			if (c == ' ')
			{
				d = atof(str);//将字符串转化为doule类型并压入栈中
				push(&s, d);
				i = 0;
				break;
			}
		}
		switch (c)//若是符号就进行以下工作
		{
			case '+':
				pop(&s, &d);
				pop(&s, &e);
				push(&s, d+e );
				break;
			case '-':
				pop(&s, &d);
				pop(&s, &e);
				push(&s, e - d);
				break;
			case '*':
				pop(&s, &d);
				pop(&s, &e);
				push(&s, e * d);
				break;
			case'/':
				pop(&s, &d);
				pop(&s, &e);
				if (d != 0)
				{
					push(&s, e / d);
				}
				else {
					printf("出错，除数不能为0\n");
					return -1;
				}
				break;
		}
		scanf("%c", &c);
	}
	pop(&s, &d);
	printf("\n最终值为%lf\n", d);
	return 0;
}
