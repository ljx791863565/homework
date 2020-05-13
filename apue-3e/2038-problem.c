#include <stdio.h>
#include <limits.h>
#include <time.h>

/*
 * 在32位UNIX(unix-like)系统中 日历存放表示为一个32位有符号的整数类型
 * 即其最大值为 2^31
 */
int main(int argc, const char *argv[])
{
//	time_t seconds = 2147483647; 
	time_t seconds = INT_MAX; 
	struct tm *p;
	p = gmtime(&seconds);
	printf("%s", asctime(p));

	return 0;
}
