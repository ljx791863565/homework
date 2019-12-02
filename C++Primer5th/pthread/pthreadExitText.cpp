#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


void *handler1(void *arg)
{
	static char str[20] = "handler1 is over";	//此处需要传递给pthread_join的参数必须是static的，不可以在栈上，否则随着线程退出会变成随即值，那么接收信息就没有意义了
	pthread_exit((void*)(str));
}

void *handler2(void *arg)
{
	static char str[20] = "handler2 is over";
	return (void*)(str);
}
int main()
{
	pthread_t pid[2];
	int ret;

	ret = pthread_create(&pid[0], NULL, handler1, NULL);
	if (ret < 0){
		perror("pthread_create");
		return -1;
	}

	ret = pthread_create(&pid[1], NULL, handler2, NULL);
	if (ret < 0){
		perror("pthread_create");
		return -1;
	}

	char *pRet1;
	char *pRet2;
	
	if (pid[0] != 0){
		pthread_join(pid[0], (void **)&pRet1);		//此处第二个参数接收到线程退出时的返回值
		printf("get thread 0 exitcode: %s\n", pRet1);
	}
	if (pid[1] != 0){
		pthread_join(pid[1], (void **)&pRet2);
		printf("get thread 1 exitcode: %s\n", pRet2);
	}

	return 0;
}
