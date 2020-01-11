#ifndef _SDS_H_
#define _SDS_H_ 

//1Mb空间用于指导分配内存
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

/*
 * 对比c字符串
 * sds可以高效执行长度计算 (strlen)
 * sds可以高效执行追加操作 (append)
 * 二进制安全 ？
 * 追加操作时加快操作速度 降低内存分配次数 代价就是占有了更多的内存 并且这些内存不会被主动释放
 */

//sds 类型 Simple Dynamic String
typedef char* sds;

struct sdshdr{
	// buf 已占用长度
	int len;
	// buf 剩余可用长度
	int free;
	//实际保存字符串的buf
	//在c89中要定义保存字符的空间 必须 char array[n]提前规定数组大小n n需要为常数 不然编译不通过 
	//或者使用指针 char *p;
	//但是如果使用指针时，要操作这个内存需要越过该指针 4字节的空间 很麻烦 
	//使用数组时 又需要提前确定大小 不符合我们的需要
	//flexible array member
	//c99伸缩性数组成员 c89不支持 编译时需要 -std=c99
	//buf长度为0 不占用sdshdr空间 sizeof(sdshdr) = 8
	//同时 struct sdshdr *sh->buf就直接指向其所被分配到的空间
	//实际操作 ：struct sdshdr *sh = malloc(sizeof(struct sdshdr) + 100*sizeof(char));
	//这样为buf分配到了100个char大小的内存
	//用sh->buf即可直接访问到该地址
	//即使分配过内存后sizeof(struct sdshdr)依旧为8 因为在定义结构体时buf就不属于这个结构体 是属于编外元素 不占用结构体编制
	//只有在使用的时候，buf才作为sdshdr的一员被使用
	//释放时直接 free(sh->buf)即可
	//buf只作为一个符号地址存在 必须是结构体最后一个成员 且不可以只有这一个成员 必须前面有至少一个其他成员
	//同时柔性数组成员不仅可以用于char 也可以是int等数组
	char buf[];
};

/* -------------------------------两个常用inline函数------------------------------------ */
//inline C99标准
//inline关键字在GCC参考文档中仅有对其使用在函数定义(Definition)上的描述 而没有提到其是否能用于函数声明(Declare)
//inline关键字仅仅是建议编译器做内联展开处理，而不是强制
//inline这个关键字会给编译器这样一种建议：不要进行实际的函数调用而只是将这段代码插入到进行调用的地方
//这个关键词对于编译器来说仅仅只是一种"hint" 也即提示 
//编译器有可能会忽略它 并且即使在没有加关键词inline的情况下也会对某些函数进行"inline"
//
//GCC的static inline定义很容易理解：你可以把它认为是一个static的函数 加上了inline的属性。
//这个函数大部分表现和普通的static函数一样 只不过在调用这种函数的时候 gcc会在其调用处将其汇编码展开编译而不为这个函数生成独立的汇编码
//
//在C语言中 static主要适用于指明范围的 static意味着这个变量或者函数只能在相同的"translation unit"中使用
//"translation unit"是指一个目标文件生成所需要的"ultimate input" 也即产生这个目标文件所需要的输入文件
//在通常情况下也把"translation unit"叫做"compliation unit" 即编译单元
//一个"translation unit"大概会包含经过C预处理器所处理过的源文件 这意味着#include中的文件会被包含近来 #ifdef也会被包含近来 宏也是
//即static修饰的变量或函数在 translation unit外部是不可见的 只能在内部可见和使用
//返回 sds buf的已占用长度
//static inline 函数直接在.h文件中定义 只在被引用代码中可见
//引用此.h也不会造成重复定义
static inline size_t sdslen(const sds s)
{
	struct sdshdr *sh = (void*)(s - (sizeof(struct sdshdr)));
	return sh->len;
}

//返回sds 的buf 可用长度
static inline size_t sdsavail(const sds s)
{
	struct sdshdr *sh = (void *)(s - (sizeof(struct sdshdr)));
	return sh->free;
}
/* --------------------------------------------------------------------------------------- */

/* ------------------------------------基本API-------------------------------------------- */
sds sdsnewlen(const void *init, size_t initlen);			//创建一个指定长度的sds 接收一个c字符串作为初始化值	O(N)
sds sdsnew(const char *init);								//根据给定的c字符串创建相应的sds O(N)
sds sdsempty();												//创建一个只包含空白字符串""的sds O(N)
size_t sdslen(const sds s);									//返回给定sds的长度 O(1)
sds sdsdup(const sds s);									//创建一个sds副本 O(N)
void sdsfree(sds s);										//释放给定sds O(N)
size_t sdsavail(const sds s);
sds sdsgrowzero(sds s, size_t len);							//将sds的buf扩展到指定长度 无内容内存用\0填充 O(N)
sds sdscatlen(sds s, const void *t, size_t len);			//对sds的buf按给定长度扩展 并将给定字符串追加到末尾 O(N)
sds sdscat(sds s, const char *t);							//将一个字符串追加到sds末尾 O(N)
sds sdscatsds(sds s, const sds t);							//将一个sds (t) 追加到sds (s) 末尾 O(N)
sds sdscpylen(sds s, const char *t, size_t len);			//将一个字符串的部分内容复制到sds中 需要时要对sds进行扩展 O(N)
sds sdscpy(sds s, const char *t);							//将一个c字符串复制到sds O(N)
void sdsupdatelen(sds s);									//更新给定sds对应sdshdr结构的free和len属性 O(1)
void sdsclear(sds s);										//清除给定sds内容 初始化为"" O(1)
/* --------------------------------------------------------------------------------------- */

/* ----------------------------基本字符串API的sds版本------------------------------------- */
sds sdscatvprintf(sds s, const char *fmt, va_list ap);		//字符串格式化输出
sds sdscatprintf(sds s, const char *fmt, ...);				//字符串格式化输入 类似sprintf
sds sdstrim(sds s, const char *cast);						//字符串trim
sds sdsrange(sds s, int start, int end);					//字符串截取
int sdscmp(const sds s1, const sds s2);						//字符串比较
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);	//字符串分割
void sdsfreesplitres(sds *token, int count);				//释放分割完的token数组
void sdstoupper(sds s);										//统一转换小写
void sdstolower(sds s);										//统一转换大写
sds sdsfromlonglong(long long value);						//将一个long long 类型数字转换为字符串
sds sdscatrepr(sds s, const char *p, size_t len);			//添加引用字符串
sds *sdssplitargs(const char *line, int *argc);				//参数解析
void sdssplitargs_free(sds *argv, int argc);				//释放解析完的参数数组
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);	//字符串替换
/* --------------------------------------------------------------------------------------- */

/* -----------------------------暴露给用户的低级API--------------------------------------- */
sds sdsMakeRoomFor(sds s, size_t addlen);					//对sds的sdshdr结构的buf属性进行扩展 O(N)
void sdsIncrLen(sds s, int incr);							//对给定sds的buf右端进行 扩展expand 或者修剪trim O(1)
sds sdsRemoveFreeSpace(sds s);								//在不改动buf情况下将buf多余空间清除 O(N)
size_t sdsAllocSize(sds s);									//计算给定sds的buf所占用内存总数 O(1)
/* --------------------------------------------------------------------------------------- */
#endif	//_SDS_H_
