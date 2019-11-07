#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

//此函数可以用来计算大数乘法，先将两字符串表示的大数相乘，结果以数组保存，再将结果转化为字符串形式
void multiply(const char *a, const char *b)
{
	assert(a != NULL && b != NULL);
	int i, j, la, lb, *s;
	la = strlen(a);
	lb = strlen(b);

	s = (int *)malloc((la+lb) *sizeof(int));
	for (i = 0; i< la+lb; i++){
		s[i] = 0;	//清零
	}

	for (i=0; i<la; i++){
		for (j = 0; j < lb; j++){
			s[i+j+1] += (a[i] - '0') * (b[j] - '0');	//逐位相乘
		}
	}

	for (i=la+lb-1; i>=0; i--){
		if (s[i] >= 10){
			s[i-1] += s[i] / 10;		//进一位+1
			s[i] %= 10;
		}
	}

	char *c = (char *)malloc((la+lb+1)*sizeof(char));
	i = 0;
	//这里跳过乘积结果前面为0的项
	while (s[i] == 0){
		i++;
	}

	for (j = 0; i < la+lb; i++,j++){
		c[j] = s[i] + '0';		//数字恢复成字符串模式
	}
	c[j] = '\0';	//结尾加上字符串结束符号
	
	for (i = 0; i < la+lb; i++){
		std::cout<<c[i];
	}
	std::cout<<std::endl;
	free(s);
	free(c);
}

int main()
{
	char s1[10] = "1234";
	char s2[10] = "1234";

	multiply(s1, s2);

	return 0;
}
