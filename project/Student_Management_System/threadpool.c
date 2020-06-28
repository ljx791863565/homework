#include "threadpool.h"

CThread_pool *pool = NULL;

//线程池初始化
void pool_init(int max_thread_num)
{
	pool = (CThread_pool*)malloc(sizeof(CThread_pool));
	pthread_mutex_init(&(pool->queue_lock), NULL);
	pthread_cond_init(&(pool->queue_ready), NULL);

	pool->queue_head = NULL;
	pool->max_thread_num = max_thread_num;
	pool->cur_queue_size = 0;

	pool->shutdown = 0;		//设置线程池运行状态 0：正在运行

	//申请到存放max_thread个线程id的线程id数组的地址
	pool->threadid = (pthread_t *)malloc(max_thread_num *sizeof(pthread_t));

	int i = 0;
	int ret;

	//创建max_thread个线程
	for (i = 0; i < max_thread_num; i++){
		ret = pthread_create(&(pool->threadid[i]), NULL, thread_routine, NULL);
		if (ret < 0){
			perror("pthread_create");
			return ;
		}
	}
}

//加入线程池任务列表
int pool_add_worker(void *(process)(void *arg), void *arg)
{
	printf("new working is in pool\n");
	//构建一个新的任务
	CThread_worker *newworker = (CThread_worker *)malloc(sizeof(CThread_worker));
	newworker->process = process;
	newworker->arg = arg;
	newworker->next = NULL;

	pthread_mutex_lock(&(pool->queue_lock));
	CThread_worker *member = pool->queue_head;
	if (member != NULL){
		while (member->next != NULL){
			member = member->next;
		}
		member->next = newworker;		//此时线程池中已经有在工作的任务，需要添加到任务链表的最未
	}else{
		pool->queue_head = newworker;	//此时线程池中还没有工作任务，加入的一个是第一个任务
	}

	pool->cur_queue_size ++;			//工作中的任务个数更新
	pthread_mutex_unlock(&(pool->queue_lock));	
	pthread_cond_signal(&(pool->queue_ready));		//唤醒线程池中的一个线程领取此任务去工作

	return 0;
}

//线程池销毁
int pool_destroy()
{
	//只销毁正在运行的线程池，如果已经销毁过的，防止重复销毁
	if (pool->shutdown){
		return -1;
	}
	
	//设置标志表示要销毁了
	pool->shutdown = 1;

	//唤醒所有等待的线程
	pthread_cond_broadcast(&(pool->queue_ready));

	int i;
	//依次结束线程
	for (i = 0; i < pool->max_thread_num; i++){
		pthread_join(pool->threadid[i], NULL);
	}

	//释放掉线程池中的线程id的空间
	free(pool->threadid);

	CThread_worker *head = NULL;
	//依次释放工作任务的空间
	while (pool->queue_head != NULL){
		head = pool->queue_head;
		pool->queue_head = pool->queue_head->next;
		free(head);
	}

	//销毁锁和条件变量
	pthread_mutex_destroy(&(pool->queue_lock));
	pthread_cond_destroy(&(pool->queue_ready));


	//释放线程池的空间
	free(pool);
	pool = NULL;
	return 0;
}


//线程池的工作函数
void *thread_routine(void *arg)
{
	printf("starting thread 0x%x\n", (unsigned int)pthread_self());

	while (1){
		pthread_mutex_lock(&(pool->queue_lock));
		//线程池中没有正在运行的工作线程时，阻塞住所有的线程，告知它们等待被唤醒
		while (pool->cur_queue_size == 0 && !pool->shutdown){
			printf("thread 0x%x is waiting\n", (int)pthread_self());
			pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
		}

		//线程池被销毁时，释放锁，结束线程
		if (pool->shutdown){
			pthread_mutex_unlock(&(pool->queue_lock));
			printf("thread 0x%x will exit\n", (int)pthread_self());
			pthread_exit(NULL);
		}

		printf("thread 0x%x is starting to working\n", (int)pthread_self());
		
		//取出工作链表中的头结点，并让工作中的线程数-1
		pool->cur_queue_size --;
		CThread_worker *worker = pool->queue_head;
		pool->queue_head = worker->next;
		pthread_mutex_unlock(&(pool->queue_lock));

		//让这个节点领取任务
		(*(worker->process))(worker->arg);
		free(worker);
		worker = NULL;
	}
}


