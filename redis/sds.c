#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "sds.h"
#include "zmalloc.h"

//创建一个指定长度的sds
//如果给定了初始化值 将其复制到sds中
//O(N)
sds sdsnewlen(const void *init, size_t initlen)
{
	struct sdshdr *sh;

	//init存在
	if (init) {
		sh = zmalloc(sizeof(struct sdshdr) + initlen +1);
	}else {
		sh = zcalloc(sizeof(struct sdshdr) + initlen +1);
	}

	//内存不足 分配失败
	if (sh == NULL)
		return NULL;
	
	sh->len = initlen;
	sh->free = 0;

	//给定了 init 并且 initlen不为0
	//复制init到 sh->buf
	if (initlen && init){
		memcpy(sh->buf, init, initlen);
	}
	//字符串的结束符
	sh->buf[initlen] = '\0';

	//返回buf
	return (char *)sh->buf;
}

//创建一个只包含空字符串""的sds
//O(N)
sds sdsempty(void)
{
	return sdsnewlen("", 0);
}

//只给定初始化init创建sds
//如果init为 NULL 创建一个只包含""的sds
//O(N)
sds sdsnew(const char *init)
{
	size_t initlen = (init == NULL) ? 0 : strlen(init);
	return sdsnewlen(init, initlen);
}

//复制给定的sds
//O(N)
sds sdsdup(const sds s)
{
	return sdsnewlen(s, sdslen(s));
}

//释放sds对应的 sdshdr结构的内存
//如果给定sds为 NULL 不必释放
//O(N)
void sdsfree(sds s)
{
	if (s == NULL)
		return;
	//从sdshdr结构体开始位置释放
	zfree(s-sizeof(struct sdshdr));
}

//更新给定sds所对应的sdshdr结构中的 free和len属性
//O(N)
void sdsupdatelen(sds s)
{
	//指向s头
	struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
	int reallen = strlen(s);
	sh->free += (sh->len - reallen);
	sh->len = reallen;
}

//清除给定 sds中buf的内容 让它只包含一个'\0'
//O(1)
void sdsclear(sds s)
{
	struct sdshdr *sh = (void *)(s - sizeof(struct sdshdr));
	
	//未使用大小free为原来使用的大小len 表示这个sds没有任何字符
	sh->free += sh->len;
	//使用的大小len为0
	sh->len = 0;
	sh->buf[0] = '\0';
}

//对sds的buf进行扩展 扩展大小不少于addlen
//O(N)
sds sdsMakeRoomFor(sds s, size_t addlen)
{
	struct sdshdr *sh = (void *)(s - sizeof(struct sdshdr));
	struct sdshdr *newsh;

	size_t free = sdsavail(s);
	size_t len, newlen;

	//free空间足够存放下addlen大小字符串 不需要扩展
	if (free >= addlen)
		return s;
	//目前buf长度
	len = sdslen(s);
	//新buf长度
	newlen = (len + addlen);

	//目前版本中 SDS_MAX_PREALLOC =1024*1024
	//即如果目标buf的大小小于 1M
	//分配2倍的newlen
	//如果大于1M
	//分配1M+newlen大小
	if (newlen < SDS_MAX_PREALLOC){
		newlen *= 2;
	}else {
		newlen += SDS_MAX_PREALLOC;
	}
	
	//根据需要大小分配到内存
	newsh = zrealloc(sh, sizeof(struct sdshdr) + newlen +1);

	if (newsh == NULL)
		return NULL;

	//分配内存成功 确定剩余可用大小
	newsh->free = newlen - len;
	return newsh->buf;
}

//在不改动sds 的buf 内容情况下 将buf中多余的空间释放 即类似收缩内存
//在执行这个收缩操作后 还需要在sds后追加 必须要先执行分配内存操作 因为执行这个操作后sds不会有任何多余的空间
//O(N)
sds sdsRemoveFreeSpace(sds s)
{
	struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
	
	//重新分配内存 大小为只能放下len 不需要的直接会被zrealloc掉
	//不保留任何多余的空间
	sh = zrealloc(sh, sizeof(struct sdshdr) + sh->len +1);
	sh->free = 0;

	return sh->buf;
}

