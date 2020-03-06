#include <stdio.h>
#include <assert.h>
char * myStrcpy(char *dst, const char *src)
{
	assert(dst != NULL && src != NULL);
	//不要轻易改变形参的值
	char *str = dst;
	while ((*dst++ = *src++) != '\0');//把 '\0'复制到dst的内存地址后while循环退出
	return str;
}


int main()
{
	char buf1[] = "abdsfkjws";
	char buf2[20];
	char *p = myStrcpy(buf2, buf1);
	printf("%s\n", p);

	return 0;
}
