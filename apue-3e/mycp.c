#include "apue.h"
#include "error.h"
#define BUFFSIZE 4096

//从标准输入读取，写在标准输出 使用shell重定向使得可以不用打开输入输出文件
//进程终止时会自动关系所打开的文件描述符 即标准输入0和标准输出1 使得不用close输入输出文件
//在UNIX中 文本文件和二进制文件没有区别 所以不用区分
//当BUFFSIZE大于等于4096时候 (磁盘块长度4096) 继续增加缓冲区长度对于cpu读写速度没有影响 
int main(int argc, const char *argv[])
{
	int n;
	char buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			err_sys("write error");
		}
	}
	if (n < 0) {
		err_sys("read error");
	}
	return 0;
}
