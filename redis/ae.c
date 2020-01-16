#include "ae.h"
#include "zmalloc.h"

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <time.h>

/*
#ifdef HAVE_EVPORT
#include "ae_evpoll.c"
#else 
	#ifdef HAVE_EPOLL
	#include "ae_epoll.c"
	#else
		#ifdef HAVE_KQUEUE
		#include "ae_kqueue.c"
		#else
		#include "ae_selete.c"
		#endif
	#endif
#endif
*/
#include "ae_selete.c"


//初始化事件处理器状态
aeEventLoop *aeCreateEventLoop(int setsize)
{
	aeEventLoop *eventLoop;
	int i;
	if ((eventLoop = zmalloc(sizeof(aeEventLoop))) == NULL){
		if (eventLoop){
			zfree(eventLoop->events);
			zfree(eventLoop->fired);
			zfree(eventLoop);
		}
		return NULL;
	}

	eventLoop->events = zmalloc(sizeof(aeFileEvent) *setsize);
	eventLoop->fired = zmalloc(sizeof(aeFiredEvent) *setsize);
	if ((eventLoop->events == NULL) || (eventLoop->fired == NULL)){
		if (eventLoop){
			zfree(eventLoop->events);
			zfree(eventLoop->fired);
			zfree(eventLoop);
		}
		return NULL;
	}
	eventLoop->setsize = setsize;
	eventLoop->lastTime = time(NULL);
	eventLoop->timeEventHead = NULL;
	eventLoop->timeEventNextId = 0;
	eventLoop->stop = 0;	//事件处理器开关 1：停止 0：开始
	eventLoop->maxfd = -1;
	eventLoop->beforesleep = NULL;

	if (aeApiCreate(eventLoop) == -1){
		if (eventLoop){
			zfree(eventLoop->events);
			zfree(eventLoop->fired);
			zfree(eventLoop);
		}
		return NULL;
	}

	for (i = 0; i < setsize; i++){
		eventLoop->events[i].mask = AE_NONE;
	}

	return eventLoop;

}

//删除事件处理器
void aeDeleteEventLoop(aeEventLoop *eventLoop)
{
	aeApiFree(eventLoop);
	zfree(eventLoop->events);
	zfree(eventLoop->fired);
	zfree(eventLoop);
}

//停止事件处理器
void aeStop(aeEventLoop *eventLoop)
{
	eventLoop->stop = 1;
}

//根据参数mask 监听fd文件状态
//当fd可以时 执行参数proc函数
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask, aeFileProc *proc, void *clientData)
{
	if (fd >= eventLoop->setsize){
		return AE_ERR;
	}
	aeFileEvent *fe = &eventLoop->events[fd];

	if (aeApiAddEvent(eventLoop, fd, mask) == -1){
		return AE_ERR;
	}
	
	fe->mask |= mask;
	if (mask & AE_READABLE){
		fe->rfileProc = proc;
	}
	if (mask & AE_WRITABLE){
		fe->wfileProc = proc;
	}

	fe->clientData = clientData;

	if (fd > eventLoop->maxfd){
		eventLoop->maxfd = fd;
	}

	return AE_OK;
}

//将fd从mask指定的监听队列中删除
void aeDelteFileEvent(aeEventLoop *eventLoop, int fd, int mask)
{
	if (fd >= eventLoop->setsize){
		return;
	}
	aeFileEvent *fe = &eventLoop->events[fd];

	if (fe->mask == AE_NONE){
		return;
	}

	fe->mask = fe->mask & (~mask);
	if (fd == eventLoop->maxfd && fe->mask == AE_NONE){
		int k;
		for (k = eventLoop->maxfd-1; k>=0; k--){
			if (eventLoop->events[k].mask != AE_NONE)
				break;
			eventLoop->maxfd = k;
		}	
	}
	aeApiDelEvent(eventLoop, fd, mask);
}

