#include <stdio.h>

int main()
{
	int countN = 0; //'\n'
	int countT = 0; //'\t'
	int countL = 0; //' '

	int ch;
	while ((ch = getchar()) != EOF){
		if (ch == '\n'){
			countN++;
		}else if(ch == '\t'){
			countT++;
		}else if(ch == ' '){
			countL++;
		}
		printf("countN = %d countT = %d countL = %d\n",countN, countT, countL);
	}
	return 0;
}
