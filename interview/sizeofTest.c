#include <stdio.h>

struct test
{
	unsigned int a;
	char b[2];
	double c;
	short d;
};

struct None
{};

int main()
{
	struct test t ;
	printf("sizeof(t) = %lu\n", sizeof(t));
	struct None n ;
	printf("sizeof(t) = %lu\n", sizeof(n));

	return 0;
}
