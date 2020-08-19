#include <stdio.h>
#include <string.h>

/*
 * "abcdefghi" => "hiabcdefg"
 */
#define MAX 100
void loopMove(char *pStr, const int steps)
{
	char temp[MAX] = {0};
	//temp 保存最后steps个字节
	memcpy(temp, pStr + (strlen(pStr)-steps), steps);
	//在内存中空steps个字节开始复制从开始到len-steps个字节的字符
	memcpy(pStr+steps, pStr, strlen(pStr)-steps);
	//在空出的steps个字节的地址中复制temp中的字节
	memcpy(pStr, temp, steps);
}
int main()
{
	char str[20] = "abcdefghi";
	printf("%s\n", str);
	loopMove(str, 2);
	printf("%s\n", str);

	return 0;
}
