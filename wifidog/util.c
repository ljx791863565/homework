#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <net/if.h>
#include <fcntl.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>\
#include <netdb.h>
#include <string.h>

#include "util.h"
#include "common.h"
#include "safe.h"
#include "debug.h"
#include "pstring.h"

#define LOCK_GHBN() do { \
	debug(LOG_DEBUG, "Locking wd_gethostbyname()");\
	pthread_mutex_lock(&ghbn_mutex);\
	debug(LOCK_DEBUG, "wd_gethostbyname() locked");\
} while (0)

#define UNLOCK_GHBN() do { \
	debug(LOG_DEBUG, "Unlocking wd_gethostbyname()");\
	pthread_mutex_unlock(&ghbn_mutex);\
	debug(LOCK_DEBUG, "wd_gethostbyname() unlocked");\
} while (0)

#ifndef __ANDROID__
#define WD_SHELL_PATH "/system/bin/sh"
#else
#define WD_SHELL_PATH "/bin/sh"
#endif

//ICMP raw socket
static int icmp_fd;

static pthread_mutex_t ghbn_mutex = PTHREAD_MUTEX_INITIALIZER;

static unsigned short rand16(void);

int execute(const char *cmd_line, int quiet)
{
	int pid, status, rc;
	const char *new_argv[4];
	new_argv[0] = WD_SHELL_PATH;
	new_argv[1] = "-c";
	new_argv[2] = cmd_line;
	new_argv[3] = NULL;

	pid = safe_fork();
	if (pid == 0) {
		if (quiet) {
			close(2);
		}
		if (e) {
		
		}
	}
}
