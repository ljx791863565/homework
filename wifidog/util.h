#ifndef _UTIL_H_
#define _UTIL_H_

#define NUM_EXT_INTERFACE_DETECE_RETRY 0
#define EXT_INTERFACE_DETECT_RETRY_INTERVAL 1

int execute(const char *, int);

struct in_addr *wd_gethostbuname(const char *);

char *get_iface_ip(const char *);

char *get_iface_mac(const char *);

int *get_ext_iface(void);

int init_icmp_socket(void);

int close_icmp_socket(void);

void icmp_ping(const char *);

void save_pid_file(const char *);
#endif //_UTIL_H_
