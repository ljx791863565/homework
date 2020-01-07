#ifndef _SDS_H_
#define _SDS_H_ 

#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

//sds 类型
typedef char* sds;

struct sdshdr{
	// buf 已占用长度
	int len;
	// buf 剩余可用长度
	int free;
	//实际保存字符串的buf
	char buf[];
};

//返回 sds buf的已占用长度
static inline size_t sdslen(const sds s)
{
	struct sdshdr *sh = (void*)(s - (sizeof(struct sdshdr)));
	return sh->len;
}

sds sdsnewlen(const void *init, size_t initlen);
sds sdsnew(const char *init);
sds sdsempty();
size_t sdslen(const sdd s);
sds sdsdup(const sds s);
void sdsfree(sds s);
size_t sdsavail(const sds s);
sds sdsgrowzero(sds s, size_t len);
sds sdscatlen(sds s, const void *t, size_t len);
sds sdscat(sds s, const char *t);
sds sdscatsds(sds s, const sds t);
sds sdscpylen(sds s, const char *t, size_t len);
sds sdscpy(sds s, const char *t);

sds sdscatvprintf(sds s, const char *fmt, va_list ap);
sds sdscatpeintf(sds s, const char *fmt, ...);

sds sdstrim(sds s, const char *cast);
sds sdsrange(sds s, int start, int end);
void sdsupdatelen(sds s);
void sdsclear(sds s);
int sdscmp(const sds s1, const sds s2);
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);
void sdsfreesplitres(sds *token, int count);
void sdstoupper(sds s);
void sdstolower(sds s);
sds sdsfromlonglong(long long value);
sds sdscatrepr(sds s, const char *p, size_t len);
sds *sdssplitargs(const char *line, int argc);
void sdssplitargs_free(sds *argv, int argc);
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);

sds sdsMakeRoomFor(sds s, size_t addlen);
void sdsIncrLen(sds s, int incr);
sds sdsRemoveFreeSpace(sds s);
size_t sdsAllocSize(sds s);
#endif	//_SDS_H_
