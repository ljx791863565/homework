#include <stdio.h>

#define N 9

int main(int argc, const char *argv[])
{
	int i, j;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= i; j++) {
			printf("%d * %d = %d ", i, j, i*j);
		}
		printf("\n");
	}
	return 0;
}
