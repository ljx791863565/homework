#include "dict.h"
#include "zmalloc.h"
/*对不同系统的支持 ,暂时感觉不需要*/	
//#include "fmacros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*主要目的为让函数能够接收可变参数*/
#include <stdarg.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>


static int dict_can_resize = 1;
/*
 * ratio = used / size	哈希表的覆盖比例
 * 当其比例大于这个数的时候，会执行两个ht的强制rehash
 */
static unsigned int dict_force_resize_ratio = 5;


