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
	char buf[BUFSIZE];
	
	msgid = msgget(0x1234, 0666|IPC_CREAT);		//创建一个消息队列，key值是自定义的0x1234
	if (msgid == -1){
		fprintf(stderr, "msgget failed with errno %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while(running){
		printf("Enter some text\n");
		fgets(buf, BUFSIZE, stdin);
		some_data.my_msg_type = 1;
		strcpy(some_data.some_text, buf);
		if (msgsnd(msgid, (void *)&some_data, BUFSIZE, 0) == -1){
			fprintf(stderr, "msgsnd failed with errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}

		if (strncmp(buf, "end", 3) == 0){
			running = 0;	//接收到消息为 "end" 结束读取
		}
	}

	exit(EXIT_SUCCESS);
}
