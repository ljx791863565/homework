#include <stdio.h>
#include <assert.h>
#include <string.h>
/*
 * reverse() 将一个字符串反转
 */

char* reverse(char *str)
{
	assert(str != NULL);
	int i, j;
	for(i = 0, j = strlen(str)-1; i < j; i++, j--) {
		str[i] ^= str[j];
		str[j] ^= str[i];
		str[i] ^= str[j];
	}

	return str;
}
 
 
