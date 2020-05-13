#include <stdio.h>

int GetLine(char *str, int maxline)
{
	int ch;
	int i = 0;
	while (maxline >0 && (ch = getchar()) != EOF && ch != '\n') {
		str[i++] = ch;
		if (ch == '\n') {
			str[i++] = ch;
		}
		str[i] = '\0';
		return i;
	}			
}
