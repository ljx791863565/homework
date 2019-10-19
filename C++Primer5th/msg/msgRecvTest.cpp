#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define BUFSIZE 1024
struct my_msg_st{
	long int my_msg_type;
	char some_text[BUFSIZE];
};

int main()
{
	int running = 1;	//控制循环的进行
	
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;	//long tmsgtype 此标识为0表示消息队列中所有的消息都会被读取

	msgid = msgget(0x1234, 0666|IPC_CREAT);		//创建一个消息队列，key值是自定义的0x1234
	if (msgid == -1){
		fprintf(stderr, "msgget failed with errno %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(running){
		if (msgrcv(msgid, (void *)&some_data, BUFSIZE, msg_to_receive, 0) == -1){
			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		printf("you wrote : %s", some_data.some_text);	//打印从消息队列中读取到的信息

		if (strncmp(some_data.some_text, "end", 3) == 0){
			running = 0;	//接收到消息为 "end" 结束读取
		}
	}

	if (msgctl(msgid, IPC_RMID, 0) == -1){
		//结束后删除该消息队列
		fprintf(stderr, "msgctl(ICP_RMID) failed : %d\n", errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
