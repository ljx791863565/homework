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
#include <sys/epoll.h>

#define MAX_BUFF 1024
#define MAX_LISTEN 10
#define MAX_EPOLL 256
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
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&option, sizeof(option));
	if (ret < 0){
		perror("setsockopt");
		return -1;
	}
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
	
	int epfd = epoll_create(256);
	if (epfd < 0){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev, event[255];
	ev.data.fd = sockfd;
	ev.events = EPOLLIN | EPOLLET |EPOLLERR;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
	if (ret < 0){
		perror("epoll_ctl");
		return -1;
	}
	struct sockaddr_in serAddr;
	socklen_t len = sizeof(serAddr);

	int newFd, readFd;
	while (1){
		int wait_count = epoll_wait(epfd, event, 256, -1);
		if (wait_count == -1){
			if (errno == EINTR){
				continue;
			}
		}
		int i;
		for (i = 0; i < wait_count; i++){
			if (event[i].data.fd == sockfd){
				newFd = accept(sockfd, (struct sockaddr*)&serAddr, &len);
				if (newFd < 0){
					perror("accept");
					continue;
				}
				ev.data.fd = newFd;
				ev.events = EPOLLIN | EPOLLET |EPOLLERR |EPOLLRDHUP;
				epoll_ctl(epfd, EPOLL_CTL_ADD, newFd, &ev);
			}else if (event[i].events & EPOLLIN){
				readFd = event[i].data.fd;
				memset(buf, 0, sizeof(buf));
				ret = read(readFd, buf, sizeof(buf));
				if (ret < 0){
					perror("read");
					break;
				}
				cJSON *json;
				if (buf[0] != '{' || buf[0] != '['){
					break;
				}
				json = cJSON_Parse(buf);
				if (json == NULL){
					perror("cjson_parse");
					cJSON_Delete(json);
					break;
				}
				cJSON *json_cmd1 = cJSON_GetObjectItem(json, "cmd1");
				if (json_cmd1 == NULL){
					perror("cJSON_GetObjectItem)");
					cJSON_Delete(json_cmd1);
					break;
				}
				cJSON *cmd1_com = cJSON_GetObjectItem(json_cmd1, "com");
				if (cmd1_com == NULL){
					perror("cJSON_GetObjectItem)");
					cJSON_Delete(cmd1_com);
					break;
				}
				cJSON *cmd1_op = cJSON_GetObjectItem(json_cmd1, "op");
				if (cmd1_op == NULL){
					perror("cJSON_GetObjectItem)");
					cJSON_Delete(cmd1_op);
					break;
				}
				printf("%s %s %s\n", json_cmd1->valuestring, cmd1_com->valuestring, cmd1_op->valuestring);
				if (!strcmp(cmd1_com->valuestring, "ifname/lte")){
					if (!strcmp(cmd1_op->valuestring, "status")){
						system("/usr/sbin/4ginfo_json.sh 2> /dev/null");
						FILE *fp = NULL;
						fp = fopen("/tmp/info", "r");
						if (fp == NULL){
							perror("fopen");
							fclose(fp);
							continue;
						}
						char buf[MAX_BUFF];
						memset(buf, 0, sizeof(buf));
						int ret = fread(buf, sizeof(buf), 1, fp);
						if (ret < 0){
							perror("fread");
							fclose(fp);
							continue;
						}
						printf("%s\n", buf);
						ret = write(readFd, buf, sizeof(buf));
						if (ret < 0){
							perror("write");
							close(readFd);
							continue;
						}
						fclose(fp);
					}else if (!strcmp(cmd1_op->valuestring, "reboot")){
						char buff[1024];
						memset(buff, 0, sizeof(buff));
						strcpy(buff, "{{\"true\"}}");
						int ret = write(readFd, buff, sizeof(buff));
						if (ret < 0){
							perror("write");
							close(readFd);
							continue;
						}
						sleep(2);
						system("reboot");
					}else {
					}
				}
				cJSON_Delete(json);
			}
		}
	}
	close(epfd);
	return 0;
}

