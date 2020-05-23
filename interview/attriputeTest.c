#include <stdio.h>

typedef struct Test
{
	char a;
	int b;
	char c;
} __attribute__ ((packed)) TEST_T;

typedef struct Test2
{
	char a;
	int b;
	char c;
} TEST_T2;
int main(int argc, const char *argv[])
{
	TEST_T t;
	TEST_T2 t2;
	printf("sizeof(TEST_T) = %lu\n", sizeof(t));
	printf("sizeof(TEST_T2) = %lu\n", sizeof(t2));
	return 0;
}
