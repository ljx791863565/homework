#ifndef __SELECTSERVER__H__
#define __SELECTSERVER__H__
#define MAXLEN 1024

#include <vector>
#include <stdint.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
class  SelectServer
{
public:
	SelectServer();
	bool init();
	bool _listen(uint32_t port);
	bool pulse();
	void broadcast(const string  &data);

protected:
	//Callback functions to be override
	virtual void onNewConnection(const sockaddr_in &clientAddr)
	{
		(void)clientAddr;
	}
	virtual void onClientData(int clientFd, const string &data)
	{
		(void)clientFd;
		(void)data;
	}
	virtual void onClientBroken(int errCode)
	{
		(void)errCode;
	}

private:
	bool _accept;
	bool _receive(int clientFd);
	bool removeClient(int clientFd);

	fd_set m_masterFdSet;			//master file descriptot list
	fd_set m_readFdSet;				//temp file descriptor list for select()
	sockaddr_in m_serverAddr;		//server address
	int m_maxFd;
	int m_listenerSocket;			//listening socket descriptor
	char buf[MAXLEN];
};
#endif //__SELECTSERVER
