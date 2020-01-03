#include <stdio.h>

int main()
{
	printf("sizeof(int) = %lu\n",sizeof(int));
	printf("sizeof(char) = %lu\n", sizeof(char));
	printf("sizeof(short) = %lu\n", sizeof(short));
	printf("sizeof(long) = %lu\n", sizeof(long));
	printf("sizeof(float) = %lu\n", sizeof(float));
	printf("sizeof(double) = %lu\n", sizeof(double));
	printf("sizeof(long long) = %lu\n", sizeof(long long));

	char *p = NULL;
	int *p2 = NULL;
	printf("sizeof(char *) = %lu\n", sizeof(p));
	printf("sizeof(int *) = %lu\n", sizeof(p2));
	printf("sizeof(size_t) = %zu\n", sizeof(size_t));
	return 0;
}
