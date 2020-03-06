#include <stdio.h>
#include <assert.h>

char * myStrcat(char *dst, const char *src)
{
	char *str = dst;
	assert((dst != NULL) && (src != NULL));
	while (*dst){
		dst++;
	}
	while ((*dst++ = *src++) != '\0');
	return str;
}

int main()
{
	char buf1[] = "afsdfs";
	char buf2[43] = "dwjf123";
	char *p = myStrcat(buf2, buf1);
	printf("%s\n", p);

	return 0;
}
