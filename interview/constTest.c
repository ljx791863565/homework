#include <stdio.h>


/*
 * const只是对类型起到修饰作用 限定被修饰的对象不可以被修改的属性 
 * int *p; 
 * 一个指针p 不加const修饰时 它的值和指向的值都是可以被修改的
 * int const *p;
 * const int *p;
 * 这两个都是一样的意思，在const的判定中，自动先忽略类型 const离哪个近就修饰哪个
 * 此处const都是修饰*p 即指针p的值(指向)是可以修改的 指针p指向的值(*p)是不可以修改的
 *
 * int * const p;
 * 这里忽略类型(int*) 即const修饰的是p 则p的值(指向)不可以修改 *p可以被修改
 *
 * const int *const p; p的值和指向的值都不可以被修改
 *
 */
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
