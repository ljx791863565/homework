#include <stdio.h>

/*replace string blanks with a single blank*/
int main()
{
	int c;
	int cp = 'a';
	while ((c = getchar()) != EOF){
		if (c != ' '){
			putchar(c);
		}
		if (c == ' '){
			if (cp != ' '){
				putchar(c);
			}
		}
		cp = c;
	}
	return 0;
}
