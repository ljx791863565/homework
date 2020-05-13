/*
 * 删除每个输入行尾的空格和制表符 并删除完全是空格的行
 */
#include <stdio.h>
#define MAXLINE 1000
extern int Getline(char *line, int maxline);
int Remove(char *str)
{
	int i = 0;
	while ( str[i] != '\n') {
		++i;
	}
	--i;
	while ( i >= 0 && (str[i] == ' ' || str[i] == '\t')) {
		--i;
	}
	if (i >= 0) {
		++i;
		str[i] = '\n';
		++i;
		str[i] = '\0';
	}
	return i;
}
int main(int argc, const char *argv[])
{
	char line[MAXLINE];
	while ( Getline(line, MAXLINE) > 0) {
		if (Remove(line) > 0) {
			printf("%s\n", line);
		}
	}
	return 0;
}
