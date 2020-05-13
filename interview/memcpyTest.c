#include <stdio.h>
#include <string.h>

/*
 * "abcdefghi" => "hiabcdefg"
 */
#define MAX 100
void loopMove(char *pStr, const int steps)
{
	char temp[MAX] = {0};
	memcpy(temp, pStr + (strlen(pStr)-steps), steps);
	memcpy(pStr+steps, pStr, strlen(pStr)-steps);
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
