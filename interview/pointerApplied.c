/*
**1. 有一个首尾都有不定空格的字符串，实现空格成为一个新的字符串
**2. 有一个字符串要求把偶数位字符连接成一个新的字符串，奇数位连接成一个新的字符串
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
int GetOddStrEvenStr(char *str, char *oddStr, char *evenStr)
{
	if (str == NULL || oddStr == NULL || evenStr == NULL){
		printf("error : memorry NULL\n");
		return -1;
	}
	int i = 0;
	char *p = str;
	char *oddTmp = oddStr, *evenTmp = evenStr;
	for (i=0; i<strlen(p); ++i){
		if (i % 2){
			*evenStr++ = p[i];
		}else {
			*oddTmp++ = p[i];
		}
	}

	return 0;
}

int TrimSpace(char *inSpace, char *outSpace)
{
	if (inSpace == NULL || outSpace == NULL){
		printf("error: inspace == NULL || outSpace == NULL\n");
		return -1;
	}
	char *p = inSpace;
	char *tmpbuf = outSpace;
//这个是我自己实现的，不太专业化，很幼稚
/* 
	int flag = 0;
	while (*p != '\0'){
		char ch = *p;
		p++;
		if ((*p != ' ' && ch == ' ') || flag == 1){
			flag = 1;
			*tmpbuf++ = *p;
			if (*p == ' ' && ch != ' '){
				flag = 0;
			}
		}
	}
*/
//两头堵模型
	int i = 0, j = strlen(p) - 1;
	//从头开始到第一个非空字符
	while (isspace(p[i]) && p[i] != '\0'){
		i++;
	}
	//从尾开始到第一个非空字符
	while (isspace(p[j] && p[j] != '\0')){
		j--;
	}
	//字符串的长度是 j-i+1
	strncpy(tmpbuf, p+i, j-i+1);
	
	return 0;
}


int main()
{
	char *p = "    dwhqjpqi141424         ";
	char buf[50];

	int ret = TrimSpace(p, buf);
	if (ret < 0){
		printf("fun TrimSpace() error: %d\n", ret);
		return ret;
	}
	printf("%s\n", p);
	printf("%s\n", buf);

	char *p2 = "jfk232rkr2323n23k23";
	char odd[30] = {0};
	char even[30] = {0};
	ret = GetOddStrEvenStr(p2, odd, even);
	if (ret < 0){
		printf("GetOddStrEvenStr() error: %d\n", ret);
		return ret;
	}
	printf("str :%s\n", p2);
	printf("odd :%s\n", odd);
	printf("even :%s\n", even);

	
	return 0;
}
