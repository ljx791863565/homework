#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

/*
 * double strtod(const char *nptr,char **endptr);
 * strtod()会扫描参数nptr字符串，跳过前面的空格字符，直到遇上数字或正负符号才开始做转换
 * 到出现非数字或字符串结束时('\0')才结束转换，并将结果返回
 */
int main1()
{
	char time[] = "365.24 29.53";
	char *end;
	double d1, d2;
	d1 = strtod(time, &end);
	d2 = strtod(end, NULL);

	printf("the moon around earth %.2f a year\n", d1/d2);
	return 0;
}

int main2()
{
	//将字符串按照进制转换成长整形数
	char a[] = "1000000000";
	char b[] = "1000000000";
	char c[] = "ffff";
	printf("a = %ld\n", strtol(a, NULL, 2));
	printf("b = %ld\n", strtol(b, NULL, 10));
	printf("c = %ld\n", strtol(c, NULL, 16));
}

int main3()
{
	int a = 51;
	char b;
	printf("a ascii = %d(%c)\n", a, a);

	//将整型数转换成合法的ASC||值
	b = toascii(a);
	printf("b ascii = %d(%c)\n", b, b);

	return 0;
}

int main4()
{
	time_t timep;
	time(&timep);
//	printf("%s", asctime(gmtime(&timep)));
	printf("%s", ctime(&timep));

	return 0;
}
int main5()
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	printf("tv_sec:%ld\n", tv.tv_sec);
	printf("tv_usec:%ld\n", tv.tv_usec);
	printf("tz_minuteswest:%d\n", tz.tz_minuteswest);
	printf("tz_dsttime:%d\n", tz.tz_dsttime);
}

int main()
{
	main1();
}
