#include <stdio.h>
//二维数组作为函数参数的传参
void showInt(int (*p)[4], int n, int m)
{
	int i, j;
	for (i=0; i<n; ++i){
		for (j=0; j<m; ++j){
			printf("%d ", p[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	int a[3][4] ={11, 42, 3, 54, 5,26, 77, 18, 9, 35, 31, 62};
	showInt(a, 3, 4);

	return 0;
}
