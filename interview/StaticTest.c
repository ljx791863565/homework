#include <stdio.h>

static int a;
void fun()
{
	static int c = 30;
	c++;
	printf("c = %d\n", c);
}

int main()
{
	a = 20;
	printf("a = %d\n", a);
	a ++ ;
	printf("a = %d\n", a);

	fun();
}