//计算给定 sds 的buf 内存总长度
//包括free和len
//O(1)
size_t sdsAllocSize(sds s)
{
	struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
	//这个1是用来保存'\0'的 在每一个分配内存的sds中都存在
	//sizeof(*sh)即 sizeof(struct sdshdr) 一个sdshdr头 固定大小8
	return sizeof(*sh) + sh->len + sh->free + 1;
}

//对s指向的空间进行扩展或剪裁
//当incr为正数时 s->len+= incr  s->buf后多incr个空间并且值为随机
//hdr = sdshdr {
//		len = 10;
//		free = 10;
//		buf = "hello meto\0"
//	}
//	sdsIncrLen(s->buf, 2);
//hdr = sdshdr {
//		len = 12;
//		free = 8;
//		buf = "hello meto\0[ ]\0";		//这里的[ ]可以表示任意2byte内容
//	}
//
//当incr为负数时 s->len-= incr  s->buf从右向左减少incr大小的内容
//hdr = sdshdr {
//		len = 10;
//		free = 10;
//		buf = "hello meto\0"
//	}
//	sdsIncrLen(s->buf, -2);
//hdr = sdshdr {
//		len = 8;
//		free = 12;
//		buf = "hello me\0";		//这里的[ ]可以表示任意2byte内容
//	}
//
//	O(1)
void sdsIncrLen(sds s, int incr)
{
	struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));

	assert(sh->free >= incr);
	sh->len += incr;
	sh->free -= incr;

	//如果free全部使用完了不可以进行裁剪
	assert(sh->free >= 0);
	s[sh->len] = '\0';
}

//将 sds 的buf 扩展到给定的len大小 扩展的内容用\0填充
//O(N)
sds sdsgrowzero(sds s, size_t len)
{
	struct sdshdr *sh = (void *) (s - (sizeof(struct sdshdr)));
	size_t totlen, curlen;
	curlen = sh->len;

	if (len <= curlen)
		return s;

	s = sdsMakeRoomFor(s, len - curlen);
	if (s == NULL){
		return NULL;
	}

	sh = (void *)(s - (sizeof(struct sdshdr)));
	//使用0填充扩展后的空位 确保没有垃圾数据
	memset(s+curlen, 0, (len - curlen+1));

	//扩展后的大小加上原来剩余的大小
	totlen = sh->len + sh->free;
	//正式确认len为扩展后的len大小
	sh->len = len;
	sh->free = totlen - sh->len;

	return s;
}

//按指定len扩展sds 并将t拼到s末尾
//O(N）
sds sdscatlen(sds s, const void *t, size_t len)
{
	struct sdshdr *sh;
	size_t curlen = sdslen(s);

	s = sdsMakeRoomFor(s, len);
	if (s == NULL)
		return NULL;

	//拼接到末尾
	memcpy(s+curlen, t, len);

	//更新 len free 属性
	sh = (void *)(s - (sizeof(struct sdshdr)));
	sh->len = curlen +len;
	sh->free = sh->free - len;

	s[curlen+len] = '\0';

	return s;
}

//将一个c字符串拼接到sds末尾
//O(N)
sds sdscat(sds s, const char *t)
{
	return sdscatlen(s, t, strlen(s));
}

//将一个sds拼接到另一个sds末尾
sds sdscatsds(sds s, const sds t)
{
	return sdscatlen(s, t, sdslen(t));
}

//将t字符串的前len字节复制到s
//如果s的buf长度不够 扩展buf长度到 >= len
//O(N)
sds sdscpylen(sds s, const char *t, size_t len)
{
	struct sdshdr *sh = (void *)(s -(sizeof(struct sdshdr)));
	
	//总共的空间大小 包括使用的和free的
	size_t totlen = sh->free + sh->len;

	//需要扩容 大小设置为超出原使用的大小
	if (totlen < len){
		s = sdsMakeRoomFor(s, len - sh->len);
		if (s == NULL)
			return NULL;

		sh = (void *)(s - (sizeof(struct sdshdr)));
		//扩容后的空间大小
		totlen = sh->free + sh->len;
	}
	//复制
	memcpy(s, t, len);
	s[len] = '\0';

	//更新s属性
	sh->len = len;
	sh->free = totlen - len;

	return s;
}

