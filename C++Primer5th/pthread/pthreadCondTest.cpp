#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

int i = 1;

void* handler1(void *arg)
{
	for (i=1; i < 20; i++){
		pthread_mutex_lock(&mutex);
		if (i % 3 == 0){
			pthread_cond_signal(&cond);		//唤醒条件变量的线程
		}else{
			printf("thread1 : %d can not divide exactly 3\n", i);
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void *handler2(void *arg)
{
	while (i < 20){
		pthread_mutex_lock(&mutex);
		if (i % 3 != 0){
			pthread_cond_wait(&cond, &mutex);		//等待的条件变量,持续阻塞直到条件被满足
		}
		printf("thread2: %d can divide exactly 3 \n", i);
		pthread_mutex_unlock(&mutex);
		sleep(1);
		i++;
	}
}
int main()
{
	pthread_t pid1, pid2;
	int ret;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	if (ret = (pthread_create(&pid1, NULL, handler1, NULL)) < 0){
		perror("pthread_create");
		return -1;
	}
//	pthread_join(pid1, NULL);
	if (ret = (pthread_create(&pid2, NULL, handler2, NULL)) < 0){
		perror("pthread_create");
		return -1;
	}
	pthread_join(pid2, NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}
