#include "apue.h"
#include <sys/wait.h>
#include <error.h>

void sig_int(int signo)
{
	printf("interruot\n%% ");
}

//从stdio输入读取命令 fork一个子进程execlp调用这个命令 父进程获取子进程退出状态
int main(int argc, const char *argv[])
{
	char buf[MAXLINE];
	pid_t pid;
	int status;

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		err_sys("signal error");
	}
	printf("%% ");
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) -1] == '\n') {
			buf[strlen(buf) -1] = '\0';
		}
		if ((pid = fork()) < 0) {
			err_sys("fork error");
		//child	
		//Execute commands entered from standard
		}else if (pid == 0) {
			execlp(buf, buf, (char *)0);
			err_ret("could not execute: %s", buf);
			exit(127);
		}
		//parent
		//wait for the child process to terminate. return to the terminate state
		if ((pid == waitpid(pid, &status, 0)) < 0) {
			err_sys("waitpid error");
		}
		printf("%% ");
	}
	return 0;
}
