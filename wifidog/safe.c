#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>

#include "safe.h"
#include "debug.h"

static void cheanup_fds(void);

typedef struct _fd_list {
	int fd;
	struct _fd_list *next;
} fd_list_t;
static fd_list_t *fd_list = NULL;
//fd_list链表保存都是子进程的fd 需要批量close
static void cheanup_fds(void)
{
	fd_list_t *entry;
	while( (entry = fd_list) != NULL ) {
		close(entry->fd);
		fd_list = entry->next;
		free(entry);
	}
}
void *safe_malloc(size_t size)
{
	void *retval = NULL;
	retval = malloc(size);
	if (!retval) {
		debug(LOG_CRIT, "Failed to malloc %d bytes of memory: %s. Bailing out", size, strerror(errno));
		exit(1);
	}
	memset(retval, 0, size);
	return retval;
}

void *safe_realloc(void *ptr, size_t newsize)
{
	void *retval = NULL;
	retval = realloc(ptr, newsize);
	if (!retval) {
		debug(LOG_CRIT, "Failed to realloc buffer to %d bytes of memory: %s. Bailing out", newsize, strerror(errno));
		exit(1);
	}
	return retval;
}

char *safe_strdup(const char *str)
{
	char *retval = NULL;
	if (!str) {
		debug(LOG_CRIT, "safe_strdup called with NULL witch would have crashed strdup. Bailing out");
		exit(1);
	}
	retval = strdup(str);
	if (!retval) {
		debug(LOG_CRIT, "Failed to duplicate a string: %s. Bailing out", strerror(errno));
		exit(1);
	}
	return retval;
}

int safe_asprintf(char **buf, const char *fmt, ...)
{
	va_list ap;
	int retval;

	va_start(ap, fmt);
	retval = safe_vasprintf(buf, fmt, ap);
	va_end(ap);

	return retval;
}

int safe_vasprintf(char **buf, const char *fmt, va_list ap)
{
	int retval;

	retval = vasprintf(buf, fmt, ap);

	if (retval == -1) {
		debug(LOG_CRIT, "Failed to vasprintf: %s. Bailing out", strerror(errno));
		exit(1);
	}
	return retval;
}

pid_t safe_fork(void)
{
	pid_t result;
	result = fork();

	if (result == -1) {
		debug(LOG_CRIT, "Failed to fork: %s. Bailing out", strerror(errno));
		exit(1);
	}else if (result == 0) {
		//child
		cleanup_fds();
	}

	//father
	return result;
}
