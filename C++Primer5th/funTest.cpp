#include <iostream>
//此题用来考察一个数二进制的1的数量
int fun(int x)
{
	int countx = 0;
	while (x){
		countx++;
		x = x&(x-1);	//每次执行一次此行时，会将二进制下的x最右边的一个1，循环在x=0时结束，此时countx的数值将是x含有1的数量	
	}
	return countx;
}

int main()
{
	std::cout<<fun(20) << std::endl;

	return 0;
}
