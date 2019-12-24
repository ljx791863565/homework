#include "communal.h"

void send_result(int fd, int result, int type)
{
	MSH_HEAD_T msghead;
	memset(&msghead, 0, sizeof(msghead));
	int len = sizeof(msghead);

	msghead.len = len;
	msghead.msg_type = type;
	msghead.result = result;

	msghead.len = htonl(msghead.len);
	msghead.msg_type = htonl(msghead.msg_type);
	msghead.result = htonl(msghead.result);

	int ret = write(fd, &msghead, sizeof(msghead));
	if (ret < 0){
		perror("write %d", type);
		return NULL;
	}
}
