#include <stdio.h>

union {
	int a;
	char b[2];
}u, *p;

int main()
{
	p = &u;
	p->b[0] = 0x39;
	p->b[1] = 0x38;

	printf("%x\n", *p);
}
