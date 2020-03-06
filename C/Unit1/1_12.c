#include <stdio.h>

#define IN 1
#define OUT 0

int main()
{
	int c, state;
	state = OUT;//初始时不在单词内
	while ((c= getchar()) != EOF){
		if (c == ' ' || c == '\t' || c == '\n'){
			if (state == IN){
				putchar('\n');
				state = OUT;
			}
		}else if (state == OUT){
			state = IN;
			putchar(c);
		}else{
			putchar(c);
		}
	}
	return 0;
}
