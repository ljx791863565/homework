#ifndef __THREADTOOL_H__
#define __THREADTOOL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

typedef struct worker
{
	void *(*process)(void *arg);//工作的回调函数
	void *arg;//回调函数的入参
	struct worker *next;
}CThread_worker;

typedef struct pool
{
	pthread_mutex_t queue_lock;//保护工作任务链表
	pthread_cond_t queue_ready;//线程等待，条件变量//状态锁
	CThread_worker *queue_head;//工作任务的链头

	int shutdown;//退出线程的标志 0：在运行  1：退出
	pthread_t *threadid;//保存我们创建线程的id

	int max_thread_num;//线程池里面线程的最的大个数
	int cur_queue_size;//工作任务里面任务的个数

}CThread_pool;

int pool_add_worker(void *(*process)(void *arg), void *arg);
void * thread_routine(void *arg);

extern CThread_pool *pool;

#endif
