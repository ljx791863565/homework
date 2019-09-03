
#include <stdio.h>

int main()
{
	int N;
	int sum = 0;

	printf("please put in a number,,,\n");
	scanf("%d", &N);

	for (int i = 1; i <= N; i++){
		sum += i;
	}

	printf("1 add to %d is %d\n", N, sum);


	return 0;
}
