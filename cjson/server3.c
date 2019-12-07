#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "cJSON.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_BUFF 1024
#define MAX_LISTEN 10
#define MAX_EPOLL 256

void *replyStatus(void *arg)
{
	int fd = *(int *)arg;
	system("/usr/sbin/4ginfo_json.sh 2> /dev/null");
	FILE *fp = fopen("/tmp/info", "r");
	if (fp == NULL){
		perror("fopen");
		return;
	}
	char buf[MAX_BUFF];
	memset(buf, 0, sizeof(buf));
	int ret = fread(buf, sizeof(buf), 1, fp);
	if (ret < 0){
		perror("fread");
		return;
	}
	ret = write(fd, buf, sizeof(buf));
	if (ret < 0){
		perror("write");
		return;
	}
}

void *replyReboot(void *arg)
{
	int fd = *(int*)arg;
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	strcpy(buff, "{{\"true\"}}");
	int ret = write(fd, buff, sizeof(buff));
	if (ret < 0){
		perror("write");
		return;
	}
	sleep(2);
	system("reboot");
}
int main(int argc, char **argv)
{
	if (argc > 3){
		printf("argc number error\n");
		return -1;
	}

	char buf[MAX_BUFF];
	int sockfd;
	int ret = 0;
	struct sockaddr_in cliAddr;
	cliAddr.sin_family = AF_INET;
	cliAddr.sin_port = htons(atoi(argv[2]));
	cliAddr.sin_addr.s_addr = inet_addr(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("socket");
		return -1;
	}
	int option = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&option, sizeof(option));
	ret = bind(sockfd, (struct sockaddr*)&cliAddr, sizeof(cliAddr));
	if (ret < 0){
		perror("bind");
		return -1;
	}

	ret = listen(sockfd, MAX_LISTEN);
	if (ret < 0){
		perror("listen");
		return -1;
	}

	struct sockaddr_in serAddr;
	socklen_t len = sizeof(serAddr);
	int newFd = accept(sockfd, (struct sockaddr*)&serAddr, &len);
	if (newFd < 0){
		perror("accept");
		return -1;
	}
	pthread_t pid1, pid2;
	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(newFd, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return -1;
		}
		cJSON *json;
		json = cJSON_Parse(buf);
		if (json == NULL){
			cJSON_Delete(json);
			return NULL;
		}
		cJSON *json_cmd1 = cJSON_GetObjectItem(json, "cmd1");
		cJSON *cmd1_com = cJSON_GetObjectItem(json_cmd1, "com");
		cJSON *cmd1_op = cJSON_GetObjectItem(json_cmd1, "op");

		if (!strcmp(cmd1_op->valuestring, "status")){
			pthread_create(&pid1, NULL, replyStatus, &newFd);
			if (pid1 < 0){
				perror("pthread_create");
				return -1;
			}
		}else if (!strcmp(cmd1_op->valuestring, "reboot")){
			pthread_create(&pid2, NULL, replyReboot, &newFd);
			if (pid2 < 0){
				perror("pthread_create");
				return -1;
			}
		}else {
			
		}
		printf("server recv:%s\n", buf);
		cJSON_Delete(json);
	}
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	return 0;
}

