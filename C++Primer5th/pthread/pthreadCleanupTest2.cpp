#include <iostream>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void mycleanFunc(void *arg)
{
	printf("mycleanFunc : %d\n", *((int *)arg));
}

void *handler1(void *arg)
{
	printf("handler1 coming\n");
	int n = 1;
	pthread_cleanup_push(mycleanFunc, &n);
	while (1){
		n++;
		printf("n = %d\n", n);
	}
	printf("this code can not be doing\n");
	pthread_cleanup_pop(0);
	return (void *)0;
}

int main()
{
	pthread_t pid1;

	int ret;
	void *pret = NULL;
	ret = pthread_create(&pid1, NULL, handler1, NULL);
	if (ret < 0){
		perror("pthread_create");
		return -1;
	}
	sleep(1);	//空出cpu时间让线程可以运行
	pthread_cancel(pid1);	//发送取消线程的请求
	pthread_join(pid1, (void **)&pret);
/*	if ((int *)pret == PTHREAD_CANCELED){
		printf("thread is stopped, and exit code : %n\n", (int *)pret);
	}else{
		printf("some error occured\n");
	}
*/
	printf("main is over: pret = %d\n", (int *)pret);
	return 0;
}
