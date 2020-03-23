#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 23

// 定义一个数组　大小为Ｎ
// 产生０－N-1　的随机数
// 将产生的随机数存入数组　要求每一个数组元素都不相同
//
void RandArray(int a[], int n)
{
	int i, t;
	for (i = 0; i < n; i++) {
		a[i] = 0;
	}
	n = n -1;
	srand((unsigned)time(NULL));
	for (i = 0; i < n; i++) {
		while (a[t=rand()%n +1] != 0) {
			;
		}
		a[t] = n - i;
	}
}

int main(int argc, const char *argv[])
{
	int array[N];
	RandArray(array, N);

	int i;
	for (i = 0; i < N; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
	return 0;
}
