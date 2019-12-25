#include <stdio.h>
#define N 5
void print_stat()
{
	int i, j;
	for (i = 1; i <= N; i++){
		for (j = 0; j < i; j++){
			printf("*");
		}
		printf("\n");
	}
}
int main()
{
	print_stat();
	return 0;
}
