#include <stdio.h>

struct test
{
	unsigned int a;
	char b[2];
	double c;
	short d;
};

int main()
{
	struct test t ;
	printf("sizeof(t) = %lu\n", sizeof(t));

	return 0;
}
