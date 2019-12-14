#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "condition.h"

typedef struct task
{
	void *(*run)(void *arg);
	void *arg;
	struct task *next;
}task_t;

typedef struct threadpool
{
	condition_t ready;
	task_t *first;			//任务队列中的第一个任务
	task_t *last;			//任务队列中的第二个任务
	int counter;			//线程池中已有的线程数
	int idle;				//线程池中空闲线程数
	int max_threads;		//线程池最大线程数
	int quit;				//退出标志
}threadpool_t;

void threadpool_init(threadpool_t *pool, int threads);

void threadpool_add_task(threadpool_t *pool, void* (*rum)(void *arg), void *arg);

void threadpool_destroy(threadpool_t *pool);
#endif
