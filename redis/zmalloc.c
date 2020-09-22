#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
//已经使用的内存,zmalloc增加 zfree减少 
//真实记录内存的占有大小
static size_t used_memory = 0;
static int zmalloc_thread_safe = 0;			//线程安全标志  1：不安全 0：安全

#include <pthread.h>
pthread_mutex_t used_memory_mutex = PTHREAD_MUTEX_INITIALIZER;

void zlibc_free(void *ptr)
{
	free(ptr);
}

#include <string.h>
#include "zmalloc.h"

#define PREFIX_SIZE (sizeof(size_t))

#define update_zmalloc_stat_add(__n) do { \
	pthread_mutex_lock(&used_memory_mutex); \
	used_memory += (__n); \
	pthread_mutex_unlock(&used_memory_mutex); \
}while(0)

#define update_zmalloc_stat_sub(__n) do { \
	pthread_mutex_lock(&used_memory_mutex); \
	used_memory -= (__n); \
	pthread_mutex_unlock(&used_memory_mutex); \
}while(0)

//分配的空间是8的整数倍，如果不是，补齐
//zmalloc_thread_safe=1时线程不安全 调用thread_mutex来保证资源被安全使用
#define update_zmalloc_stat_alloc(__n) do {	\
	size_t _n = (__n); \
	if (_n&(sizeof(long)-1)) \
		_n += sizeof(long)-(_n&(sizeof(long)-1)); \
	if (zmalloc_thread_safe){ \
		update_zmalloc_stat_add(_n); \
	}else { \
		used_memory += _n; \
	}\
}while(0)

#define update_zmalloc_stat_free(__n) do {	\
	size_t _n = (__n); \
	if (_n&(sizeof(long)-1)) \
		_n += sizeof(long)-(_n&(sizeof(long)-1)); \
	if (zmalloc_thread_safe){ \
		update_zmalloc_stat_sub(_n); \
	}else { \
		used_memory -= _n; \
	}\
}while(0)



//分配内存失败时调用,用于打印log
//oom： out of memory
static void zmalloc_default_oom(size_t size)
{
	fprintf(stderr, "zmalloc: Out of memory trying to allocate %zu bytes\n", size);

	fflush(stderr);

	abort();
}

static void (*zmalloc_oom_handler)(size_t) = zmalloc_default_oom;

//封装了malloc，区别是在开头固定补充了PREFIX_SIZE大小的头，用于保存size大小
//申请的内存8字节对齐	以size_t大小为单位,复用malloc，可以强制转换类型
//					zmalloc返回首地址			补齐字节到8字节对齐
//			8字节	 |	                        |    
//		----------------------------------------------
//		| PREFIX_SIZE|		size				|	 |													
//		|---------------------------------------------
//		保存size大小 |  申请的size大小内存		|	 |
//		|
//		malloc返回首地址
void *zmalloc(size_t size)
{
	void *ptr = malloc(size + PREFIX_SIZE);
	//头部有PREFIX_SIZE大小空间保存了size大小
	//在数据域保存分配数据的实际大小
	*((size_t*)ptr) = size;
	//记录使用的内存大小，并补齐为8的倍数大小内存块
	update_zmalloc_stat_alloc(size + PREFIX_SIZE);
	return (char*)ptr + PREFIX_SIZE;
}

//zcalloc()和zmalloc()具有相同的编程接口，实现功能基本相同，唯一不同之处是zcalloc()会做初始化工作，而zmalloc()不会
//封装calloc，将size大小内存全部清0
void *zcalloc(size_t size)
{
	//参数1表示zcalloc()功能是每次分配 size+PREFIX_SIZE 的空间，并初始化
	void *ptr = calloc(1, size + PREFIX_SIZE);
	if (!ptr)
		zmalloc_oom_handler(size);
	*((size_t*)ptr) = size;
	update_zmalloc_stat_alloc(size + PREFIX_SIZE);
	return (char*)ptr + PREFIX_SIZE;
}

//复用realloc，将ptr扩大为size大小，当然需要8字节对齐
void *zrealloc(void *ptr, size_t size)
{
	void *realptr;
	size_t oldsize;
	void *newptr;

	//对于realloc 传入的指针为NULL 直接调用malloc
	if (ptr == NULL)
		return zmalloc(size);

	//将指针移到头位置
	realptr = (char *)ptr - PREFIX_SIZE;
	oldsize = *((size_t*)realptr);
	newptr = realloc(realptr, size + PREFIX_SIZE);
	if (!newptr)
		zmalloc_oom_handler(size);

	*((size_t *)newptr) = size;

	update_zmalloc_stat_free(oldsize);
	update_zmalloc_stat_alloc(size);
	return (char *)newptr + PREFIX_SIZE;
}

//返回申请到的内存大小+PREFIX_SIZE
size_t zmalloc_size(void *ptr)
{
	//回到malloc申请内存的开始位置
	void *realptr = (char *)ptr - PREFIX_SIZE;
	size_t size = *((size_t*)realptr);
	//此处做size_t大小字节对齐 如果不对齐 补齐
	if (size&(sizeof(long) -1))
		size += sizeof(long) - (size&(sizeof(long) -1));

	return size + PREFIX_SIZE;
}

//释放内存
void zfree(void *ptr)
{
	void *realptr;
	size_t oldsize;
	if (ptr == NULL)
		return;
	//退回到最开始的地址，才能正确调用free()不产生内存泄漏
	realptr = (char*)ptr - PREFIX_SIZE;
	//保存住size的大小
	oldsize = *((size_t*)realptr);
	//更改used_memory大小
	update_zmalloc_stat_free(oldsize + PREFIX_SIZE);
	//释放掉这一块内存
	free(realptr);
}

