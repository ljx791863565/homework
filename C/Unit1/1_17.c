/*
 * 打印长度大于80个字符的所有行
 */

#include <stdio.h>
#define MAXLINE 1000
#define LONGLINE 80

int Getline(char *line, int maxline)
{
	int i, j, ch;
	j = 0;
	for (i = 0; (ch = getchar()) != EOF && ch != '\n'; i++) {
		if (i < maxline -2) {
			line[i] = ch;
			++j;
		}
		if (ch == '\n') {
			line[j] = ch;
			++j;
			++i;
		}
		line[j] = '\0';
	} 
	return i;
}
int main()
{
	int len;
	char line[MAXLINE];

	while ( (len = Getline(line, MAXLINE)) > 0) {
		if (len > LONGLINE) {
			printf("%s", line);
		}
	}
	return 0;
}
	
