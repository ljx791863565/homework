/*
	生产者和消费者问题
	多线程同步问题
	四个线程，两个生产者用于生产
			  两个消费者产生消费
	共享多个缓冲区，当缓冲区满时让生产者阻塞等待最少消费1，当缓冲区为空时让消费者阻塞等待，至少生产1
	因为对相同的数据进行访问，需要做互斥锁处理
	

   */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>

#define PRODUCER_COUNT 2	//生产者人数
#define CONSUMER_COUNT 2	//生产者人数
#define BUFSIZE 5
int g_buf[BUFSIZE];		//缓存区数目
pthread_mutex_t g_lock;

unsigned short in  = 0;		//放入产品的指针 
unsigned short out = 0;		//取出缓存区指针
unsigned short producer_id = 0;
unsigned short consumer_id = 0;

sem_t g_sem_full;	//可以使用的空的缓冲区数（缓冲区可以生产多少产品）
sem_t g_sem_empty;	//缓冲区内可以使用的产品数（可消费的产品数)

pthread_t g_thread[PRODUCER_COUNT + CONSUMER_COUNT];

void *consumer(void *arg)
{
	int i;
	int *p = (int *)arg;
	int num = *p;
	printf("this is consumer\n");
	while (1){
		printf("%d wait buf not empty\n", num);
		sem_wait(&g_sem_empty);
		pthread_mutex_lock(&g_lock);

		//遍历缓冲区，看看有哪些缓冲区是可以生产产品的
		for (i=0; i<BUFSIZE; i++){
			printf("%.2d ", i);
			if (g_buf[i] == -1){
				printf("%s", "null");
			}else {
				printf("%d", g_buf[i]);
			}

			if (i == out){
				printf("\t<--consumer");
			}

			printf("\n");
		} 
		//produce()操作 生产产品
		consumer_id = g_buf[out];
		printf("%d begin consume product %d\n", num, consumer_id);
		g_buf[out] = -1;
		//将取出缓冲区的指针偏移1 
		out = (out + 1) % BUFSIZE;
		printf("%d end consume priduct %d\n", num, consumer_id);

		pthread_mutex_unlock(&g_lock);
		sem_post(&g_sem_full);
		sleep(1);
	}
	return NULL;
}

void *producer(void *arg)
{
	printf("this is producer\n");
	int *p = (int *)arg;
	int num = *p;
	int i;
	while (1){
		printf("%d wait buf not full\n", num);
		sem_wait(&g_sem_full);

		pthread_mutex_lock(&g_lock);
		for (i=0; i<BUFSIZE; i++){
			printf("%.2d ", i);
			if (g_buf[i] == -1){
				printf("%s", "null");
			}else{
				printf("%d", g_buf[i]);
			}

			if (i == num){
				printf("\t<--produce");
			}

			printf("\n");
		}
		printf("%d begin produce product %d\n", num, producer_id);
		g_buf[in] = producer_id;
		in = (in + 1) % BUFSIZE;
		printf("%d end produce product %d\n", num, producer_id++);
		
		pthread_mutex_unlock(&g_lock);
		sem_post(&g_sem_empty);
		sleep(5);
	}

	return NULL;
}
int main()
{
	int i;
	for (i=0; i<BUFSIZE; i++){
		g_buf[i] = -1;
	}

	sem_init(&g_sem_full, 0, BUFSIZE);
	sem_init(&g_sem_empty, 0, 0);

	pthread_mutex_init(&g_lock, NULL);
	for (i=0; i<CONSUMER_COUNT; i++){
		pthread_create(&g_thread[i], NULL, consumer, &i);
	}

	for (i=0; i<PRODUCER_COUNT; i++){
		pthread_create(&g_thread[CONSUMER_COUNT + i], NULL, producer, &i);
	}

	for (i=0; i<CONSUMER_COUNT + PRODUCER_COUNT; i++){
		pthread_join(g_thread[i], NULL);
	}

	sem_destroy(&g_sem_full);
	sem_destroy(&g_sem_empty);
	pthread_mutex_destroy(&g_lock);
	return 0;
}
