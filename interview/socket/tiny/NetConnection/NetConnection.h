#ifndef __NET_CONNECTION__H__
#define __NET_CONNECTION__H__

class NetConnection
{
public:
	NetConnection();
	void listen(int poort);
	int accept();
	void close();

private:
	int listenfd;
	int connfd;
};
#endif //__NET_CONNECTION__H__