//将一个字符串t复制到s
//O(N)
sds sdscpy(sds s, const char *t)
{
	return sdscpylen(s, t, strlen(t));
}

//底层格式化追加到s函数
sds sdscatvprintf(sds s, const char *fmt, va_list ap)
{
	va_list cpy;
	char *buf, *t;
	size_t buflen = 16;

	while (1) {
		buf = zmalloc(buflen);
		if (buf == NULL)
			return NULL;

		buf[buflen -2] = '\0';
		va_copy(cpy, ap);
		//这里将cpy参数格式化16个字符到buf中
		//如果大于14位 即\0被覆盖 重新申请buflen*2空间
		vsnprintf(buf, buflen, fmt, cpy);
		if (buf[buflen - 2] != '\0'){
			zfree(buf);
			buflen *= 2;
			continue;
		}
		break;
	}
	t = sdscat(s, buf);
	zfree(buf);
	return t;
}

//打印函数底层实现
sds sdscatprintf(sds s, const char *fmt, ...)
{
	va_list ap;
	char *t;
	va_start(ap, fmt);
	t = sdscatvprintf(s, fmt, ap);
	va_end(ap);

	return t;
}

//sds中删除所有在cest中出现的字符
//比如 sds串为 "hello worldhe\0" cest串为 "he"
//删除后结果为 "llo world\0"
//len 原先是13 删除后是9
//free 假设原先是10 删除后是14 少了4个字符所以多了4个free
//O(N^2)
sds sdstrim(sds s, const char *cest)
{
	struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
	char *start, *end, *sp, *ep;
	size_t len;

	//指向s字符串开始
	sp = start = s;
	//指向s字符串结尾
	ep = end = s+sdslen(s) -1;

	//从左向右逐个字符匹配cest串
	//strchr返回在串中找到第一个出现的指针 没找到返回NULL
	//当出现不再是cest字符串的字符时 循环结束
	while (sp <= end && strchr(cest, *sp))
		sp++;
	//从右向左逐个字符匹配cest串
	while (ep > start && strchr(cest, *ep))
		ep--;
	
	//如果此时sp在ep右边 说明没有在s中cest字符串 不需要删除
	//如果在左边 len是删除后剩下的s字符串长度
	len = (sp > ep) ? 0 : ((ep - sp) +1);
	//不是s串开头
	if (sh->buf != sp){
		//将sp开始的len个字节copy到buf开始位置 
		//先将len个字节保存到缓冲区 再从缓冲区复制到目的地址 即使是内存重叠也可以正确处理
		memmove(sh->buf, sp, len);
	}

	//更新删除后的s属性
	sh->buf[len] = '\0';
	sh->free = sh->free + (sh->len - len);
	sh->len = len;

	return s;
}

//取s的start到end范围的字符
sds sdsrange(sds s, int start, int end)
{
	struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
	size_t newlen, len;
	len = sdslen(s);

	if (len == 0) {
		return s;
	}
	//如果start/end为负数 表示从右向左移动-srart/-end长度 否则为0
	if (start < 0) {
		start = len +start;
		if (start < 0){
			start = 0;
		}
	}
	if (end < 0){
		end = len + end;
		if (end < 0){
			end = 0;
		}
	}

	newlen = (start > end) ? 0 : (end - start)+1;
	if (newlen != 0) {
		if (start >= (signed)len) {
			newlen = 0;
		} else if (end >= (signed)len){
			end = len -1;
			newlen = (start > end) ? 0 :(end - start)+1;
		}
	} else {
		start = 0;
	}

	if (start && newlen) {
		memmove(sh->buf, sh->buf+start, newlen);
	}

	sh->buf[newlen] = '\0';
	sh->free = sh->free + (sh->len - newlen);
	sh->len = newlen;
	return s;
}

//将sds全部转换为小写字母
void sdstolower(sds s)
{
	int len = sdslen(s);
	int j;
	for (j = 0; j < len; j++){
		s[j] = tolower(s[j]);
	}
}

//将sds全部转换为大写字母
void sdstoupper(sds s)
{
	int len = sdslen(s);
	int j;
	for (j = 0; j < len; j++){
		s[j] = toupper(s[j]);
	}
}

