#include "apue.h"
#include "error.h"

int main(int argc, const char *argv[])
{
	int c;
	while ((c = getc(stdin)) != EOF) {
		if (putc(c, stdout) == EOF) {
			err_sys("output error");
		}
	}
	if (ferror(stdin)) {
		err_sys("input error");
	}
	return 0;
}
