#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	char *str = (char*)malloc(100);
	strcpy(str, "hello");
	free(str);
	if (str != NULL){
		strcpy(str, "world");
		printf("%s\n",str);
	}

	return 0;
}
