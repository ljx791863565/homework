#include "zmalloc.h"
#include <stdio.h>
#include "adlist.h"
#include "sds.h"
#include "string.h"
#include "testhelp.h"

int main(int argc, const char *argv[])
{
	/*
	{
	char *p = zmalloc(15*sizeof(char));
	printf("used_memory = %zu\n", zmalloc_size(p));

	char *p2 = zmalloc(11 *sizeof(char));
	printf("used_memory = %zu\n", zmalloc_used_memory());

	list *list;
	list = listCreate();
	}
	*/
	{
		struct sdshdr *sh;
		sds x = sdsnew("foo");
		sds y;
		
		test_cond("创建一个字符串并且获得长度", sdslen(x) == 3 && memcmp(x, "foo\0", 4) == 0);
		
		sdsfree(x);
		x = sdsnewlen("foo", 2);
		test_cond("创建一个规定长度的字符串", sdslen(x) == 2 &&memcmp(x, "fo\0", 3) == 0);

	}
	test_report();
	return 0;
}
