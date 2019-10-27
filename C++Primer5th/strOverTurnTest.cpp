#include <iostream>
#include <string.h>
#include <stdio.h>
void Overturn(char *str)
{

	char *p = str;
	char *pend = str + strlen(str) -1;
	while (p < pend){
		char ch = *p;
		*p = *pend;
		*pend = ch;
		p++;
		pend--;
	}
	
	
}

int main()
{
	char str[20] = "117930";
	Overturn(str);
	printf("%s\n", str);

	return 0;
}
