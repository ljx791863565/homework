#include <stdio>
#include <time.h>

/*
   clock(), 捕捉程序开始（一般是从main()函数开始）到clock()被调用所消耗时间
   可以用开比较数据量较少的两个算法的快慢
   */
clock_t start, stop;

int main()
{
	double duration; //保存测量函数运行消耗的时间,单位是 s
	start = clock();//开始计时
	/*
	   fun();//被测量函数。
	   */
	stop = clock();//计时结束
	duration = ((double)(stop - start)) / CLOCKS_PER_SEC;

	/*
	   此处为后续 如打印出duration的值
	   */

	return 0;
}
