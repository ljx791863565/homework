#include <stdio.h>

/*
 *			21 22 ...
 *			20 7  8  9  10
 *			19 6  1  2  11
 *			18 5  4  3  12
 *			17 16 15 14 13
 *		
 *		如上规律的队列排序 假设从1开始 设1点的坐标为(0,0) 向右为正想左为负 向下为正向上为负
 *		即7点坐标为(-1. -1)
 *		要求实现输入任一点坐标 输出该点对应的值
 */
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(a) ((a) > 0 ? (a) : (-a))
#define N 4
int returnNumber(int x, int y)
{
	int t = max(abs(x), abs(y));
	int u = t + t;
	int v = u -1;
	v = v * v + u;
	if (x == -t) {
		v += u + t - y;
	}else if (y == -t) {
		v += 3 * u + x - t;	
	}else if (y == t) {
		v += y - t;
	}else {
		v += y - t;
	}
	return v;
}

int main(int argc, const char *argv[])
{
	int x, y;
	for (y = -N; y <= N; y++) {
		for (x = -N; x <= N; x++) {
			printf("%5d", returnNumber(x,y));
		}
		printf("\n");
	}
	while (scanf("%d%d", &x, &y) == 2) {
		printf("%d\n", returnNumber(x, y));
	}
	return 0;
}
