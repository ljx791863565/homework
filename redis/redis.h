#ifndef _REDIS_H_
#define _REDIS_H_ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

//提供整数的各种转换宏
#include <inttypes.h>

#include <pthread.h>
#include <netinet/in.h>
#include <lua.h>
#include <signal.h>
#include <syslog.h>

#include "ae.h"
#include "anet.h"
#include "sds.h"
#include "dict.h"
#include "adlist.h"
#include "zmalloc.h"
#include "ziplist.h"
#include "intset.h"
#include "util.h"

#endif
