#include "IoReader.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cerrno>

using namespace std;

const int MAX_LENGTH = 8192;

struct rio_t
{
	int rio_fd;
	int rio_cnt;
	char *rio_bufptr;
	char rio_buf[MAX_LENGTH];
};

void unix_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

void Rio_readinitb(rio_t *rp, int fd)
{
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;

	while (rp->rio_cnt <= 0){
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

		if (rp->rio_cnt < 0){
			if (errno != EINTR){
				return -1;
			}
		}else if (rp->rio_cnt == 0){
			return 0;
		}else {
			rp->rio_bufptr = rp->rio_buf;
		}
	}

	cnt = n;
	if (rp->rio_cnt < n){
		cnt = rp->rio_cnt;
	}
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;

	return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	int n, rc;
	char c, *bufp = reinterpret_cast<char *>(usrbuf);

	for (n = 1; n < maxlen; n++){
		if ((rc = rio_read(rp, &c, 1)) == 1){
			*bufp++ = c;
			if (c == '\n'){
				break;
			}
		}else if (rc == 0){
			if (n == 1){
				return 0;
			}else{
				break;
			}
		}else {
			return -1;
		}
	}

	*bufp = 0;
	return n;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	ssize_t rc;
	if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0){
		unix_error("Ric_readlineb error");
	}

	return rc;
}

rio_t rio;

IoReader::IoReader(int fd)
{
	Rio_readinitb(&rio, fd);
}

void IoReader::getLineSplitedByBlank(std::vector<std::string>& buf)
{
	char innerBuf[MAX_LENGTH], method[MAX_LENGTH], uri[MAX_LENGTH], version[MAX_LENGTH];
	Rio_readlineb(&rio, innerBuf, MAX_LENGTH);

	sscanf(innerBuf, "%s %s %s", method, uri, version);
	buf.push_back(method);
	buf.push_back(uri);
	buf.push_back(version);
}
