#include <stdio.h>

char *c[] = {"ENTER", "THE", "NEW", "FIRST"};
char **cp[] = {c+3, c+2, c+1, c};
char ***cpp = cp;

int main()
{
	int i, j;
	for (i = 0; i < 4; i++){
		printf("%s", c[i]);
	}
	printf("\n");
	for (i = 0; i < 4; i++){
		printf("%s", *cp[i]);
	}
	printf("\n");

	printf("%s\n", cpp[2][1] + 2);
}
