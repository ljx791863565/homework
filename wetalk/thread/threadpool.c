#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

void *thread_routine(void *arg)
{
	struct timespec abstime;
	int timeout;
	printf("thread %d is starting\n", (int)pthread_self());
	threadpool_t *pool = (threadpool_t *)arg;

	while (1)
	{
		timeout = 0;
		condition_lock(&pool->ready);

		pool->idle++;

		while (pool->first == NULL && !pool->quit)
		{
			printf("thread %d is waiting\n", (int)pthread_self());
			clock_gettime(CLOCK_REALTIME, &abstime);
			abstime.tv_sec +=2;
			int status;
			status = condition_timedwait(&pool->ready, &abstime);
			if (status == ETIMEDOUT){
				printf("thread %d wait timed out\n", (int)pthread_self());
				break;
			}
		}
		pool->idle --;
		if (pool->first != NULL){
			task_t *t = pool->first;
			pool->first = t->next;
			condition_unlock(&pool->ready);
			t->run(t->arg);
			free(t);
			condition_lock(&pool->ready);
		}

		if (pool->quit && pool->first == NULL){
			pool->counter --;
			if (pool->counter == 0){
				condition_signal(&pool->ready);
			}
			condition_unlock(&pool->ready);
			break;
		}

		if (timeout == 1){
			pool->counter --;
			condition_unlock(&pool->ready);
			break;
		}

		printf("thread %d is exiting\n", (int)pthread_self());
		return NULL;
	}
}

void threadpool_init(threadpool_t *pool, int threads)
{
	condition_init(&pool->ready);
	pool->first = NULL;
	pool->last = NULL;
	pool->counter = 0;
	pool->idle = 0;
	pool->max_threads = threads;
	pool->quit = 0;
}

void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
	//产生一个新的任务
	task_t *newtask = (task_t *)malloc(sizeof(task_t));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next=NULL;//新加的任务放在队列尾端

//	线程池的状态被多个线程共享，操作前需要加锁
	condition_lock(&pool->ready);

	if(pool->first == NULL)//第一个任务加入
	{
		pool->first = newtask;
	}        
	else    
	{
		pool->last->next = newtask;
	}
	pool->last = newtask;  //队列尾指向新加入的线程

	//线程池中有线程空闲，唤醒
	if(pool->idle > 0)
	{
		condition_signal(&pool->ready);
	}
	//当前线程池中线程个数没有达到设定的最大值，创建一个新的线性
	else if(pool->counter < pool->max_threads)
	{
		pthread_t tid;
		pthread_create(&tid, NULL, thread_routine, pool);
		pool->counter++;
	}
	//结束，访问
	condition_unlock(&pool->ready);
}

//线程池销毁
void threadpool_destroy(threadpool_t *pool)
{
	//如果已经调用销毁，直接返回
	if(pool->quit)
	{
		return;
	}
	//加锁
	condition_lock(&pool->ready);
	//设置销毁标记为1
	pool->quit = 1;
	//线程池中线程个数大于0
	if(pool->counter > 0)
	{
		//对于等待的线程，发送信号唤醒
		if(pool->idle > 0)
		{
			condition_broadcast(&pool->ready);
		}
		//正在执行任务的线程，等待他们结束任务
		while(pool->counter)
		{
			condition_wait(&pool->ready);
		}
	}
	condition_unlock(&pool->ready);
	condition_destroy(&pool->ready);
}
