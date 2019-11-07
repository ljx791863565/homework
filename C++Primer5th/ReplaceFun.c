#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//将一个字符串中的所有空格替换成%20
//hello world my friend
//hello%20world%20my%20friend
//思路是先计算字符串中空格的个数，然后重新确定新的字符串的大小，从后向前替换

void ReplaceFun(char *str, int n)
{
	int spaceCount = 0;
	int newLen = 0;

	int i = 0;
	for (i = 0; i < n; i++){
		if (str[i] == ' '){
			spaceCount++;
		}
	}
	newLen = n + spaceCount * 2;	//本身有一个空格的空间，所以只需要再添加2个字符大小就可以替换了
	str[newLen] = '\0';

	//从后向前遍历字符串，遇到空格就替换成%20
	for (i = n-1; i >= 0; i--){
		if (str[i] == ' '){
			str[newLen -1] = '0';
			str[newLen -2] = '2';
			str[newLen -3] = '%';
			newLen -= 3;
		}else {
			str[newLen-1] = str[i];
			newLen -= 1;
		}
	}
}
int main()
{
	char str[50] = {"hello world my friend"};

	printf("%s\n", str);
	ReplaceFun(str, strlen(str));

	printf("%s\n", str);
	return 0;
}
