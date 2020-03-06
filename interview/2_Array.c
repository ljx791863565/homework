/*
**对应的是 王保名 c语言提高 c提高day03_总结考试作业 第二第三题
**给定一个字符串 “dafsf, dwihr, sad2424, fhiw, fffff, jfeik”
**要求定义一个函数接口实现以逗号分隔形成一个二维数组，并求出二维数组的行数
**二维数组要求在heap上分配空间
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int GetArray(char *str, char ch, char ***p2, int *n)
{
	if (str == NULL || p2 == NULL || n == NULL){
		printf("memory error\n");
		return -1;
	}
	char **p3 = *p2; //保存传入的保存空间的首地址
	char *p = NULL;
	char *tmp = NULL;
	p = str;
	tmp = str;
	int count = 0;

	do {
		p = strchr(p, ch);
		if (p != NULL){
			if (p - tmp > 0){
				strncpy(p3[count], tmp, p-tmp);
				p3[count][p-tmp] = '\0';//补成字符串
				count++;
				tmp = p = p+1;//两个指针同时走到ch字符后重新开始
			}
		}else{
			break;
		}
	}while (*p != '\0');

	*n = count;
	return 0;
}
int GetMemory(char ***p, int row, int len)
{
	int i = 0;
	if (p == NULL){
		printf("get memory error\n");
		return -1;
	}
	char **p2 = NULL;
	p2 = (char **)malloc(sizeof(char *) * row);
	if (p2 == NULL){
		perror("malloc");
		return -1;
	}
	for (i=0; i<row; ++i){
		p2[i] = (char *)malloc(sizeof(char ) *len);
		memset(p2[i], 0, len);
	}
	*p = p2;
}
void Memory_free(char ***p, int row)
{
	if (p == NULL){
		printf("get memory error\n");
		return ;
	}
	int i = 0;
	char **tmp = *p;
	for (i=0; i<row; ++i){
		if (tmp[i] != NULL){
			free(tmp[i]);
			tmp[i] = NULL;
		}
	}
	free(tmp);
	*p = NULL;//让调用函数中的值为NULL， 避免野指针
}
int main()
{
	char *p = "aa32a,bb bbb,cc43 ccc,d#@dd,e.,ette eeeee,ffffffff,gg,";
	char ch = ',';
	int i = 0;
	int len = 0;//行数
	//获取内存分配空间
	char **p2 = NULL;
	int ret = GetMemory(&p2, 10, 30);
	if (ret < 0){
		return -1;
	}
	//解析字符串
	ret = GetArray(p, ch, &p2, &len);
	if (ret < 0){
		return -1;
	}
	for (i=0; i<len; ++i){
		printf("%s\n", p2[i]);
	}

	Memory_free(&p2, len);
	return 0;
}
