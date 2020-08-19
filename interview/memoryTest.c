#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Stu{
	//char name[20];
	//指针只指向地址而不分配内存 所以要使用的话要先分配到内存
	//
	char *name;
	int age;
}stu, *pstu;

int main(void)
{
	pstu = (struct Stu*)malloc(sizeof(struct Stu));
	if (pstu == NULL) {
		perror("malloc");
		return -1;
	}
	pstu->name = (char *)malloc(sizeof(char) *20);
	if (pstu->name == NULL) {
		perror("malloc");
		return -1;
	}

	strcpy(pstu->name, "hello");
	pstu->age = 20;

	printf("%s %d\n",pstu->name, pstu->age);
	free(pstu->name);
	free(pstu);
	return 0;
}
