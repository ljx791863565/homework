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

		x = sdscat(x, "bar");
		test_cond("连接字符串", sdslen(x) == 5 && memcmp(x, "fobar\0", 6) == 0);

		x = sdscpy(x, "a");
		test_cond("sdscpy() 原来的字符串", sdslen(x) == 1 && memcpy(x, "a\0", 2) == 0);

		x = sdscpy(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk");
		test_cond("sdscpy() 一个长字符串", sdslen(x) == 33 && memcpy(x, "xyzxxxxxxxxxxyyyyyyyyyykkkkkkkkkk\0", 34));

		sdsfree(x);
		x = sdscatprintf(sdsempty(), "%d", 123);
		test_cond("sdscatpeintf() 工作", sdslen(x) == 3 && memcmp(x, "123\0", 4) == 0);

		sdsfree(x);
		x = sdstrim(sdsnew("xxciaoyyy"), "xy");
		test_cond("sdstrim() 正确工作", sdslen(x) == 4 && memcmp(x, "ciao\0", 5) == 0);

		y = sdsrange(sdsdup(x), 1, 1);
		test_cond("sdsrange(..., 1, 1)", sdslen(y) == 1 && memcmp(y, "i\0", 2) == 0);

		sdsfree(y);
		y = sdsrange(sdsdup(x), 1, -1);
		test_cond("sdsrange(..., 1, -1)", sdslen(y) == 3 && memcmp(y, "iao\0", 4) == 0);

		sdsfree(y);
		y = sdsrange(sdsdup(x), -2, -1);
		test_cond("sdsrange(..., -2, -1)", sdslen(y) == 2 && memcmp(y, "ao\0", 3) == 0);
		
		sdsfree(y);
		y = sdsrange(sdsdup(x), 2, 1);
		test_cond("sdsrange(..., 2, 1)", sdslen(y) == 0 && memcmp(y, "\0", 1) == 0);

		sdsfree(y);
		y = sdsrange(sdsdup(x), 1, 100);
		test_cond("sdsrange(..., 1, 100)", sdslen(y) == 3 && memcmp(y, "iao\0", 4) == 0);

		sdsfree(y);
		y = sdsrange(sdsdup(x), 100, 100);
		test_cond("sdsrange(..., 100, 100)", sdslen(y) == 0 && memcmp(y, "\0", 1) == 0);

		sdsfree(y);
		sdsfree(x);
		x = sdsnew("foo");
		y = sdsnew("foa");
		test_cond("sdscmp(foo, foa)", sdscmp(x, y) > 0);

		sdsfree(y);
		sdsfree(x);
		x = sdsnew("bar");
		y = sdsnew("bar");
		test_cond("sdscmp(bar, bar)", sdscmp(x, y) == 0);

		sdsfree(y);
		sdsfree(x);
		x = sdsnew("aar");
		y = sdsnew("bar");
		test_cond("sdscmp(aar, bar)", sdscmp(x, y) < 0);

		{
			int oldfree;

			sdsfree(x);
			x = sdsnew("0");
			sh = (void *)(x - (sizeof(struct sdshdr)));
			test_cond("sdsnew() free len buf", sh->free = 0 && sh->len == 1);

			x = sdsMakeRoomFor(x, 1);
			sh = (void *)(x - (sizeof(struct sdshdr)));
			test_cond("sdsMakeRoomFor(x, 1)", sh->free > 0 && sh->len == 1);

			oldfree = sh->free;
			x[1] = '1';
			sdsIncrLen(x, 1);
			test_cond("sdsIncrLen() -- content", x[0] == '0' && x[1] == '1');
			test_cond("sdsIncrLen() -- len", sh->len == 2);
			test_cond("sdsIncrLen() -- free", sh->free == oldfree-1);

			
		}
	}
	test_report();
	return 0;
}
