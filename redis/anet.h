#ifndef _ANET_H_
#define _ANET_H_

#define ANET_OK 0
#define ANET_ERR -1
#define ANET_ERR_LEN 256

#include <sys/types.h>
#include <sys/stat.h>

int anetTcpConnect(char *err, char *addr, int port);
int anetTcpNonBlockConnect(char *err, char *addr, int port);
int anetUnixConnect(char *err, char *path);
int anetUnixNonBlockConnect(char *err, char *path);

int anetTcpAccept(char *err, int serversock, char *ip, int *port);
int anetUnixAccept(char *err, int serversock);

int anetRead(int fd, char *buf, int count);
int anetWrite(int fd, char *buf, int count);

int anetTcpServer(char *err, int port, char *bindaddr);
int anetUnixServer(char *err, char *path, mode_t perm);

int anetNonBlock(char *err, int fd);
int anetTcpNoDelay(char *err, int fd);
int anetTcpKeepAlive(char *err, int fd);
int anetPeerToString(int fd, char *ip, int port);
int anetResolve(char *err, char *host, char *ipbuf);

#endif
