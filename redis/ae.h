#ifndef _AE_H_
#define _AE_H_

#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0
#define AE_READABLE 1
#define AE_WRITABLE 2

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
#define AE_DNOT_WAIT 4

#define AE_NOMORE -1

#define AE_NOTUSED(V) ((void) V)
#include <time.h>
struct aeEventLoop;

//------------------------------------------------------------------------------------------------

typedef void aeFileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, int mask);
typedef int aeTimeProc(struct aeEventLoop *eventLoop, long long id, void *clientData);
typedef void aeEventFinalizerProc(struct aeEventLoop *eventLoop, void *clientData);
typedef void aeBeforeSleepProc(struct aeEventLoop *eventLoop);

//------------------------------------------------------------------------------------------------

//文件事件结构
typedef struct aeFileEvent {
	//事件类型掩码 
	//值可以为 AE_READABLE, AE_WRITABLE 或 AE_READABLE|AE_WRITABLE
	int mask;
	//读事件函数
	aeFileProc *rfileProc;
	//写事件函数
	aeFileProc *wfileProc;
	//多路复用库的私有数据
	void *clientData;
} aeFileEvent;

//时间事件结构
typedef struct aeTimeEvent {
	//时间事件的唯一标识符
	long long id;
	//事件到达的时间
	long when_sec;
	long when_ms;
	//事件处理函数
	aeTimeProc *timeProc;
	//事件释放函数
	aeEventFinalizerProc *finalizerProc;
	//多路复用库私有数据
	void *clientData;
	//指向下一个时间事件结构的指针
	struct aeTimeEvent *next;
} aeTimeEvent;

//已就绪事件
typedef struct aeFiredEvent {
	//已就绪文件描述符
	int fd;
	//事件类型掩码
	//值可以是 AE_READABLE, AE_WRITABLE
	int mask;
} aeFiredEvent;

//事件处理器状态
typedef struct _aeEventLoop {
	//已注册最大描述符
	int maxfd;
	//已追踪的最大描述符
	int setsize;
	//生成时间事件id
	long long timeEventNextId;
	//最后一次执行时间事件的时间
	time_t lastTime;
	//已注册文件事件
	aeFileEvent *events;
	//已就绪文件事件
	aeFiredEvent *fired;
	//时间事件
	aeTimeEvent *timeEventHead;
	//事件处理器开关
	int stop;
	//多路复用库私有数据
	void *apidata;
	//在处理事件前要执行的函数
	aeBeforeSleepProc *beforesleep;
} aeEventLoop;

//------------------------------------------------------------------------------------------------

aeEventLoop *aeCreateEventLoop(int setsize);
void aeDeleteEventLoop(aeEventLoop *eventLoop);
void aeStop(aeEventLoop *eventLoop);

int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask, aeFileProc *proc, void *clientData);
void aeDelteFileEvent(aeEventLoop *eventLoop, int fd, int mask);
int aeGetFileEvents(aeEventLoop *eventLoop, int fd);

long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds, aeTimeProc *proc, void *clientData, aeEventFinalizerProc *finalizerProc);
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id);
int aeProcessEvents(aeEventLoop *eventLoop, int flags);
int aeWait(int fd, int mask, long long milliseconds);

void aeMain(aeEventLoop *eventLoop);
char  *aeGetApiName(void);
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *beforesleep);
#endif
