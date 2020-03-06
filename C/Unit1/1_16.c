#include <stdio.h>

#define MAXLEN 1000

int getLine(char *s, int lim)
{
	int c, i, j=0;
	for (i=0; (c = getchar()) != EOF && c != '\n'; ++i){
		if (i < lim -2){
			s[j] = c;
			++j
		}
	
		if (c == '\n'){
			s[j] = c;
			++i;
			++j;
		}
	s[i] = '\0';
	return i;
}

void copy(char *to, char *from)
{
	int i = 0;
	while ((to[i] = from[i]) != '\0'){
		++i;
	}
}

int main()
{
	int len;
	int max;
	char line[MAXLEN];
	char longest[MAXLEN];

	max = 0;
	while ((len = getLine(line, MAXLEN)) > 0){
		printf("%d, %s",len, line);
		if (len > max){
			max = len;
			copy(longest, line);
		}
		if (max > 0){
			printf("%s",longest);
		}
	}
	return 0;
}
