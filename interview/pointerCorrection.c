#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////////////
//GetMemory并不能为str分配到内存 所以传入strcpy函数的str
//依然是NULL 程序奔溃 Segmentation fault (core dumped)
void GetMemory(char *p)
{
	p = (char *)malloc(100);
}
void Test(void)
{
	char *str = NULL;
	GetMemory(str);
	strcpy(str, "hello, world");
	printf("%s\n", str);
}

///////////////////////////////////////////////////////
//在GetMemory函数中数组p保存了一个字符串，但是在函数退栈
//后指针p被析构 str为NULL
char *GetMemory(void)
{
	char p[] = "hello world";
	return p;
}

void Test(void)
{
	char *str = NULL;
	str = GetMemory();
	printf("%s\n", str);
}
///////////////////////////////////////////////////////
//在这里的GetMemory中为指针p分配到了100的内存，但是并么有
//检测malloc分配内存是否成功 p有可能指向不可用的内存区域
//并且在使用完后没有做free处理 造成内存泄漏
//应该加上 if (p == NULL){
//				perror("malloc");
//				return;
//			}
//并且在printf后main函数退栈前
//		   if (str != NULL){
//				free(str);
//				str = NULL;
//	       }
//
void GetMemory(char **p, int n)
{
	*p = (char *)malloc(n);
}
void Test(void)
{
	char *str = NULL;
	GetMemory(*str, 100);
	strcpy(str, "hello world");
	printf("%s\n", str);
}
///////////////////////////////////////////////////////
int main()
{
	Test();
	return 0;
}