//返回给定fd正在监听事件的类型
int aeGetFileEvents(aeEventLoop *eventLoop, int fd)
{
	if (fd > eventLoop->setsize){
		return 0;
	}
	aeFileEvent *fe = &eventLoop->events[fd];

	return fe->mask;
}

//取出当前时间 并保存到参数中
static void aeGetTime(long *seconds, long  *milliseconds)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	*seconds = tv.tv_sec;
	*milliseconds = tv.tv_usec/1000;
}

//当前时间加上 milliseconds ms
static void aeAddMillisecondsToNow(long long milliseconds, long *sec, long *ms)
{
		long cur_sec, cur_ms, when_sec, when_ms;
		ae_GetTime(&cur_sec, &cur_ms);

		when_sec = cur_sec + milliseconds/1000;
		when_ms = cur_ms + milliseconds%1000;

		if (when_ms >= 1000){
			when_sec ++;
			when_ms -= 1000;
		}

		*sec = when_sec;
		*ms = when_ms;
}

//创建时间事件
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds, aeTimeProc *proc, void *clientData, aeEventFinalizerProc *finalizerProc)
{
	long long id = eventLoop->timeEventNextId++;
	aeTimeEvent *te = zmalloc(sizeof(aeTimeEvent));
	if (te == NULL){
		return AE_ERR;
	}
	te->id = id;
	aeAddMillisecondsToNow(milliseconds, &te->when_sec, &te->when_ms);
	te->timeProc = proc;
	te->finalizerProc = finalizerProc;
	te->clientData = clientData;

	te->next = eventLoop->timeEventHead;
	eventLoop->timeEventHead = te;

	return id;
}

//删除给定id的时间事件
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id)
{
	aeTimeEvent *te = eventLoop->timeEventHead;
	aeTimeEvent *prev = NULL;
	while (te) {
		if (te->id == id) {
			//第一个节点
			if (prev == NULL) {
				eventLoop->timeEventHead = te->next;
			} else {
				prev->next = te->next;	
			}
			if (te->finalizerProc) {
				te->finalizerProc(eventLoop, te->clientData);
			}
			zfree(te);
			return AE_OK;
		}
		prev = te;
		te = te->next;
	}
	//没找到
	return AE_ERR;
}

static aeTimeEvent *aeSearchNearestTimer(aeEventLoop *eventLoop)
{
	aeTimeEvent *te = eventLoop->timeEventHead;
	aeTimeEvent *nearest = NULL;

	while (te) {
		if (!nearest || te->when_sec < nearest->when_sec || 
				(te->when_sec == nearest->when_sec) && te->when_ms < nearest->when_ms) {
			nearest = te;
		}
		te = te->next;
	}
	return nearest;
}

//处理所有已经到达的时间事件
static int processTimeEvents(aeEventLoop *eventLoop)
{
	int processed = 0;
	aeTimeEvent *te;
	long long maxId;
	time_t now = time(NULL);
	if (now < eventLoop->lastTime) {
		te = eventLoop->timeEventHead;
		while (te) {
			te->when_sec = 0;
			te = te->next;
		}
	}
	//更新最后一次处理时间事件的时间
	eventLoop->lastTime = now;


	te = eventLoop->timeEventHead;
	maxId = eventLoop->timeEventNextId - 1;
	while (te) {
		long now_sec, now_ms;
		long long id;
		if (te->id > maxId) {
			te = te->next;
			continue;
		}
		aeGetTime(&now_sec, &now_ms);
		//如果当前时间 >= 事件的执行时间 那么执行这个事件
		if (now_sec > te->when_sec || (now_sec == te->when_sec && now_ms >= te->when_ms)) {
			int retval;
			id = te->id;
			retval = te->timeProc(eventLoop, id, te->clientData);
			processed++;
			
			//根据执行结果判断是否需要循环执行这个时间事件
			if (retval != AE_NOMORE) {
				//需要 在retval ms后继续执行这个时间事件
				aeAddMillisecondsToNow(retval, &te->when_sec, &te->when_ms);
			}else {
				//不需要 事件已经执行完毕，删除它
				aeDeleteTimeEvent(eventLoop, id);
			}
			//执行完事件后 事件列表可能已经被改变了
			//需要将te重新指向head 继续开始
			te = eventLoop->timeEventHead;		
		}else {
			te = te->next;
		}
	}
	return processed;
}

