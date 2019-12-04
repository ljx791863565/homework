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

#define MAX_BUFF 1024			//update 2019.11.21

cJSON *jsonroot = NULL;
cJSON *jsoncmd1 = NULL;
cJSON *jsonout = NULL;

void* handler(void *arg)
{
	int fd = *(int *)arg;
	char buf[MAX_BUFF];
	int ret;
	while (1){
		memset(buf, 0, sizeof(buf));
		ret = read(fd, buf, sizeof(buf));
		if (ret < 0){
			perror("client:read");
			return ;
		}
		printf("server to client: %s\n", buf);
	}
}
cJSON *getJson(const char *str)
{
		const char *p = str;
		jsonroot = NULL;
		jsoncmd1 = NULL;
		jsonroot = cJSON_CreateObject();
		jsoncmd1 = cJSON_CreateObject();
		cJSON_AddStringToObject(jsoncmd1, "com", "ifname/lte");
		cJSON_AddStringToObject(jsoncmd1, "op", p);
		cJSON_AddItemToObject(jsonroot, "cmd1", jsoncmd1);
		jsonout = cJSON_Print(jsonroot);
		return jsonout;
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

	ret = connect(sockfd, (struct sockaddr*)&cliAddr, sizeof(cliAddr));
	if (ret < 0){
		perror("connect");
		close(sockfd);
		return -1;
	}
	printf("client %s connect suss, port = %d\n", argv[1], atoi(argv[2]));
	pthread_t pid;
	ret = pthread_create(&pid, 0, handler, &sockfd);
	if (ret < 0){
		perror("pthread_create");
		return -1;
	}
	jsoncmd1 = cJSON_CreateObject();
	while (1){
/*		memset(buf, 0, sizeof(buf));
		ret = read(0, buf, sizeof(buf));
		if (ret < 0){
			perror("read");
			return -1;
		}
*/
		sleep(5);
		jsonout = NULL;
		//jsonout = getJson("status");
		jsonout = getJson("reboot");
		ret = write(sockfd, jsonout, strlen(jsonout));
		if (ret < 0){
			perror("client:write");
			return -1;
		}
		printf("client %d :%s\n",inet_addr(argv[1]), buf);
	}
	pthread_join(pid, NULL);
	cJSON_Delete(jsonroot);	
	return 0;
}
