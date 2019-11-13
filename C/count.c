#include <stdio.h>

int main()
{
	int spaceCount = 0, nCount = 0, tCount = 0;
	int c;
	while ((c = getchar()) != EOF){
		if (c == ' '){
			+spaceCount;
		}else if (c == '\n'){
			++nCount;
		}else if (c == '\t'){
			++tCount;
		}

	}
	printf("spaceCount = %d\n", spaceCount);
	printf("nCount = %d\n", nCount);
	printf("tCount = %d\n", tCount);
	return 0;
}
