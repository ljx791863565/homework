#include <stdio.h>
#include <stdlib.h>
int main()
{
	int  a = 2;	//0000 0010
	int  b = 3; //0000 0011

	int c = a & b;
	int d = a | b;
	int e = a ^ b;
	int f = ~a;

	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("c = %d\n", c);
	printf("d = %d\n", d);
	printf("e = %d\n", e);
	printf("f = %d\n", f);
	
	/*
	   a	0000 0010
	   b	0000 0011
	   a&b	0000 0010	a与b 与操作，当两个操作数都是1时为1，其余为0
	   a|b	0000 0011	a或b 或操作，当连个操作数都是0时为0，其余为1
	   a^b  0000 0001   a亦或b， 亦或操作，当两个操作数相同时为0， 不同时为1
	   ~a   1111 1101	取反a， 取反操作，只有一个操作数，所有的1变成0，所有的0变成1
	   */
	return 0;
}
