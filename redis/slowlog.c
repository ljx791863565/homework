#include "slowlog.h"
#include "redis.h"

slowlogEntry *slowlogCreateEntry(robj **argv, int argc, long long duration)
{
	slowlogEntry *se = zmalloc(sizeof(*se));
	int slargc = argc;

	if (slargc > SLOWLOG_ENTRY_MAX_ARGC) {
		slsrgc = SLOWLOG_ENTRY_MAX_ARGC;
	}

	se->argc = slargc;
	se->argv = zmalloc(sizeof(robj*) * slargc);

	int i;
	for (i = 0; i < slargc; i++) {
		if (slargc != argc && i == slargc-1) {
			//当参数数量超过服务器允许的最大参数数量时 用最后一个参数表示省略提示
			se->argv[i] = crearteObject(REDIS_STRING, sdscatprintf(sdsempty(), "... (%d more arguments)", argc-slargc+1));
		} else {
			//如果参数太长 截断到SLOWLOG_ENTRY_MAX_STRING
			if (argv[i]->type == REDIS_STRING 
					&& argv[i]->encoding == REDIS_ENCODING_RAW 
					&& sdslen(argv[i]->ptr) > SLOWLOG_ENTRY_MAX_STRING) {
				sds s = sdsnewlen(argv[i]->ptr, SLOWLOG_ENTRY_MAX_STRING);
				s = sdscatprintf(s, "... (%lu more bytes)", (unsigned long)sdslen(argv[i]->ptr) - SLOWLOG_ENTRY_MAX_STRING);
				se->argv[i] = createObject(REDIS_STRING, s);
			} else {
				se->argv[i] = argv[i];
				//增加对象的引用计数
				incrRefCount(argv[i]);
			}	
		}
	}
	se->time = time(NULL);
	se->duration = duration;
	se->id = server.slowlog_entry_id++;

	return se;
}

void slowlogFreeEntry(void *septr)
{
	slowlogEntry *se = septr;
	int i;
	for (i = 0; i < se->argc; i++) {
		decrRefCount(se->argv[i]);
	}
	zfree(se->argv);
	zfree(se);
}

void slowlogInit(void)
{
	//创建慢查询链表
	server.slowlog = listCreate();
	//初始化服务器慢查询日志id
	server.slowlog_entry_id = 0;
	//初始化慢查询日志的析构函数
	listSetFreeMethos(server.slowlog, slowlogFreeEntry);
}

void slowlogPushEntryIfNeeded(robj **argv, int argc, long long duration)
{
	//标志慢查询功能未开启 不进行push
	if (server.slowlog_log_slower_than < 0) {
		return;
	}
	//运行时间满足条件 add进链表
	if (duration >= server.slowlog_log_slower_than) {
		listAddNodeHead(server.slowlog, slowlogCreateEntry(argv, argc, duration));
	}

	//日志只保持 server.slowlog_max_len条 过多的日志会被删除
	while (listLength(server.slowlog) > server.slowlog_max_len) {
		listDelNode(server.slowlog, listLast(server.slowlog));
	}
}

//清除全部的慢查询日志
void slowlogReset(void)
{
	while (listLength(server.slowlog) > 0) {
		listDelNode(server.slowlog, listLast(server.slowlog));
	}
}

//SLOWLOG 命令实现 支持 GET RESET LEN参数
void slowlogCommand(redisClient *c)
{
	if(c->argc == 2 && !strcasecmp(c->argv[1]->ptr, "reset")) {
		slowlogReset();
		addReply(c, shared.ok);
	} else if (c->argc == 2 && !strcasecmp(c->argv[1]->ptr, "len")) {
		aadReplyLongLong(c, listLength(server.slowlog));
	}else if ((c->argc == 2 || c->argc == 3) && !strcasecmp(c->argv[1]->ptr, "get")) {
		//get某条或全部日志
		long count = 10;
		long sent = 0;
		listIter li;
		listNode *ln;
		slowlogEntry *se;

		if (c->argc == 3 && ) {
		
		}
	}
}