//比较s1 s2两个sds
//返回值如果大于0 表示s1 > s2
//返回值如果小于0 表示s1 < s2
//返回0表示两个sds完全相等
int sdscmp(const sds s1, const sds s2)
{
	size_t len1, len2, minlen;
	int cmp;
	len1 = sdslen(s1);
	len2 = sdslen(s2);
	minlen = (len1 < len2) ? len1 : len2;
	cmp = memcmp(s1, s2, minlen);
	if (cmp == 0) {
		return len1 - len2;		
	}
	return cmp;
}

//用sep分割符分割字符串s 返回分割完后的sds数组 count用于计数
//当内存不足时 s长度为0时 分隔符长度为0时 返回NULL
//sep可以是多个字符组成的字符串
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count)
{
	int elements = 0, slots = 5, start = 0, j;
	sds *tokens;

	if (seplen < 1 || len < 0){
		return NULL;
	}

	tokens = zmalloc(sizeof(sds) * slots);
	if (tokens == NULL){
		return NULL;
	}
	if (len == 0){
		*count = 0;
		return tokens;
	}
	for (j = 0; j < (len - (seplen -1)); j++){
		if (slots < elements + 2){
			sds *newtokens;
			slots *=2;
			newtokens = zrealloc(tokens, sizeof(sds) *slots);
			if (newtokens == NULL){
				goto cleanup;
			}
			tokens = newtokens;
		}

		if ((seplen == 1 && *(s+j) == sep[0]) || (memcmp(s+j, sep, seplen) == 0)){
			tokens[elements] = sdsnewlen(s+start, j-start);
			if (tokens[elements] == NULL){
				goto cleanup;
			}
			elements++;
			start = j+seplen;
			j = j+seplen-1;
		}
	}
	tokens[elements] = sdsnewlen(s+start, len-start);
	if (tokens[elements] == NULL){
		goto cleanup;
	}
	elements++;
	*count = elements;
	return tokens;

cleanup:
	{
		int i;
		for (i=0; i<elements; i++){
			sdsfree(tokens[i]);
		}
		zfree(tokens);
		*count = 0;
		return NULL;
	}
}

void sdsfreespliters(sds *tokens, int count)
{
	if (!tokens){
		return ;
	}
	while (count--){
		sdsfree(tokens[count]);
	}

	zfree(tokens);
}

//将一个long long 值转换成sds
sds sdsfromlonglong(long long value)
{
	char buf[32], *p;
	unsigned long long v;

	//转换时必须保证是正数
	v = (value < 0) ? -value : value;
	//将p指向数组最后一个地址
	p = buf +31;
	do {
		//从最右边开始逐个将数字转换成字符
		*p-- = '0' + (v%10);
		v /= 10;
	} while(v);
	
	//如果value是负数 需要在最前面加上'-'
	if (value < 0){
		*p-- = '-';
	}
	//因为前面p赋值后还执行了--操作 所以需要将p向右移动一位来保证p指向的就是字符串开始位置
	p++;
	//p-buf表示此时p指针到buf[0]距离
	//总长度减去这个距离表示这个转换后的字符串len
	return sdsnewlen(p, 32-(p-buf));
}

sds sdscatrepr(sds s, const char *p, size_t len)
{
	//在s后追加一个"
	s = sdscatlen(s, "\"", 1);
	while (len--){
		switch (*p)
		{
			case '\\':
			case '"':
				s = sdscatprintf(s, "\\%c", *p);
				break;
			case '\n':
				s = sdscatlen(s, "\\n", 2);
				break;
			case '\r':
				s = sdscatlen(s, "\\r", 2);
				break;
			case '\t':
				s = sdscatlen(s, "\\t", 2);
				break;
			case '\a':
				s = sdscatlen(s, "\\a", 2);
				break;
			case '\b':
				s = sdscatlen(s, "\\b", 2);
				break;
			default:
				//判断*p是否可打印 0x20 - 0x7e
				if (isprint(*p)){
					s = sdscatprintf(s, "%c", *p);
				} else {
					s = sdscatprintf(s, "\\x%02x", (unsigned char)*p);
				}
				break;
		}
		p++;
	}
	return sdscatlen(s, "\"", 1);
}

