#include <iostream>
#include <stdio.h>
#include <string.h>
int main()
{
	char str[1000] = "";
	int nLen1 = sizeof(str);
	int nLen2 = strlen(str);

	strcpy(str, "abc");
	int nLen3 = sizeof(str);
	int nLen4 = strlen(str);

	int nTemp[100];
	int *p = nTemp;
	int nLen5 = sizeof(p);

	char result[200] = "";
	sprintf(result, "%d, %d, %d, %d, %d", nLen1, nLen2, nLen3, nLen4, nLen5);
	printf("%s\n", result);

	return 0;
}
