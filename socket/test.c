#include "threadpool.h"
void *myprocess(void *arg)
{
	printf("thread is 0x%x, working on task %d\n", (int)pthread_self(), *(int*)arg);
	sleep(1);
}
int main()
{
	pool_init(6);
	int n = 6;
	pool_add_worker(myprocess, &n);
	
}