//判断是不是16进制数 
//是 ：返回非0
//不是：返回0
int is_hex_digit(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

//二进制数转换成十进制整数
int hex_digit_to_int(char c)
{
	switch (c)
	{
		case '0' :
			return 0;
		case '1' :
			return 1;
		case '2' :
			return 2;
		case '3' :
			return 3;
		case '4' :
			return 4;
		case '5' :
			return 5;
		case '6' :
			return 6;
		case '7' :
			return 7;
		case '8' :
			return 8;
		case '9' :
			return 9;
		case 'a' :
		case 'A' :
			return 10;
		case 'b' :
		case 'B' :
			return 11;
		case 'c' :
		case 'C' :
			return 12;
		case 'd' :
		case 'D' :
			return 13;
		case 'e' :
		case 'E' :
			return 14;
		case 'f' :
		case 'F' :
			return 15;
		default :
			return 0;
		
	}
}

sds *sdssplitargs(const char *line, int *argc)
{
	const char *p = line;
	char *current = NULL;
	char **vector = NULL;
	*argc = 0;

	while (1) {
		//跳过空格
		while (*p && isspace(*p)){
			p++;
		}
		if (*p) {
			int inq = 0;		//"标识
			int insq = 0;		//'标识
			int done = 0;		//停止标识 如果不为0 不进行

			if (current == NULL){
				//申请一个空的sds
				current = sdsempty();
			}
			while (!done) {
				if (inq) {
					// \x02 保证\后是16进制数
					if (*p == '\\' && *(p+1) == 'x' && is_hex_digit(*(p+2)) && is_hex_digit(*(p+3))){
						unsigned char byte;
						//将16进制数转换成10进制整数
						byte = (hex_digit_to_int(*(p+2))*16 + hex_digit_to_int(*(p+3)));
						current = sdscatlen(current, (char*)&byte ,1);
						p += 3;
					}else if (*p == '\\' && *(p+1)){
						char c;
						p++;
						switch (*p)
						{
							case 'n' :
								c = '\n';
								break;
							case 'r' :
								c = '\r';
								break;
							case 't' :
								c = '\t';
								break;
							case 'a' :
								c = '\a';
								break;
							case 'b' :
								c = '\b';
								break;
							default :
								c = *p;
								break;
						}
						current = sdscatlen(current, &c, 1);
					}else if (*p == '"') {
						if (*(p+1) && !isspace(*(p+1))){
							goto err;
						}
						done = 1;
					}else if (!*p) {
						goto err;
					}else {
						current = sdscatlen(current, p, 1);
					}
				} else if (insq) {
					// \'
					if (*p == '\\' && *(p+1) == '\''){
						p++;
						current = sdscatlen(current, "'", 1);
					}else if (*p == '\''){
						//p+1有值且不是空格
						if (*(p+1) && !isspace(*(p+1))){
							goto err;
						}
						done = 1;
					}else if (!*p){
						goto err;
					}else {
						current = sdscatlen(current, p, 1);
					}
				}else {
					switch (*p) {
						case ' ' :
						case '\n' :
						case '\r' :
						case '\t' :
						case '\0' :
							done =1;
							break;
						case '"' :
							inq = 1;
							break;
						case '\'' :
							insq = 1;
							break;
						default :
							current = sdscatlen(current, p, 1);
							break;
					}
				}

				if (*p){
					p++;
				}
			}
			vector = zrealloc(vector, ((*argc) + 1) * sizeof(char *));
			vector[*argc] = current;
			(*argc)++;
			current = NULL;
		} else {
			return vector;
		}
	}
	
err:
	while ((*argc)--){
		sdsfree(vector[*argc]);
	}
	zfree(vector);
	if (current){
		sdsfree(current);
	}
	return NULL;
}

void sdssplitargs_free(sds *argv, int argc)
{
	int i;
	for (i = 0; i < argc; i++){
		sdsfree(argv[i]);
	}
	zfree(argv);
}

sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen)
{
	size_t j, i, len = sdslen(s);

	for (j = 0; j < len; j++){
		for (i = 0; i < setlen; i++){
			if (s[j] == from[i]) {
				s[j] = to[i];
				break;
			}
		}
	}

	return s;
}
