#include <stdio.h>
#include <stdlib.h>
int main()
{
	int *p = (int *)malloc(sizeof(int) * 4);
	int i = 0;
	for (i = 0; i<4; i++){
		p[i] = i;
	}

	free(p);
	for (i = 0; i < 4; i++){
		printf("%d ", p[i]);
	}
	printf("\n");

	return 0;
}
