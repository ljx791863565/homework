#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
typedef struct{
	int n;
	char buf[20];
}MS;

void* handler(void * arg)
{
	MS *p = (MS *)arg;

	printf("in the handler , n = %d, buf = %s\n",p->n, p->buf);
	return (void*)0;
}
int main()
{
	MS ms;
	ms.n = 100;
	strcpy(ms.buf, "hello world");

	pthread_t pid;
	int ret = pthread_create(&pid, NULL, handler, (void *)&ms);
	if (ret){
		perror("pthread_create");
		return -1;
	}

	pthread_join(pid, NULL);	//阻塞主线程让子线程有机会运行，当检测到子线程运行结束后，阻塞结束，接着运行主线程后面代码
	
	return 0;
}
