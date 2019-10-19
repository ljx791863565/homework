#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	pid_t childpid;
	int status;
	int retval;

	childpid = fork();

	if (childpid == -1){
		perror("fork()");	//fork失败
		exit(EXIT_FAILURE);
	}else if (childpid == 0){
		puts("in child process");	//子进程
		sleep(100);
		exit(EXIT_SUCCESS);
	}else{
		//waitpid会暂时停止目前进程的执行，直到有信号到来或者子进程结束
		//





		if (0 == (waitpid(childpid, &status, WNOHANG))){
			retval = kill(childpid, SIGKILL);	//向子进程发送kill信号，成功返回0，失败返回-1
			if (retval){
				puts("kill failed");
				perror("kill");
				waitpid(childpid, &status, 0);
			}else{
				printf("%d killed\n", childpid);
			}
		}
	}

	return 0;

}
