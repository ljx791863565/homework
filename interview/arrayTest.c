#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a[5] = {1, 2, 3, 4, 5};

	int *ptr1 = (int *)&a+1;
	int *ptr2 = (int *)(a+1);
	printf("%x, %x\n", *(ptr1-1), *ptr2);
	return 0;
}
