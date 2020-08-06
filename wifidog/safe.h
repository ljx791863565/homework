#ifndef _SAFE_H_
#define _SAFE_H_

void register_fd_cleaunp_on_fork(const int fd);

void *safe_malloc(size_t size);

void *safe_realloc(void *ptr, size_t size);

char *safe_strdup(const char *str);

int safe_asprintf(char **buf, const char *fmt, ...);

int safe_vasprintf(char **buf, const char *fmt, va_list);

pid_t safe_fork(void);
#endif	//_SAFE_H_

