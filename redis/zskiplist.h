#ifndef _ZSKIPLIST_H
#define _ZSKIPLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <syslog.h>
#include <lua.h>
#include <signal.h>

#include "ae.h"
#include "sds.h"
#include "dict.h"
#include "adlist.h"
#include "anet.h"
#include "zmalloc.h"
#include "ziplist.h"
#include "intset.h"
#include <util.h.h>

//对象类型 robj.type
#define REDIS_STRING	0
#define REDIS_LIST		1
#define REDIS_SET		2
#define REDIS_ZSET		3
#define REDIS_HASH		4

//对象编码 robj.encoding
#define REDIS_ENCODING_RAW			0
#define REDIS_ENCODING_INT			1
#define REDIS_ENCODING_HT			2	//hash table
#define REDIS_ENCODING_ZIPMAP		3	//zipmap
#define REDIS_ENCODING_LINKEDLIST	4	//reaular linked list
#define REDIS_ENCODING_ZIPLIST		5	//ziplist
#define REDIS_ENCODING_INTSET		6	//intset
#define REDIS_ENCODING_SKIPLIST		7	//skiplist


//:表示位域  
//位域必须存储在同一个类型中 不能跨类型 同时也说明位域的长度不会超过所定义类型的长度 
//如果一个定义类型单元里所剩空间无法存放下一个域 则下一个域应该从下一单元开始存放
//例如：所定义的类型是int类型 一共32为 目前用掉了25位还剩下7位 这时要存储一个8位的位域元素 
//那么这个元素就只能从下一个int类型的单元开始而不会在前面一个int类型中占7为后面的int类型中占1位。
//如果位域的位域长度为0表示是个空域，同时下一个域应当从下一个字节单元开始存放。
//使用无名的位域来作为填充和调整位置，切记该位域是不能被使用的。
//位域的本质上就是一种结构体类型，不同的是其成员是按二进制位来分配的。
//
//即type占第一个字节的前1-4位 notused占第一个字节的5-6位 encoding占第二个字节的1-4位 lru占位第三到第五字节 共22位
//	0		3	5	7		11		15				23				31			37	39		
//	--------------------------------------------------------------------------------
//	|				|				|				|				|				|			
//	---------------------------------------------------------------------------------
//	|		|	|空	|		|空		|											|空	|						
//	  type	notuesd	encoding			lur

//redis对象
typedef struct redisObject {
	//类型
	unsigned type:4;
	//不使用对齐位
	unsigned notused:2;
	//编码方式
	unsigned encoding:4;
	//LRU时间
	unsigned lru:22;
	//对齐计数
	int refcount;
	//指向对象的值
	void *ptr;
} robj;

//初始化从栈上分配的 Redis Object
#define initStaticStringObject(_var, _ptr) do {\
	_var.refcount = 1; \
	_var.type = REDIS_STRING; \
	_var.encoding = REDIS_ENCODING_RAW; \
	_var.ptr = _ptr; \
} while (0);

//数据库结构
typedef struct _redisDb {
	//保存key value的dict
	dict *dict;
	//保存key的过期时间的dict
	dict *expires;
	//保存因为某些key而被阻塞的客户端的dict
	dict *blocking_keys;
	//保存某个接收到 PUSH 命令的blocked key的dict
	dict *ready_keys;
	//保存正在监视某个key的所有客户端的dict
	dict *watched_keys;
	//数据库id
	int id;
} redisDb;

//跳跃表对象
typedef struct _zskiplistNode {
	//member 对象
	robj *obj;
	//分值
	double score;
	//后退指针
	struct zskiplistNode *backword;
	//层
	struct zskiplistLevel {
		//前进指针
		struct zskiplistNode *forword;
		//这个层跨越的节点数量
		unsigned int span;
	} level[];
} zskiplistNode;

typedef struct _zskiplist {
	struct zskiplistNode 
} zskiplist;
#endif
