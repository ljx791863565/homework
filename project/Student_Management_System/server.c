#include "student.h"
#include "unp.h"
#include "common.h"
#define MAX_EPOLL 5
#define MAX_LISTEN 5
#define MAX_FD 65535
#define MAX_EPOLL_EVENTS 1000
#define EPOLL_TIME_OUT 500
static int epollfd;
static int sockfd;


static void send_result(int fd, int result, int msgtype)
{
	MSGHEAD_T msghead;
	int len = sizeof(msghead);

	msghead.len = len;
	msghead.msgtype = msgtype;
	msghead.result = result;

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	msghead.result = htonl(msghead.result);

	int ret = write(fd, &msghead, sizeof(msghead));
	if (ret <= 0) {
		perror("write send_result");
		return ;
	}
}

static void student_Login(int fd, char *buf)
{
	STUDENT_T stu;
	memset(&stu, 0, sizeof(stu));

	//传输的包除去头部，剩下的是需要的内容
	memcpy(&stu, buf+sizeof(MSGHEAD_T), sizeof(stu));

	MYSQL db;
	MYSQL *pdb;
	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "0123", "SMSDB", 3306, NULL, 0);
	if (pdb ==NULL) {
		perror("mysql_read_connect");
		send_result(fd, 1, 4);
		return ;
	}
	printf("mysql_real_connect ok\n");

	char sqlbuf[BUFSIZE];
	memset(sqlbuf, 0, BUFSIZE);
	strcpy(sqlbuf, "set names utf8");
	mysql_query(&db, sqlbuf);

	char sqlbuf2[BUFSIZE];
	memset(sqlbuf2, 0, BUFSIZE);
	sprintf(sqlbuf2,"select passwd from student where student_id = '%s';", stu.student_id);
	int ret = mysql_query(&db, sqlbuf2);
	if (ret != 0) {
		printf("student login selete fail\n");
		send_result(fd, 1, 4);
	}
	printf("sletet ok\n");
	
	MYSQL_RES *res = mysql_store_result((MYSQL *)res);
	MYSQL_ROW result;
	int rows = mysql_num_rows(res);
	if (rows > 0) {
		result = mysql_fetch_row(res);
		if (strcmp(result[0], stu.passwd) == 0) {
			send_result(fd, 0, 4);
		}else {
			send_result(fd, 2, 4);
		}
	}else {
		send_result(fd, 1, 4);
	}
	mysql_close(&db);
}

static void student_Register(int fd, char *buf)
{	
	STUDENT_T stu;
	memset(&stu, 0, sizeof(stu));

	//传输的包除去头部，剩下的是需要的内容
	memcpy(&stu, buf+sizeof(MSGHEAD_T), sizeof(stu));

	MYSQL db;
	MYSQL *pdb;
	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "0123", "SMSDB", 3306, NULL, 0);
	if (pdb ==NULL) {
		perror("mysql_read_connect");
		send_result(fd, 1, 2);
		return ;
	}
	printf("mysql_real_connect ok\n");

	char sqlbuf[BUFSIZE];
	memset(sqlbuf, 0, BUFSIZE);
	strcpy(sqlbuf,"set names utf8");
	mysql_query(&db, sqlbuf);

	char sqlbuf2[BUFSIZE];
	memset(sqlbuf2, 0, BUFSIZE);
	sprintf(sqlbuf2, "insert into student valuse('%s', '%s', '%s', '%d', '%s', '%s', '%s', '%s', '%s');", 
			stu.student_id, stu.passwd, stu.name, stu.age, stu.sexual_distinction, stu.id_card, stu.phone_number, stu.collage, stu.specialty);

	int ret = mysql_query(&db, sqlbuf2);
	if (ret != 0) {
		printf("insert db fail:%s\n", sqlbuf2);
		send_result(fd, 1, 2);
	}
	printf("student info insert ok\n");
	send_result(fd, 0, 2);

	mysql_close(&db);

}

void *myworker(void *arg)
{
	MSG_T *pmsg;
	pmsg = (MSG_T *)arg;

	MSGHEAD_T msghead;
	//将包的前msghead大小内容作为head
	memcpy(&msghead, pmsg->buf, sizeof(msghead));
	//从网络字节序转换到主机字节序
	msghead.len = ntohl(msghead.len);
	msghead.msgtype = ntohl(msghead.msgtype);

	switch (msghead.msgtype)
	{
		case 1:
			student_Register(pmsg->fd, pmsg->buf);
			break;
		case 2:
			student_Login(pmsg->fd, pmsg->buf);
			break;
		default:
			break;
	}
}


int main(int argc, const char *argv[])
{
	if (argc != 3) {
		printf("usage: client ip port\n");
		return -1;
	}

	int ret;
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[2]));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);

	int flag = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

	ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
	assert(ret >= 0);

	ret = listen(sockfd, MAX_LISTEN);
	assert(ret >= 0);

	epollfd = epoll_create(MAX_EPOLL);
	assert(epollfd != -1);
	
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sockfd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);

	struct epoll_event events[MAX_EPOLL_EVENTS];
	char buf[BUFSIZE];

	int clientFd;
	int readFd;
	struct sockaddr_in clientAddr;
	int client_len = sizeof(clientAddr);

	//初始化线程池
	pool_init(6);

	int i;
	while (1) {
		int count = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, EPOLL_TIME_OUT);
		for (i = 0; i < count; i++) {
			if (events[i].data.fd == sockfd) {
				clientFd = accept(sockfd, (struct sockaddr *)&clientAddr, &client_len);
				if (clientFd < 0) {
					continue;
				}
				printf("a new client connective is ok\n");

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = clientFd;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, clientFd, &ev);
			}else if (events[i].events & EPOLLIN){
				readFd = events[i].data.fd;
				memset(buf, 0, BUFSIZE);
				ret = read(readFd, buf, BUFSIZE);

				if (ret <= 0) {
					continue;
				}
				printf("ret : %d\n", ret);
				MSG_T *pmsg = (MSG_T *)malloc(sizeof(MSG_T));
				if (pmsg) {
					return -1;
				}

				memset(pmsg, 0, sizeof(MSG_T));
				pmsg->fd = readFd;
				memcpy(pmsg->buf, buf, ret);
				printf("this\n");
				pool_add_worker(myworker, pmsg);
			}
			
		}
	}

	return 0;
}
