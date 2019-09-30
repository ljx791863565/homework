#include <stdio.h>

int main()
{
	int i = 0;
	int a = i++;
	int b = ++a;
	int c = a + b;

	printf("a = %d b = %d c = %d a?b:c = %d\n", a, b, c, a?b:c);

	return 0;
}
