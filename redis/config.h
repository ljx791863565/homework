#ifndef _CONFIG_H_
#define _CONFIG_H_ 

#ifdef __linux__
#define HAVE_PROC_STAT 1
#define HAVE_PROC_MAPS 1
#define HAVE_PROC_SMAPS 1
#endif

#define HAVE_BACKTRACE 1
#define HAVE_EPOLL 1
#define aof_fsync fdatasync

#include <linux/version.h>
#include <features.h>
#define HAVE_SYNC_FILE_RANGE 1

#define rdb_fsync_range(fd,off,size) sync_file_range(fd,off,size,SYNC_FILE_RANGE_WAIT_BEFORE|SYNC_FILE_RANGE_WRITE)

#include <sys/types.h>
#include <endian.h>

#endif	//_CONFIG_H_