//复用strdup。申请一块内存并且将字符串copy在这块内存中
char *zstrdup(const char *str)
{
	size_t l = strlen(str) +1;
	char *p = zmalloc(l);
	memcpy(p, str, l);
	return p;
}

//返回此时系统zmalloc使用的总内存大小
size_t zmalloc_used_memory(void)
{
	size_t um;
	if (zmalloc_thread_safe){
		pthread_mutex_lock(&used_memory_mutex);
		um = used_memory;
		pthread_mutex_unlock(&used_memory_mutex);
	}else {
		um = used_memory;
	}

	return um;
}

//设置使用多线程
void zmalloc_enable_thread_safeness(void)
{
	zmalloc_thread_safe = 1;
}

//修改zmalloc_oom_handler函数
void zmalloc_set_omm_handler(void (*omm_handler)(size_t))
{
	zmalloc_oom_handler = omm_handler;
}

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//获取当前进程实际所驻留在内存中的空间大小，即不包括被交换(swap)出去的空间。RSS：Resident Set Size
//当前进程的 /proc/<pid>/stat [ pid表示当前进程id ]文件中进行检索。该文件的第24个字段是RSS的信息，它的单位是pages(内存页的数目)。
size_t zmalloc_get_rss()
{
	//查询内存页大小
	//sys\unistd.h 
	//#define _SC_PAGESIZE                      8
	int page = sysconf(_SC_PAGESIZE);
	size_t rss;
	char buf[4096];
	char filename[256];
	int fd, count;
	char *p, *x;

	snprintf(filename, 256, "/proc/%d/stat", getpid());
	if ((fd = open(filename, O_RDONLY)) == -1)
		return 0;
	if (read(fd, buf, 4096) <= 0){
		close(fd);
		return -1;
	}
	close(fd);

	p = buf;
	count = 23;
	//循环用' '切割读到的文件信息24次,因为RSS信息就在第24位
	while (p && count--){
		//strchr 在字符串p中查询空格字符，如果找到就把空格所在位置的字符指针返回并赋值给p，找不到会返回NULL指针
		p = strchr(p, ' ');
		if (p)
			p++;
	}
	if (!p)
		return -1;
	x = strchr(p, ' ');
	if (!x)
		return -1;
	*x = '\0';

	//string to long long 10表示进制 前两个参数表示要转换的字符串起始和终止地址
	rss = strtoll(p, NULL, 10);
	//rss获得的实际上是内存页的页数，page保存的是每个内存页的大小(单位字节),相乘之后就表示RSS实际的内存大小了
	rss *= page;
	return rss;
}

//提供内存碎片率的指标，直接用驻留在物理内存中的内存/除以分配的总物理内存，得到一个所谓的碎片率， 实际留在物理内存中的除以总分配的。
float zmalloc_get_fragmentation_ratio(void)
{
	return (float)zmalloc_get_rss() / zmalloc_used_memory();
}

//获取/proc/pid/smaps中Private_Dirty的字节大小
//Rss=Shared_Clean + Shared_Dirty + Private_Clean + Private_Dirty
//        其中：
//Shared_Clean:多进程共享的内存，且其内容未被任意进程修改 
//Shared_Dirty:多进程共享的内存，但其内容被某个进程修改 
//Private_Clean:某个进程独享的内存，且其内容没有修改 
//Private_Dirty:某个进程独享的内存，但其内容被该进程修改
//
//其实所谓的共享的内存，一般指的就是Unix系统中的共享库（.so文件）的使用
//共享库又叫动态库（含义同Windows下的.dll文件），它只有在程序运行时才被装入内存。
//这时共享库中的代码和数据可能会被多个进程所调用，于是就会产生共享（Shared）与私有（Private）、干净（Clean）与脏（Dirty）的区别了。
//此外该处所说的共享的内存除了包括共享库以外，还包括System V的IPC机制之一的共享内存段（shared memory）
size_t zmalloc_get_private_dirty(void)
{
	char line[1024];
	size_t pd = 0;
	//self/ 表示的是当前进程的状态目录,而smaps文件中记录着该进程的详细映像信息
	FILE *fp = fopen("/proc/self/smaps", "r");
	if (!fp)
		return 0;
	//逐行读取
	while (fgets(line, sizeof(line), fp) != NULL){
		if (strncmp(line, "Private_Dirty:", 14) == 0){
			//定位到字符k位置
			char *p = strchr(line, 'k');
			if (p){
				//截断。获得c风格字符串
				*p = '\0';
				//strtol()无视空格可以将字符串转换成int类型
				// Private_Dirty:         0 kB
				// kB转换为B单位
				pd += strtol(line+14, NULL, 10) * 1024;
			}
		}
	}
	fclose(fp);
	return pd;
}

#if defined(HAVE_PROC_SMAPS)
size_t zmalloc_get_smap_bytes_by_field(char *field) {
	char line[1024];
	size_t bytes = 0;
	FILE *fp = fopen("/proc/self/smaps","r");
	int flen = strlen(field);

	if (!fp) return 0;
	while(fgets(line,sizeof(line),fp) != NULL) {
		if (strncmp(line,field,flen) == 0) {
			char *p = strchr(line,'k');
			if (p) {
				*p = '\0';
				bytes += strtol(line+flen,NULL,10) * 1024;
			}
		}
	}
	fclose(fp);
	return bytes;
}
#else
size_t zmalloc_get_smap_bytes_by_field(char *field) {
	((void) field);
	return 0;
}
#endif
