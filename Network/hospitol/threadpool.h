#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

typedef struct worker
{
	void* (*process)(void *arg); // 回调函数
	void *arg;                   // 参数
	struct worker *next;         // 指向下一个工作任务的指针
}CThread_worker;

typedef struct pool
{
	pthread_mutex_t queue_lock;  // 保护工作任务链表
	pthread_cond_t queue_ready;  // 线程等待条件变量
	CThread_worker *queue_head;  // 工作任务链表头指针

	int shutdown;                // 退出线程标识  0：在运行 1：退出
	pthread_t *threadid;         // 线程id

	int max_thread_num;          // 线程池中的最大线程个数
	int cur_queue_size;          // 工作任务中的任务个数
}CThread_pool;

int pool_add_worker(void *(*process)(void *arg), void *arg);
void *thread_routine(void *arg);
void pool_init(int max_thread_num);
extern CThread_pool *pool;
#endif
