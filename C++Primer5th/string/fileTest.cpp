#include <iostream>
#include <fcntl.h>
#include <unistd.h>
int main()
{
	int fd = open("test.c", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd == -1){
		std:: cout << "oper fial..." << std::endl;
		return -1;
	}

	int ret = write(ret, "hello world", 7);
	if (ret < 0){
		std::cout << "write fial..." << ret << std::endl;
	}
	close(fd);
	return 0;
}