//处理所有已经到达的时间事件 ，以及所有已经就绪的文件事件
//
//如果不传入特殊的flags 函数阻塞直到文件事件就绪 或者下一个时间事件到达
//flags = 0 函数什么都不做 直接返回
//flags 包含 AE_ALL_EVENTS 所有事件类型都会被处理
//flags 包含 AE_FILE_EVENT 处理文件事件
//flags 包含 AE_TIME_EVENT 处理时间事件
//flags 包含 AE_DNOT_WAIT  函数处理完所有不允许阻塞的事件之后 立刻返回
//
//返回已处理事件的数量
int aeProcessEvents(aeEventLoop *eventLoop, int flags)
{
	int processed = 0;
	int numevents;

	//既不是时间事件也不是文件事件 donothing
	if (!(flags & AE_TIME_EVENTS) && !(flags & AE_FILE_EVENTS)) {
		return 0;
	}

	if (eventLoop->maxfd != -1 || ((flags & AE_TIME_EVENTS) && !(flags & AE_DNOT_WAIT))) {
		int j;
		aeTimeEvent *shortest = NULL;
		struct timeval tv, *ptv;
		if (flags & AE_TIME_EVENTS && !(flags & AE_DNOT_WAIT)){
			//获得最近的时间事件
			shortest = aeSearchNearestTimer(eventLoop);
		}
		if (shortest) {
			//如果这个时间事件存在的话
			//根据最近的可执行时间事件和现在时间的时间差来决定文件事件的阻塞时间
			long now_sec, now_ms;
			aeGetTime(&now_sec, &now_ms);
			ptv = &tv;
			ptv->tv_sec = shortest->when_sec - now_sec;
			
			//ms位不够 向sec位借一位
			if (shortest->when_ms < now_ms){
				ptv->tv_usec = ((shortest->when_ms +1000) - now_ms)*1000;
				ptv->tv_sec--;
			}else {
				ptv->tv_usec = (shortest->when_ms - now_ms) *1000;
			}
			//时间差 < 0 说明事件已经可以被执行了 将tv_sec和tv_usec设置为0 
			//即直接执行 不阻塞
			if (ptv->tv_sec < 0) {
				ptv->tv_sec = 0;
			}
			if (ptv->tv_usec < 0) {
				ptv->tv_usec = 0;
			}
		}else {
			//到这里说明没有时间事件
			//根据AE_DNOT_WAIT 是否设置在决定是否阻塞等待 并且设置阻塞等待的时间
			if (flags & AE_DNOT_WAIT) {
				//这里将不阻塞
				tv.tv_sec = tv.tv_usec = 0;
				ptv = &tv;
			}else {
				//这里将阻塞 并且一直阻塞
				ptv = NULL;
			}
		}
		//处理文件事件
		numevents = aeApiPoll(eventLoop, ptv);
		for (j = 0; j < numevents; j++) { 
			//从已经就绪的事件数组中取得文件事件
			aeFileEvent *fe = &eventLoop->events[eventLoop->fired[j].fd];
			int mask = eventLoop->fired[j].mask;
			int fd = eventLoop->fired[j].fd;
			int rfired = 0;

			//读事件
			if (fe->mask & mask & AE_READABLE) {
				//确保读写事件只能执行其中一个
				rfired = 1;
				//执行读事件
				fe->rfileProc(eventLoop, fd, fe->clientData, mask);
			}
			//写事件
			if (fe->mask & mask & AE_WRITABLE) {
				if (!rfired || fe->wfileProc != fe->rfileProc) {
					//执行写事件
					fe->wfileProc(eventLoop, fd, fe->clientData, mask);
				}
			}
			processed++;
		}
	}
	//执行时间事件
	if (flags & AE_TIME_EVENTS) {
		processed += processTimeEvents(eventLoop);
	}

	return processed;
}

