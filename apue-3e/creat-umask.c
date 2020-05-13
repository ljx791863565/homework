#include "apue.h"
#include <fcntl.h>
#include <error.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(int argc, const char *argv[])
{
	//rw-------
	umask(0);
	if (creat("foo", RWRWRW) < 0) {
		err_sys("creat error for foo");	
	}
	//rw-rw-rw-
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("bar", RWRWRW) <0) {
		err_sys("creat error for bar");
	}
	return 0;
}
