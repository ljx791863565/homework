#include "common.h"

int pri_rec(char *buf)
{
	int count;
	if (buf[0] == '@'){
		printf("你的客户端编号:%d\n", buf[1]);
		printf("在线客户端编号					IP地址		\n");
		buf++;
		while (*buf){
			printf("		%d				", *buf++);
			while (*buf != '@' && *buf != 0){
				printf("%c", *buf++);
			}
			printf("\n");
			if (*buf == '@'){
				buf++;
			}
		}
	}else if (buf[0] == '&' && buf[1] == '&'){
		printf("客户端不存在\n");
	}else {
		printf("%s\n", buf++);
	}
	return 0;
}


