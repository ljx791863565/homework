#include <iostream>

int *cal(int *input, int n)
{
	int *result = new int[n];
	int i;
	result[0]  =1;
	for (i = 1; i<n; i++){
		result[i] = result[i-1] * input[i-1];	//此时result中保存的是input[i]左边元素的乘积
	}
	result[0] = input[n-1];
	for (i = n-2; i >0; --i){
		result[i] = result[i] * result[0];
		result[0] = result[0] * input[i];
	}
	return result;
}

int main()
{
	int input[4] = {1, 2, 3, 4};
	int *p = cal(input, 4);
	int i ;
	for (i = 0; i < 4; i++)
	{
		std::cout << p[i] << std::endl;
	}

	return 0;
}
