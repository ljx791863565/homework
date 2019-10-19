#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
	pid_t pid;
	char buf[1024];
	int fd[2];
	if (pipe(fd) == -1){
		perror("pipe");
		exit(1);
	}
	pid =fork();

	if (pid < 0){
		perror("fork");
		exit(1);
	}else if(pid == 0){
		close(fd[1]);	//关闭写描述符
		printf("child process wait to read:\n");
		int len = read(fd[0], buf, sizeof(buf));
		write(STDOUT_FILENO,  buf, len);
		close(fd[0]);
	}else{
		close(fd[0]);
		write(fd[1], "this is father pipe missage to you", 30);
		wait(NULL);
		close(fd[1]);
	}

	return 0;
}
