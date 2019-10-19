#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int a = 200;	//a商品价值200
int b = 100;	//目前拥有现金100
pthread_mutex_t mutex;
void *handler1(void *arg)
{
	while (1){
		pthread_mutex_lock(&mutex);
		a -= 50;	//每样商品卖50
		b += 50;	//现金增加50
		pthread_mutex_unlock(&mutex);
//		sleep(2);
	}
}

void *handler2(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		printf("sum = %d\n", a + b);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}
int main()
{
	pthread_t pid1, pid2;
	pthread_mutex_init(&mutex, NULL);
	int ret;
	if ((ret = pthread_create(&pid1, NULL, handler1, NULL)) < 0){
		perror("pthread_create");
		return -1;
	}
	pthread_join(pid1, NULL);
	if ((ret = pthread_create(&pid2, NULL, handler2, NULL)) < 0){
		perror("pthread_create");
		return -1;
	}
	pthread_join(pid2, NULL);
	pthread_mutex_destroy(&mutex);
	return 0;
}
