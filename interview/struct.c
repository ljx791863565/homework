#include <stdio.h>

struct A
{
	int iMemory;
	char *cMemory;
};

int main()
{
	struct A a1, a2;
	char c = 'a';

	a1.iMemory = 1;
	a1.cMemory = &c;

	//赋值语句采用变量逐个拷贝 使得a1和a2中的cMemory指针指向同一个内存
	//所以在改变a2中的cMemory指向地址的值即是改变a1中cMemory指向的地址的值
	a2 = a1;

	//a
	printf("%c\n", *(a1.cMemory));
	*(a2.cMemory) = 'b';
	//b
	printf("%c\n", *(a1.cMemory));
	return 0;
}
