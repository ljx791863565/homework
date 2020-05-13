/*
 * 3-2 编写一个与3.12节dup2功能相同的函数 要求不使用fcntl函数 且做出错处理
 *
 * #include <unistd.h>
 * int dup2(int fd, int fd2);
 * 复制一个现有的文件描述符fd 可以用fd2指定新描述符的值
 * 如果fd2已经被打开，先将其关闭 如果fd2=fd 返回fd2而不关闭 
 * 否则 fd2的FD_CLOEXEC文件描述符标志被清除 这样在fd2调用exec时
 * 是打开状态 返回的新的文件描述符和参数fd共享同一个文件表项
 *
 * int mydup2(int oldfd, int newfd);
 * 思路 如果不能使用fcntl 那么是在dup函数的基础上来实现，
 * 因为dup(oldfd)每次返回的都是最小的未打开的文件描述符，
 * 因此可以一个一个打开直到dup返回newfd，最后把之前打开的文件描述符全部关闭
 * 在此之前需要检查参数的合法性。
 *
 * - 判断oldfd和newfd是否合法，因为文件描述符是有范围限制；
 * - 判断oldfd是否打开，若未打开则代表oldfd未指向文件表；
 * - 判断oldfd和newfd是否相等，若相等则直接返回newfd；
 * - 判断newfd是否打开，若已打开则关闭newfd；
 * - 反复调用dup，直到返回值等于newfd。
 * 
 * 取得文件描述符的范围：使用库函数getdtablesize()取得文件描述符表的大小N，
 * 则文件描述符范围是[0,N)
 *
 * 如何判断文件描述符是否打开
 * fcntl(fd, F_GETFD) is the canonical cheapest way to check that fd is a
 * valid open file descriptor. If you need to batch-check a lot, using poll
 * with a zero timeout and the events member set to 0 and checking for 
 * POLLVAL in revents after it returns is more efficient.
 *
 * With that said, the operation "check if a given resource handle is still
 * valid" is almost always fundamentally incorrect. After a resource handle
 * is freed (e.g. a fd is closed), its value may be reassigned to the next 
 * such resource you allocate. If there are any remaining references that 
 * might be used, they will wrongly operate on the new resource rather than
 * the old one. Thus, the real answer is probably: If you don't already know
 * by the logic of your program, you have major fundamental logic errors 
 * that need to be fixed.
 *
 * 最权威的方法就是使用fcntl 
 * int fd_is_valid(int fd)
 * {
 *     return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
 * }
 * 因为其在内核中仅仅是解引用而未求他操作 由于可能被信号中断等情况导致fcntl
 * 返回-1 所以要判断errno是否为EBADF (Bad file descriptor)
 */

#include "apue.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
//检查要复制的文件描述符是否合法，如果不合法把errno和errnum设置成EBADF
static inline bool fd_is_vaild(int fd, int *errnum)
{
	//备份原来的errno 防止原来就被设置成为EBADF
	int olderr = errno;
	if (errno == EBADF) {
		errno = 0;
	}
	int newfd = dup(fd);
	if (newfd != -1 || errno != EBADF) {
		close(newfd);
		errno = olderr;
		return true;
	}else {
		errno = EBADF;
		*errnum = EBADF;
		return false;
	}
}

//封装dup 如果dup调用失败将errno保存到errnum地址中
static inline int dup_checked(int oldfd, int *errnum)
{
	int olderr = errno;
	int newfd = dup(oldfd);
	if (newfd == -1) {
		*errnum = errno;
	}
	errno = olderr;
	return newfd;
}

//依次调用dup直到打开到newfd为止  返回newfd 关闭所有打开的fd
static inline int mydup2_imp(int oldfd, int newfd, int *errnum)
{
	close(newfd);
	int *fds = (int*)calloc(newfd+1, sizeof(int));
	int index = 0;
	int res = -1;
	for (index = 0; index < (newfd + 1); index++) {
		fds[index] = dup_checked(oldfd, errnum);
		if (fds[index] == -1 || fds[index] == newfd) {
			res = newfd;
			break;
		}
	}
	printf("close fd: ");
	int i;
	for (i = 0; i < index; i++) {
		printf("%d ", fds[i]);
		close(fds[i]);
	}
	printf("\n");
	free(fds);
	return res;
}

int mydup2(int oldfd, int newfd)
{
	int errnum = errno;
	int res = -1;
	
	//检查文件描述符是否合法
	int tbl_size = getdtablesize();
	if (oldfd < 0 || oldfd >= tbl_size || newfd < 0 || newfd >= tbl_size) {
		errnum = EBADF;
		errno = errnum;
		return res;
	}
	if (!fd_is_vaild(oldfd, &errnum)) {
		errno = errnum;
		return res;
	}
	//判断newfd参数是否等于oldfd 等于直接返回
	if (oldfd == newfd) {
		res = oldfd;
		errno = errnum;
		return res;
	}

	res = mydup2_imp(oldfd, newfd, &errnum);
}

int main(int argc, const char *argv[])
{
	int fd = 20;
	mydup2(STDOUT_FILENO, fd);
	write(fd, "hello\n", 7);
	return 0;
}