//阻塞milliseconds ms 直到fd变得可读/可写或其他
int aeWait(int fd, int mask, long long milliseconds)
{
	//#include <sys/poll.h>
	//struct pollfd{
	//	int fd;		//想查询的fd
	//	short int events;	//fd上我们感兴趣的事件 对某事感兴趣 可以直接将这些常亮和这个值按位或 | expm: fds[index].events = POLLIN | POLLOUT;
	//	short int revents;	//fd上实际发生的事件
	//};
	//int poll(struct pollfd *fds, unsigned int nfds, int timeout); 
	//fds：是一个struct pollfd结构类型的数组，用于存放需要检测其状态的Socket描述符；
	//每当调用这个函数之后，系统不会清空这个数组，操作起来比较方便；特别是对于 socket连接比较多的情况下，
	//在一定程度上可以提高处理的效率；这一点与select()函数不同，调用select()函数之后，
	//select() 函数会清空它所检测的socket描述符集合，导致每次调用select()之前都必须把socket描述符重新加入到待检测的集合中；
	//因此，select()函数适合于只检测一个socket描述符的情况，而poll()函数适合于大量socket描述符的情况；
	//
	//nfds 指定 pollfd 数组元素的个数，也就是要监测几个 pollfd
	//timeout：是poll函数调用阻塞的时间，单位：毫秒；
	//返回值
	//>0：数组fds中准备好读、写或出错状态的那些socket描述符的总数量；
	//==0：数组fds中没有任何socket描述符准备好读、写，或出错
	//如果所检测的 socket描述符上没有任何事件发生的话，那么poll()函数会阻塞timeout所指定的毫秒时间长度之后返回，
	//如果timeout==0，那么 poll() 函数立即返回而不阻塞，
	//如果timeout==INFTIM，那么poll() 函数会一直阻塞下去，直到所检测的socket描述符上的感兴趣的事件发生是才返回，
	//如果感兴趣的事件永远不发生，那么poll()就会永远阻塞下去；
	//
	//-1：  poll函数调用失败，同时会自动设置全局变量errno；
	struct pollfd pfd;
	int retmask = 0;
	int retval;

	memset(&pfd, 0, sizeof(struct pollfd));
	pfd.fd = fd;
	if (mask & AE_READABLE) {
		pfd.events |= POLLIN;
	}
	if (mask & AE_WRITABLE) {
		pfd.events |= POLLOUT;
	}

	if ((retval = poll(&pfd, 1, milliseconds)) == 1) {
		if (pfd.revents & POLLIN) {
			retmask |= AE_READABLE;
		}
		if (pfd.revents & POLLOUT) {
			retmask |= AE_WRITABLE;
		}
		if (pfd.revents & POLLERR) {
			retmask |= AE_WRITABLE;
		}
		if (pfd.revents & POLLHUP) {
			retmask |= AE_WRITABLE;
		}
		return retmask;
	}else {
		return retmask;	//0
	}
	
}

//事件处理器主循环函数
void aeMain(aeEventLoop *eventLoop)
{
	//开启事件处理器
	eventLoop->stop = 0;

	while (!eventLoop->stop) {
		//如果有需要在事件处理执行之前执行的函数 执行它
		if (eventLoop->beforesleep != NULL) {
			eventLoop->beforesleep(eventLoop);
		}

		//开始处理事件
		aeProcessEvents(eventLoop, AE_ALL_EVENTS);
	}
}
char  *aeGetApiName(void)
{
	return aeApiName();
}

//设置处理事件之前要执行的函数
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *beforesleep)
{
	eventLoop->beforesleep = beforesleep;
}

