#include <stdio.h>

#define dmsprint(fmt, arg...) do{printf(fmt, ##arg);}while(0)

int main(int argc, const char *argv[])
{
	dmsprint("%s", "hello world\n");
	return 0;
}
