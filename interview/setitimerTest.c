#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>

static int count;

void set_time()
{
	struct itimerval itv;
	itv.it_value.tv_sec = 3;	//计时器启动的间歇
	itv.it_value.tv_usec = 0;	

	itv.it_interval.tv_sec = 1;	//计时器安装后首先启动的间歇
	itv.it_interval.tv_usec = 0;

	/*
	 * int setitimer(int which, const struct itimerval *value,struct itimerval *ovalue);
	 *
	 * setitimer()将value指向的结构体设为计时器的当前值，如果ovalue不是NULL，将返回计时器原有值。
	 * which：间歇计时器类型，有三种选择
	 *
	 * ITIMER_REAL      //数值为0，计时器的值实时递减，发送的信号是SIGALRM
	 * ITIMER_VIRTUAL //数值为1，进程执行时递减计时器的值，发送的信号是SIGVTALRM
	 * ITIMER_PROF     //数值为2，进程和系统执行时都递减计时器的值，发送的信号是SIGPROF。
	 *
	 * 返回说明： 
	 * 成功执行时，返回0。失败返回-1，errno被设为以下的某个值 
	 * EFAULT：value或ovalue是不有效的指针
	 * EINVAL：其值不是ITIMER_REAL，ITIMER_VIRTUAL 或 ITIMER_PROF之一
	 *
	 */
	setitimer(ITIMER_REAL, &itv, NULL);
}

void signal_handler(int m)
{
	count++;
	printf("%d\n", count);	
}


int main(int argc, const char *argv[])
{
	signal(SIGALRM, signal_handler);
	set_time();
	while (count < 20);
		
	exit(0);
	return 0;
}
