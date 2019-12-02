#include "threadpool.h"

CThread_pool *pool = NULL;

void pool_init(int max_thread_num)
{
	pool = (CThread_pool *) malloc(sizeof(CThread_pool));//申请空间
	pthread_mutex_init(&(pool->queue_lock), NULL);
	pthread_cond_init(&(pool->queue_ready), NULL);

	pool->queue_head = NULL;

	pool->max_thread_num = max_thread_num;
	pool->cur_queue_size = 0;

	pool->shutdown = 0;//正在运行

	pool->threadid = (pthread_t *)malloc(max_thread_num * sizeof(pthread_t));//指针数组

	int i= 0;
	int ret;

	for(i = 0; i< max_thread_num; i++)
	{
		ret = pthread_create(&(pool->threadid[i]), NULL, thread_routine, NULL);
	}
}


/*向线程池加入任务*/

int pool_add_worker(void*(*process)(void *arg), void *arg)

{
	/*构建一个新任务*/
	CThread_worker * newworker = (CThread_worker *)malloc (sizeof(CThread_worker));
	newworker->process = process;//注册回调函数
	newworker->arg = arg;//回调函数入参的赋值
	newworker->next = NULL;/*不要漏掉置空这步*/

	pthread_mutex_lock(&(pool->queue_lock));
	/*将任务加到等待队列中*/
	CThread_worker *member = pool->queue_head;
	if(member != NULL)
	{
		while(member->next != NULL)
		{
			member = member->next;
		}
		member->next = newworker;//尾插
	}
	else
	{
		pool->queue_head = newworker;
	}

	pool->cur_queue_size++;
	pthread_mutex_unlock(&(pool->queue_lock));
	pthread_cond_signal(&(pool->queue_ready));

	return 0;
}

/*销毁线程池*/
int pool_destroy()
{
	if(pool->shutdown)
	{
		return -1;//防止重复销毁
	}
	pool->shutdown =1;

	/*唤醒所有等待线程，线程池准备被销毁了*/
	pthread_cond_broadcast(&(pool->queue_ready));

	/*阻塞等待线程退出，否则会成为僵尸*/
    int i;

	for(i = 0; i < pool->max_thread_num; i++)
	{
		pthread_join(pool->threadid[i], NULL);
	}

	free(pool->threadid);

	/*销毁等待队列*/
	CThread_worker *head = NULL;
	while(pool->queue_head != NULL)
	{
		head = pool->queue_head;
		pool->queue_head = pool->queue_head->next;
		free(head);
	}

	//销毁锁和条件变量
	pthread_mutex_destroy(&(pool->queue_lock));
	pthread_cond_destroy(&(pool->queue_ready));

	free(pool);//释放线程池
	pool = NULL;//销毁后置空是个好习惯
	return 0;
}

void * thread_routine(void *arg)
{
	printf("starting thread 0x%x\n",(unsigned int)pthread_self());//pthread_self 获取自身线程号函数pthread_t
	while(1)
	{
		pthread_mutex_lock(&(pool->queue_lock));
        //当工作队列里面没有可以处理的数据时，进入等待wait,根据cur_queue_size判断工作队列是否有数据;并且shutdown的时候也不需要等待，因为程序准备退出了。
		while(pool->cur_queue_size == 0 && !pool->shutdown)
		{
			printf("thread 0x%x is waiting\n", (int)pthread_self());
			pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));//开始等待
		}

		/*线程池要销毁了*/
		if(pool->shutdown)
		{
			pthread_mutex_unlock(&(pool->queue_lock));
			printf("thread 0x%x will exit\n", (int)pthread_self());
			pthread_exit(NULL);//退出线程
		}

		//如果线程池没有销毁，就走这里的正常的工作队列数据的处理
		printf("thread 0x%x is starting to work\n", (int)pthread_self());

		/*等待工作队列减去1，并取出链表里的头元素*/
		pool->cur_queue_size--;
		CThread_worker *worker = pool->queue_head;
		pool->queue_head = worker->next;//完成取结点
		pthread_mutex_unlock(&(pool->queue_lock));

		/*调用回调函数， 执行任务*/
		(*(worker->process))(worker->arg);
		free(worker);
		worker = NULL;
	}

	pthread_exit(NULL);//这行代码一般执行不到，因为前面有个死循环；
}

//测试代码

void * myprocess(void *arg)
{
	printf("thread is 0x%x,working on task %d\n", (int)pthread_self(), *(int * )arg);
	sleep(1);//休息1秒，延长任务的执行时间；

	return NULL;
}


