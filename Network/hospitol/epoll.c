#include "communal.h"

void* working(void *arg)
{
	MSG_T *p;
	p = (MSG_T *)arg;

	MSG_HEAD_T msghead;
	memset(&msghead, 0, sizeof(msghead));

	//客户端传递过来的消息前sizeof(msghead)是消息头
	memcpy(&msghead, p->buf, sizeof(msghead));

	//网络字节序转换成主机字节序
	msghead.len = ntohl(msghead.len);
	msghead.msg_type = ntohl(msghead.msg_type);

	if (msghead.msg_type == 1){
		doctor_register(p->fd, p->buf);
	}
}

int main(int argc, char **argv)
{
	int ret = 0;

	int listenfd;
	int epfd; 
	struct sockaddr_in serAddr;
	memset(&serAddr, 0, sizeof(serAddr));
	const char * const local_addr = "10.0.0.88";
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(atoi(PORT));
	serAddr.sin_addr.s_addr = inet_addr(local_addr);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);		//TCP
	if (listenfd < 0){
		perror("socket");
		return -1;
	}

	int on = 1;
	//error: Address aleeady in use   evert socket in TIME_WAIT
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
	if (ret < 0){
		perror("set socket");
		return -1;
	}

	ret = bind(listenfd, (const struct sockaddr*)&serAddr, sizeof(serAddr));
	if (ret < 0){
		perror("bind");
		return -1;
	}

	ret = listen(listenfd, MAX_LISTEN);
	if (ret < 0){
		perror("listen");
		return -1;
	}

	//epoll
	epfd = epoll_create(256);
	if (epfd < 0){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event ev, event[MAX_EVENT];
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;	//set edge-triggered
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
	if (ret < 0){
		perror("epoll_ctl");
		return -1;
	}

	struct sockaddr_in cliAddr;
	socklen_t len = sizeof(cliAddr);
	int clientFd, readFd, writeFd;
	
	pool_init(6);	//创建线程池并设置最大线程为6个

	int wait_count;		//保存通知事件的个数
	int i = 0;
	char buf[MAX_BUF];
	MSG_HEAD_T msghead;
	MSG_T *pmsg;
	while (1){
		wait_count = epoll_wait(epfd, event, MAX_EVENT, 500);
		for (i = 0; i < wait_count; i++){
			if (event[i].data.fd == listenfd)	//客户端第一次连接
			{
				clientFd = accept(listenfd, (struct sockaddr *)&cliAddr, &len);
				if (clientFd < 0){
					perror("accept");
					continue;
				}
				printf("a new client connectted, ip: %s port :%d\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

				ev.data.fd  = clientFd;
				ev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientFd, &ev);
			}else if (event[i].events & EPOLLIN)		//表示有数据读取
			{
				readFd = event[i].data.fd;
				memset(buf, 0, sizeof(buf));
				ret = read(readFd, buf, sizeof(buf));
				if (ret <= 0){
					continue;
				}
				printf("client %s: %s\n",inet_ntoa(cliAddr.sin_addr), buf);
				
				pmsg->fd = readFd;
				memcpy(pmsg->buf, buf, ret);
				
				pool_add_worker(working, pmsg);		//把这个任务加入线程池工作列表

			}

		}
	}
	close(epfd);
	return 0;
}
