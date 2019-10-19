#include <iostream>
#include <unistd.h>
int main()
{
	pid_t pid;

	std::cout << sizeof(pid_t) << std::endl; 
/*
	pid = fork();
	if (pid < 0){
		std::cout << "fork() error" << std::endl;
	}else if(pid == 0){
		std::cout << "this is child" << " " << getppid() << std::endl;
	}else {
		std::cout << "this is father" << " " << getpid() << std::endl;
	}
*/
	execl("/bin/ls", "ls", "-l", "/home/nick/homework", NULL);
	return 0;
}
