#include <stdio.h>

int main()
{
	int a = 10;
//	const int* p = &a;	//指向const int 的指针，所以不可以改变指针指向的值，即*p = 20;是错误的
//	*p = 20;
	a = 20;				//但是a可以改变
	int* const q = &a;	//指向int的const指针，所以可以改变指针指向的内存的值，不可以改变指针本身的值，即q = &b;是错误的
	*q = 30;
	a = 40;
//	printf("*p = %d\n", *p);
	printf("*q = %d\n", *q);
	printf("a = %d\n", a);
}
