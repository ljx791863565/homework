#include <stdio.h>
#include <time.h>

using namespace std;
#define LEN 8

//全部系统初始化为0
int Queen[LEN][LEN];

int count = 0;//计数

bool isOk(int a, int b)
{
	int t;
	//判断传入点上方是否有皇后
	for (t=a-1; t>=0; --t){
		if (Queen[t][b] == 1){
			return false;
		}
	}
	//判断点左边是否有皇后
	for (t=b-1; t>=0; --t){
		if (Queen[a][t] == 1){
			return false;
		}
	}
	//左上对角线是否有皇后
	int m, n;
	for (m = a-1, n = b-1; m>=0 && n>=0; --m, --n){
		if (Queen[m][n] == 1){
			return false;
		}
	}
	//右上对角线是否有皇后
	for (m = a-1, n = b+1; m>=0 && n<LEN; --m, ++n){
		if (Queen[m][n] == 1){
			return false;
		}
	}
	
	return true;
}
void print(){
	count++;
	printf("%d\n", count);
	int a, b;
	for (a=0; a<LEN; ++a){
		for (b=0; b<LEN; ++b){
			if (Queen[a][b] == 0){
				printf("* ");
			}else if(Queen[a][b] == 1){
				printf("# ");//皇后‘#’表示
			}
		}
		printf("\n");
	}
	printf("\n");
}

//m 表示行数

//算法时间复杂度为 O（n^2）
void fun(int m){
	if (m == LEN){
		print();
		return ;
	}
	int i;
	for (i=0; i<LEN; ++i){
		if (isOk(m, i)){
			Queen[m][i] = 1;
			fun(m+1);
			Queen[m][i] = 0;
		}
	}
}
int main()
{
	fun(0);//从第0行开始遍历
	return 0;
}
