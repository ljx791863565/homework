#include <stdio.h>
#include <ctype.h>
#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;
static int getch()
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

static void ungetch(int c)
{
	if (bufp >= BUFSIZE) {
		printf("ungetch : too many charachters\n");
	}else {
		buf[bufp++] = c;
	}
}
int getint(int *pn)
{
	int c, sign;
	while (isspace(c = getch()))
		;
	if (isdigit(c) && c != EOF && c != '+' && c != '-'){
		ungetch(c);
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-'){
		c = getch();
	}

	for (*pn = 0; isdigit(c); c = getch()){
		*pn = 10 * *pn + (c - '0');
	}
	*pn *= sign;
	if (c != EOF){
		ungetch(c);
	}
	return c;
}
int main()
{
	return 0;
}
