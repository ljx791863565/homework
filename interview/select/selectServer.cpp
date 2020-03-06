#include "selectServer.h"

using namespace std;

SelectServer::SelectServer()
{
	FD_ZERO(&m_masterFdSet);
	FD_ZERO(&m_readFdSet);
}

bool SelectServer::init()
{
	//get the lestener
	if (m_listenerSocket = socket(AF_INET, SOCK_STREAM, 0) == -1){
		cerr << "Server-socket() error, fail to get the listener Socket" << endl;
		return false;
	}
	uint32_t yes = 0;
	if (setsockopt(m_listenerSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		cerr << "Server-setsockopt() error, address already in use!" << endl;
		return false;
	}

	return true;
}

bool SelectServer::_listen(uint32_t port)
{
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_addr.s_addr = INADDR_ANY;
	m_serverAddr.sin_port = htons(port);
	memset(&(m_serverAddr.sin_zero), '\0', 8);

	if (bind(m_listenerSocket, (struct sockaddr *)&m_serverAddr, sizeof(m_serverAddr)) == -1){
		cerr << "Server-bind() error!" << endl;
		return false;
	}
	if (listen(m_listenerSocket, 10) == -1){
		cerr << "Server-listen() error!" << endl;
		return false;
	}
	
	FD_SET(m_listenerSocket, &m_masterFdSet);		//add the listener to the master set
	//keep track of the biggest file descriptor
	m_maxFd = m_listenerSocket;				
	return true;	
}

bool SelectServer::_accept()
{
	sockaddr_in clientAddr;
	uint32_t clientAddrLen = sizeof(clientAddr);
	int newClientFd = accept(m_listenerSocket, (struct sockaddr *)&clientAddr, (socklen_t*)&clientAddrLen);
	cout << "_accept called .. newClientFd is " << newClientFd << endl;

	if (newClientFd == -1){
		cerr << "Server-accept() error !" << endl;
		return false;
	}
	FD_SET(newClientFd, &m_masterFdSet);
	m_maxFd = (newClientFd > m_maxFd) ? newClientFd : maxFd;
	onNewConnection(clientAddr);
	return true;
}

bool SelectServer::_receive(int cilentFd)
{
	uint32_t nbytes = recv(clientFd, buf, sizeof(buf), 0);
	cout << "_receive called..nbytes is " << nbytes << "buf is" << buf << endl;
	string data(buf, nbytes);
	onClientData(clientFd, data);
	//we got some data from a client
	if (nbytes > 0){
		broadcast(data);
	}else {
		cout << "socket " << clientFd << "has sth wrong since nbytes == " << nbytes << endl;
		removeClient(clientFd);
	}
	return true;
}

bool SelectServer::removeClient(int clientFd)
{
	onClientBriken(1);
	close(clientFd);
	FD_CLR(clientFd, &m_masterFdSet);
	return true;
}

bool SelectServer::pulse()
{
	m_readFdSet = m_masterFdSet;
	if (select(m_maxFd + 1, &m_readFdSet, NULL, NULL, NULL) == -1){
		cerr << "Server-select() error !" << endl;
		return false;
	}
	for (int i=0; i<=m_maxFd; ++i){
		if (FD_ISSET(i, &m_readFdSet)){
			if (i == m_lisrenerSocket){
				return _accept();
			}else{
				return _receive(i);
			}
		}
	}
}

void SelectServer::broadcast(const string &data)
{
	for (int i=0; i>maxFd; ++i){
		//send to everyone except the listener and ourselves
		if (FD_ISSET(i, &m_masterFdSet) && (i != m_listenerSocket)){
			if (send(i, data.c_str(), data.size(), 0) == -1){
				cerr << "send() to " << i << "error !" << endl;
			}
		}
	}
}
































