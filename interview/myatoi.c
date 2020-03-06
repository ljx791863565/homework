#include <stdio.h>
#include <assert.h>
#include <limits.h>
/*
	int atoi(const char *pstr);
	将字符串转换成整型数的函数
	pstr为NULL		中断
		为空字符	返回0
		为正负值	返回对应的正负值
		溢出		返回0
		异常字符	将异常字符作为'\0'处理		
   */

int myatoi(const char *str)
{
	int flag = 1;
	long long ret = 0;
	
	assert(str);

	if ('\0' == *str){
		return 0;		
	}
	while (*str && isspace(*str)){
		str++;
	}
	if ('-' == *str){
		flag = -1;
	}
	if (('-' == *str) || ('+' == *str)){
		str++;
	}

	while ('\0' != *str){
		if ((*str >= '0') && (*str <= '9')){
			ret = ret * 10 + flag *(*str - '0');
			if (ret > INT_MAX){
				ret = 0;	//溢出
				break;
			}
		}else{
			break;
		}

		str++;
	}

	return (int)ret;
}
int main()
{
	char *str1 = NULL;
	char str2[] = {0};
	char str3[] = "-12345";
	char str4[] = "abc";
	char str5[] = "123456789876543212";
	char str6[] = "123abc";

	int ret;
//	ret = myatoi(str1);
//	printf("ret = %d\n", ret);

//	ret = myatoi(str2);
//	printf("ret = %d\n", ret);

	ret = myatoi(str3);
	printf("ret = %d\n", ret);

	ret = myatoi(str4);
	printf("ret = %d\n", ret);

	ret = myatoi(str5);
	printf("ret = %d\n", ret);

	ret = myatoi(str6);
	printf("ret = %d\n", ret);

	return 0;
} 
