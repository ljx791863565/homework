#include "zmalloc.h"
#include <stdio.h>
int main(int argc, const char *argv[])
{
	char *p = zmalloc(15*sizeof(char));
	printf("used_memory = %zu\n", zmalloc_size(p));

	char *p2 = zmalloc(11 *sizeof(char));
	printf("used_memory = %zu\n", zmalloc_used_memory());
	return 0;
}
