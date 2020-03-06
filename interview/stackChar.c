#include <stdio.h>
#include <string.h>

char g_buf[100];
void stackPrintf(char *p)
{
	if (p == NULL){
		return ;
	}
	if (*p == '\0'){
		return ;
	}
	stackPrintf(p+1);
//	打印
//	printf("%c", *p);	
	strncat(g_buf, p, 1);
}

//局部变量保存递归结果
void stackBuf(char *p, char *buf)
{
	if (p == NULL || buf == NULL){
		return ;
	}
	if (*p == '\0'){
		return ;
	}
	stackBuf(p+1, buf);

	strncat(buf, p, 1);
}
int main()
{
	char *p = "abcdefg";
	memset(g_buf, 0, sizeof(g_buf));
	stackPrintf(p);//用函数栈递归
	printf("g_buf :%s\n", g_buf);
	{
		char buf[100] = {0};
		stackBuf(p, buf);
		printf("buf :%s\n", buf);
	}
	return 0;
}
