#include <stdio.h>

int main()
{
	//判断两个float数是否相等时不要用a==b ,要采用绝对值fabs(a, b)小于某个数，如1e-9
	printf("%.7f\n",1e-9);

	//判断一个整数是否为奇数，用x % 2 != 0; 不要用 x % 2 == 1; 因为 x可能是负数


	return 0;
}
