#include <stdio.h>

static int j;

void fun1()
{
	static int i = 0;
	i++;
}

void fun2()
{
	j = 0;
	j++;
}

int main(int argc, const char *argv[])
{
	int k = 0;
	for (k = 0; k < 10; k++) {
		fun1();
		fun2();
	}

	printf("j = %d\n", j);

	return 0;
}
