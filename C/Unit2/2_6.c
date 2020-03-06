/*
   setbits(int x, int p, int n, int y);
   将x从第p位（二进制）开始的n位设置为y最右边的n个值
   x = 256	0000 0000 0000 0000 0000 0000 1111 1111
   y = 11	0000 0000 0000 0000 0000 0000 0000 1011
   p = 23
   n = 2
   结果得到
   x = 1024	0000 0000 0000 0000 0000 0011 1111 1111  
   */

#include <stdio.h>

unsigned setbits(int x, int p, int n, int y)
{
	return x & ~(~(~0 << n) << (p+1-n)) | (y & ~(~0 << n)) < (p+1 -n);	
}

int main()
{
	int x = 256, y = 11;
	int p = 23, n = 2;
	x = setbits(x, p, n, y);

	printf("x = %d\n", x);
	return 0;
}

