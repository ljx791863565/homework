#include <stdio.h>

//欧几里得算法
//求解最大公约数
int gcd(int p, int q)
{
	if (q == 0){
		return p;
	}
	int r = p % q;
	return gcd(q, r);
}
int main()
{
	int p, q;
	printf("请输入两个整数\n");
	scanf("%d %d", &p, &q);

	int num = gcd(p, q);
	printf("%d 和 %d 的最大公约数是：%d\n" ,p, q, num);
}
