#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Stu{
	//char name[20];
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
