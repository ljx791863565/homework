#include <stdio.h>
#include <stdlib.h>

//用_onexit注册一个函数 使得它可以在main函数退栈后执行
int atexit(void (*function)(void));
void fun1(void);
void fun2(void);
void fun3(void);
void fun4(void);

int main(int argc, const char *argv[])
{
	atexit(fun1);
	atexit(fun2);
	atexit(fun3);
	atexit(fun4);
	printf("this is executed first\n");
	return 0;
}

void fun1(void)
{
	printf("next\n");
}
void fun2(void)
{
	printf("executed ");
}
void fun3(void)
{
	printf("is ");
}
void fun4(void)
{
	printf("this ");
}
