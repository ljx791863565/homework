#ifndef _REDIS_H_
#define _REDIS_H_

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
#include <netinet/in.h>
#include <lua.h>
#include <signal.h>

#include "ae.h"
#include "sds.h"
#include "dict.h"
#include "adlist.h"
#include "zmalloc.h"
#include "anet.h"
#include "intset.h"
#include "util.h"

/*
 * Redis 对象
 */
typedef struct redisObject {
	unsigned type:4;
	unsigned notused:2;
	unsigned encoding:4;
	unsigned lru:22;
	int refcount;
	void *ptr;
} robj;
#endif
