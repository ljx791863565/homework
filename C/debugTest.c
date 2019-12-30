#include <stdio.h>

//#define DEBUG

int main()
{
	printf("hello world\n");
#ifdef DEBUG
	printf("this is debug...\n");
#endif
	return 0;

}
