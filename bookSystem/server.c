#include "unp.h"
#include "threadpool.h"
#include <mysql/mysql.h>

#define MAX_FD 1024
#define MAX_EVENT 1024
#define MAX_THREAD 6
#define EPOLL_WAIT_TIME_OUT 500
#define LOCAL_ADDR "127.0.0.1"
#define MYSQL_USER "root"
#define MYSQL_PASSWD "0321"
#define DB_DATABASE "bsm"
#define SETNAMESUTF8 "set names utf8"
void *handler(void *arg)
{
	MSG_T *msg = (MSG_T*)mag;
	MSGHEAD_T msghead;
	bzero(&msghead, 0);
	memcpy(&msghead, msg->buf, sizeof(MSGHEAD_T));
	
	msghead.len = ntohl(msghead.len);
	msghead.msgtype = ntohl(msghead.msgtype);
	
	switch (msghead.msgtype)
	{
		case 1:
			user_register(msg->fd, msg->buf);
			break;
		default:
			err_msg("msgtype error");
			break;
	}
}

void send_result_msg(int fd, int result, int msgtype)
{
	MSGHEAD_T msghead;
	msghead.len = htonl(sizeof(msghead));
	msghead.msgtype = htonl(msgtype);
	msghead.result_type = htonl(result);
	
	int ret = write(fd, &msghead, sizeof(msghead));
	if (ret < 0) {
		err_sys("write error");
	}
}

int Mysql_init(int fd, int msgtype, MYSQL *pdb, MYSQL **ppdb)
{
	mysql_init(pdb);

	*ppdb = mysql_real_connect(pdb, LOCAL_ADDR, MYSQL_USER, MYSQL_PASSWD, DB_DATABASE, 3306, NULL, 0);
	if ( *ppdb == NULL ) {
		send_result_msg(fd, 1, msgtype);	//1 表示失败
		err_sys("mysql connect fail");
	}
	err_msg("mysql connect ok");
	mysql_query(pdb, SETNAMESUTF8);
}

void user_register(int fd, char *buf)
{
	int msgtype = 2;
	user_info_t user_info;
	bzero(&user_info, 0);

	memcpy(&user_info, buf + sizeof(MSGHEAD_T), sizeof(user_info));
	
	MYSQL db;
	MYSQL *pdb;

	Mysql_init(fd, msgtype, &db, &pdb);

	char sql_commond[BUFSIZE];
	bzero(sql_commond, 0);

	sprintf(sql_commond, "insert into user values('%s, %s, %s, %s, %s, %lf');", 
			user_info.user.id, user_info.user.passwd,user_info.name, user_info.sfz, user_info.sex, user_info.balance);
	int ret = mysql_query(&db, sql_commond);
	if (ret == 0) {
		err_msg("userinfo insert ok");
		send_result_msg(fd, 0, msgtype);	
	}
	err_msg("userinfo insert fail");
	send_result_msg(fd, 1, msgtype);		//3 表示user写入失败

	
	mysql_close(&db);
}


int main(int argc, const char *argv[])
{
	int listenfd, epfd, connfd;
	struct sockaddr_in clientAddr, serverAddr;
	socklen_t clilen = sizeof(clientAddr);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serverAddr, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	inet_aton(argv[1], &serverAddr.sin_addr);

	int on = 1;
	Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	Bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	Listen(listenfd, LISTENQ);
	bzero(&clientAddr, 0);
	epfd = Epoll_create(MAX_FD);

	struct epoll_event ev, events[MAX_EVENT];
	ev.data.fd = listenfd;
	ev.events = EPOLLIN | EPOLLET;
	Epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	char buf[BUFSIZE];
	int wait_count;
	int readfd;
	int i, ret;

	pool_init(MAX_THREAD);
	MSG_T *msg;
	while (1) {
		wait_count = epoll_wait(epfd, events, MAX_EVENT, EPOLL_WAIT_TIME_OUT);
		for (i = 0; i < wait_count; i++) {
			if (events[i].data.fd == listenfd) {
				connfd = Accept(listenfd, (struct sockaddr*)&clientAddr, &clilen);
				err_msg("a new client ip: %s port: %d connectted to server", 
						inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connfd;
				Epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
			} else if (events[i].events & EPOLLIN) {
				readfd = events[i].data.fd;
				bzero(buf, 0);
			
				ret = read(readfd, buf, BUFSIZE);
				if (ret <= 0) {
					continue;
				}
				msg = (MSG_T*)malloc(sizeof(MSG_T));
				bzero(msg, 0);
				msg->fd =  readfd;
				strcpy(msg->buf, buf, sizeof(BUF));

				pool_add_worker(handler, msg);
			}
		}
	}
	return 0;
}
