#ifndef _UTIL_H_
#define _UTIL_H_ 
int stringmatchlen(const char *p, int plen, const char *s, int slen, int nocase);
int stringmatch(const char *p, const char *s, int nocase);
long long memtool(const char *p, int *err);
int ll2string(char *s, size_t len, long long value);
int string2ll(const char *s, size_t slen, long long *value);
int string2l(const char *s, size_t slen, long long *value);
int d2string(char *buf, size_t len, double value);
#endif