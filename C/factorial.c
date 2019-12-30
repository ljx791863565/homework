#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n = 1;
	scanf("%d",&n);
	int a[20000];
	int carry;
	int i, j;
	int digit = 1;
	a[0] = 1;
	int temp = 1;
//	a[0] * a[1] *...*a[n]
	for(i = 2; i <= n; ++i)
	{
		carry = 0;	
		for(j = 1; j <= digit; ++j) //digit 值的位数
		{
			temp = a[j-1] * i + carry;
			a[j-1] = temp % 10;	//值的尾数
			carry = temp / 10; //进位
		}
		while(carry)
		{
			a[++digit-1] = carry % 10;
			carry /= 10;
		}
	}
	for(i = digit; i >=1; --i)
	{
		printf("%d",a[i-1]);
	}
	printf("\n");
	return 0;
}
