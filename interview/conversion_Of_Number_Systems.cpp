#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <iostream>

long long binToDec(char *binstr)
{
	assert(binstr != NULL);
	int len = strlen(binstr);
	int i = 0;
	for (i = 0; i < len; i++) {
		assert(binstr[i] == '0' || binstr[i] == '1');
	}

	long long num = 0;
	for (i = 0; i < len; i++) {
		//此处要注意字符串中字符 '0' '1' 转换成数字 0 1
		num += (binstr[i] - '0') * pow(2, len-i-1);
	}
	return num;
}

void decToBin(long long num, char *str)
{
	int i = 0;
	if (num < 0) {
		num = LLONG_MAX  + num;
	}
	while (num != 0) {
		str[i] = (num % 2) + '0';
		num /= 2;
		i++;
	}
	int  j = 0;
	int k = i;	//字符串长度
	i = i-1;	
	//逆序
	while (j < i) {
		char temp = str[j];
		str[j] = str[i];
		str[i] = temp;
		j++;
		i--;
	}
	str[k] = '\0';	//字符串结尾
}

long long octToDec(char *octstr)
{
	assert(octstr != NULL);
	int len = strlen(octstr);
	int i = 0;
	for (i = 0; i < len; i++) {
		assert(octstr[i] == '0' || octstr[i] == '1' || octstr[i] == '2' || octstr[i] == '3' 
			|| octstr[i] == '4' || octstr[i] == '5' || octstr[i] == '6' || octstr[i] == '7');
	}

	long long num = 0;
	for (i = 0; i < len; i++) {
		num += (octstr[i] - '0') * pow(8, len-i-1);
	}
	return num;
}

void decToOct(long long num, char *str)
{
	int i = 0;
	if (num < 0) {
		num = LLONG_MAX + num;
	}
	while (num != 0) {
		str[i] = (num % 8) + '0';
		num /= 8;
		i++;
	}
	int  j = 0;
	int k = i;	//字符串长度
	i = i-1;	
	//逆序
	while (j < i) {
		char temp = str[j];
		str[j] = str[i];
		str[i] = temp;
		j++;
		i--;
	}
	str[k] = '\0';	//字符串结尾
}

long long hexToDec(char *hexstr)
{
	assert(hexstr != NULL);
	int len = strlen(hexstr);
	int i = 0;
	for (i = 0; i < len; i++) {
		assert(hexstr[i] == '0' || hexstr[i] == '1' || hexstr[i] == '2' || hexstr[i] == '3' 
			|| hexstr[i] == '4' || hexstr[i] == '5' || hexstr[i] == '6' || hexstr[i] == '7'
			|| hexstr[i] == '4' || hexstr[i] == '5' || hexstr[i] == '6' || hexstr[i] == '7'
			|| hexstr[i] == '8' || hexstr[i] == '9' || hexstr[i] == 'A' || hexstr[i] == 'B'
			|| hexstr[i] == 'C' || hexstr[i] == 'D' || hexstr[i] == 'E' || hexstr[i] == 'F'
			|| hexstr[i] == 'a' || hexstr[i] == 'b' || hexstr[i] == 'c' || hexstr[i] == 'd'
			|| hexstr[i] == 'e' || hexstr[i] == 'f' );
	}
	long long num = 0;
	for (i = 0; i < len; i++) {
		switch (hexstr[i])
		{
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
				{
					num += (hexstr[i] - '0') * pow(16, len-i-1);
					break;
				}
			case 'A' :
			case 'a' :
				{
					num += 10 * pow(16, len-i-1);
					break;
				}
			case 'B' :
			case 'b' :
				{
					num += 11 * pow(16, len-i-1);
					break;
				}
			case 'C' :
			case 'c' :
				{
					num += 12 * pow(16, len-i-1);
					break;
				}
			case 'D' :
			case 'd' :
				{
					num += 13 * pow(16, len-i-1);
					break;
				}
			case 'E' :
			case 'e' :
				{
					num += 14 * pow(16, len-i-1);
					break;
				}
			case 'F' :
			case 'f' :
				{
					num += 15 * pow(16, len-i-1);
					break;
				}
		}
	}
	return num;
}

void decToHex(long long num, char *str)
{
	int i = 0;
	int m = 0;
	if (num < 0) {
		num = LLONG_MAX + num;
	}
	while (num != 0) {
		m = (num % 16);
		switch (m)
		{
			case 0 :
			case 1 :
			case 2 :
			case 3 :
			case 4 :
			case 5 :
			case 6 :
			case 7 :
			case 8 :
			case 9 :
			{
				str[i] = m + '0';
				break;
			}
			case 10 :
				str[i] = 'A';
				break;
			case 11 :
				str[i] = 'B';
				break;
			case 12 :
				str[i] = 'C';
				break;
			case 13 :
				str[i] = 'D';
				break;
			case 14 :
				str[i] = 'E';
				break;
			case 15 :
				str[i] = 'F';
				break;
			default :
				break;
		}
		num /= 16;
		i++;
	}
	int  j = 0;
	int k = i;	//字符串长度
	i = i-1;	
	//逆序
	while (j < i) {
		char temp = str[j];
		str[j] = str[i];
		str[i] = temp;
		j++;
		i--;
	}
	str[k] = '\0';	//字符串结尾
}

//目前只能支持正数  负数的情况我还没想好怎么转换。。。
//支持了负数 但只能用64位的 long long 类型表示　
//即signed long long 类型的-1 和 signed long long LLONG_MAX + 1 的二进制表示是一样的
//如果要用不同类型表示 必须重载函数使得不同的数据类型调用不同函数
int main(int argc, const char *argv[])
{
//	long long num = 9223372036854775806;
	long long num = -12;
	printf("%d\n", num);	
	char *p = (char*)malloc(sizeof(char)*65);
	if (p == NULL) {
		perror("malloc");
		return -1;
	}
	decToBin(num, p);
	printf("%s\n", p);
	long long new_num = binToDec(p);
	printf("%lld\n", new_num);

	decToOct(num, p);
	printf("0%s\n", p);
	new_num = octToDec(p);
	printf("%lld\n", new_num);
	
	decToHex(num, p);
	printf("0x%s\n", p);
	new_num = hexToDec(p);
	printf("%lld\n", new_num);

	free(p);
	p = NULL;
	return 0;
}
