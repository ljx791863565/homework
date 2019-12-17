/*************************************************************************
    > File Name: httpd.c
    > Author: ljx
    > Mail: liaojia138797@163.com 
    > Created Time: Tue 17 Dec 2019 03:10:33 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>//
#include <sys/wait.h>
#include <sys/stat.h>


#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: ljx-tiny-httpd/0.1.0\r\n"
#define MAX_LISTEN 5
#define MAX_BUF 1024 
#define MAX_URL 255
#define MAX_PATH 512

void accept_request(int);
void bad_request(int);
void cat(int ,FILE*);
void cannot_execute(int);
void execute_cgi(int, const char *, const char *, const char *);
int get_line(int, char *, int);
void handers(int, FILE*);
void not_found(int);
void serve_file(int, const char *);
int startup(u_short *);
void unimplemented(int);
void readBufferBeforeSend(int);
int get_line(int sock, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size -1) && (c != '\n')){
		n = recv(sock, &c, 1, 0);
		if (n > 0){
			if (c == '\r'){
				n = recv(sock, &c, 1, MSG_PEEK);
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else 
					c = '\n';
			}
			buf[i] = c;
			i++;
		}else{
			c = '\n';
		}
	}

	buf[i] = '\0';
	return i;
}

void readBufferBeforeSend(int connfd)
{
	int numchars = 1;
	char buf[MAX_BUF];

	int content_length = -1;
	int i;
	char c;

	buf[0] = 'A';
	buf[1] = '\0';

	while ((numchars > 0) && strcmp("\n", buf)){
		buf[15] = '\0';		

		if (strcasecmp(buf, "Content-Length:") == 0){
			content_length = atoi(&(buf[16]));
		}
		numchars = get_line(connfd, buf, sizeof(buf));
	}

	if (content_length != -1){
		for (i = 0; i < content_length; i++){
			recv(connfd, &c, 1, 0);
		}
	}
}

void serve_file(int connfd, const char *filename)
{
	FILE *resource = NULL;
	int numchars = 1;
	char buf[MAX_BUF];

	buf[0] = 'A';
	buf[1] = '\0';

	while ((numchars > 0) && strcmp("\n", buf)){
		numchars = get_line(connfd, buf, sizeof(buf));
	}

	resource = fopen(filename, "r");
	if (resource == NULL){
		not_found(connfd);
	}else {
		handers(connfd, filename);
		cat(connfd, resource);
	}

	fclose(resource);
}

void execute_cgi(int connfd, const char *path, const char *method, const char *query_string)
{
	char buf[MAX_BUF];
	int cgi_output[2];
	int cgi_input[2];
	pid_t pid;
	int status;
	int i;
	char c;
	int numchars = 1;
	int content_length = -1;

	buf[0] = 'A';
	buf[1] = '\0';

	if (strcasecmp(method, "GET") == 0){
		while ((numchars > 0) && strcmp("\n", buf)){
			numchars = get_line(connfd, buf, sizeof(buf));
		}
	}else {
		numchars = get_line(connfd, buf, sizeof(buf));
		while ((numchars > 0) && strcmp("\n", buf)){
			buf[15] = '\0';
			if (strcasecmp(buf, "Content-Length:") == 0){
				content_length = atoi(&(buf[16]));
			}
			numchars = get_line(connfd, buf, sizeof(buf));
		}

		if (content_length == -1){
			bad_request(connfd);
			return;
		}
	}

	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	send(connfd, buf, strlen(buf), 0);

	if (pipe(cgi_output) < 0){
		cannot_execute(connfd);
		return;
	}

	if (pipe(cgi_input) < 0){
		cannot_execute(connfd);
		return;
	}

	if ((pid = fork()) < 0){
		cannot_execute(connfd);
		return;
	}

	if (pid == 0){
		char meth_env[MAX_URL];
		char query_env[MAX_URL];
		char length_env[MAX_URL];

		dup2(cgi_output[1], 1);
		dup2(cgi_input[0], 0);
		close(cgi_output[0]);
		close(cgi_input[1]);

		sprintf(meth_env, "REQUEST_MOTHOD=%s", method);
		putenv(meth_env);

		if (strcasecmp(method, "GET") == 0){
			sprintf(query_env, "QUERY_STRING=%s", query_string);
			putenv(query_env);
		}else{
			sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
			putenv(length_env);
		}

		execl(path, path, NULL);
		return;
	}else{
		close(cgi_output[1]);
		close(cgi_input[0]);
		if (strcasecmp(method, "POST") == 0){
			for (i = 0; i < content_length; i++){
				recv(connfd, &c, 1, 0);
				write(cgi_input[1], &c, 1);
			}
		}

		while (read(cgi_output[0], &c, 1) > 0){
			send(connfd, &c, 1, 0);
		}

		close(cgi_output[0]);
		close(cgi_input[1]);

		waitpid(pid, &status, 0);
	}
}


void bad_request(int connfd)
{
	char buf[MAX_BUF];
	memset(buf, 0, sizeof(buf));

	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(connfd, buf, sizeof(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, sizeof(buf), 0);
	sprintf(buf, "\r\n");
	send(connfd, buf, sizeof(buf), 0);
	sprintf(buf, "<P>Your browser sent a bad request,\r\n");
	send(connfd, buf, sizeof(buf), 0);
	sprintf(buf, "such as a POST without a Content-Length.\r\n");
	send(connfd, buf, sizeof(buf), 0);
}

void cannot_execute(int connfd)
{
	char buf[MAX_BUF];
	memset(buf, 0, sizeof(buf));

	sprintf(buf, "HTTP/1.0 500 Method Internal Server Error\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
	send(connfd, buf, strlen(buf), 0);
	
}
void unimplemented(int connfd)
{
	char buf[MAX_BUF];
	memset(buf, 0, sizeof(buf));

	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "</TITLE></HEAD>\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>HTTP request Method Not Implemented\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HEAD>\r\n");
	send(connfd, buf, strlen(buf), 0);

}

void not_found(int connfd)
{
	char buf[MAX_BUF];
	memset(buf, 0, sizeof(buf));

	sprintf(buf, "HTTP/1.0 404 Not FOUND\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "is nuavailable or ljx nonexistent.\r\n");
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(connfd, buf, strlen(buf), 0);

}

void cat(int connfd, FILE *filename)
{
	char buf[MAX_BUF];
	memset(buf, 0, sizeof(buf));

	fgets(buf, sizeof(buf), filename);
	while (!feof(filename)){
		send(connfd, buf, strlen(buf), 0);
		fgets(buf, sizeof(buf), filename);
	}
}

void handers(int connfd, FILE *filename)
{
	char buf[MAX_BUF];
	memset(buf, 0, sizeof(buf));

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(connfd, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(connfd, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(connfd, buf, strlen(buf), 0);
}

void accept_request(int connfd)
{
	char buf[MAX_BUF];
	int numchars;		//按字节读取数目
	char method[MAX_URL];	//请求方法
	char url[MAX_URL];		//请求url
	char path[MAX_PATH];	//文件路径

	size_t i = 0, j = 0;
	struct stat st;
	int cgi = 0;
	char *query_string = NULL;

	numchars = get_line(connfd, buf, sizeof(buf));
	
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1)){
		method[i] = buf[j];
		i++;
		j++;
	}
	method[i] = '\0';

	if (strcasecmp(method , "GET") && strcasecmp(method, "POST")){
		readBufferBeforeSend(connfd);
		unimplemented(connfd);
		close(connfd);
		return;
	}

	if (strcasecmp(method, "POST") == 0){
		cgi = 1;
	}

	i = 0;
	while (!ISspace(buf[j]) && (i < sizeof(buf))){
		url[i] = buf[j];
		i++;
		j++;
	}
	url[i] = '\0';

	if (strcasecmp(method, "GET") == 0){

		query_string = url;
		while ((*query_string != '?') && (*query_string != '\0')){
			query_string++;
		}

		if (*query_string == '?'){
			*query_string = '\0';
			query_string++;
		}
	}

	sprintf(path, "htdocs%s", url);

	if (path[strlen(path) - 1] == '/'){
		strcat(path, "index.html");
	}

	if (stat(path, &st) == -1){
		readBufferBeforeSend(connfd);
		not_found(connfd);
	}else{
		if ((st.st_mode & S_IFMT) == S_IFDIR){
			strcat(path, "/index.heml");
		}

		if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)){
			cgi = 1;
		}

		if (!cgi){
			serve_file(connfd, path);
		}else{
			execute_cgi(connfd, path, method, query_string);
		}
	}

	close(connfd);
}

//socket bind listen...
int startup(u_short *port)
{
	int httpd = 0;
	struct sockaddr_in name;
	httpd = socket(AF_INET, SOCK_STREAM, 0);
	if (httpd == -1){
		perror("socket");
		return -1;
	}
	int option = 1;
	if (setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0){
		perror("setsockopt");
		return -1;
	}

	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(httpd, (struct sockaddr*)&name, sizeof(name)) < 0){
		perror("bind");
		return -1;
	}
	if (*port == 0){
		socklen_t len = sizeof(name);
		if (getsockname(httpd, (struct sockaddr *)&name, &len) == -1){
			perror("getsockname");
			return -1;
		}
		*port = ntohs(name.sin_port);
	}

	if (listen(httpd, MAX_LISTEN) < 0){
		perror("listen");
		return -1;
	}
	
	return httpd;
}

int main()
{
	int listenfd = -1;	//?
	u_short port = 8090;
	int connfd = -1;

	struct sockaddr_in client;
	socklen_t client_len = sizeof(client);
	
	listenfd = startup(&port);
	printf("httpd running on port%d\n", port);
	if (listenfd == -1){
		perror("startup");
		return -1;
	}

	while (1){
		connfd = accept(listenfd, (struct sockaddr*)&client, &client_len);
		if (connfd == -1){
			perror("accept");
			return -1;
		}

		accept_request(connfd);
	}
	close(listenfd);
	return 0;
}
