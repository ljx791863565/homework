#include <iostream>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
void mycleanFunc(void *arg)
{
	printf("mycleanFunc : %d\n", *((int *)arg));
}

void *handler1(void *arg)
{
	printf("handler1 coming\n");
	int n = 1;
	pthread_cleanup_push(mycleanFunc, &n);
	return (void *)0;
	pthread_cleanup_pop(0);
}

void *handler2(void *arg)
{
	printf("handler2 coming\n");
	int n = 2;
	pthread_cleanup_push(mycleanFunc, &n);
	pthread_exit(0);
	pthread_cleanup_pop(0);
}
int main()
{
	pthread_t pid1, pid2;

	int ret;

	ret = pthread_create(&pid1, NULL, handler1, NULL);
	if (ret < 0){
		perror("pthread_create");
		return -1;
	}

	pthread_join(pid1, NULL);

	ret = pthread_create(&pid2, NULL, handler2, NULL);
	if (ret < 0){
		perror("pthread_create");
		return -1;
	}
	pthread_join(pid2, NULL);

	printf("main is over\n");

	return 0;
}
