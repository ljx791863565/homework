#ifndef SELECT_H
#define SELECT_H

#include <vector>

class SelectServer
{
public:
    SelectServer();
    bool init();
    bool _listen(uint32_t port);
    bool pulse();
    void broadcast(const std::string& data);

protected:
    /// Callback functions to be override
    virtual void onNewConnection(const sockaddr_in& clientAddr) {(void)clientAddr;}
    virtual void onClientData(int clientFd, const std::string& data){(void)clientFd; (void)data;}
    virtual void onClientBroken(int errCode) {(void)errCode;}

private:
    bool _accept();
    bool _receive(int clientFd);
    bool removeClient(int clientFd);

    fd_set m_masterFdSet;                       /** master file descriptor list */
    fd_set m_readFdSet;                         /** temp file descriptor list for select() */
    sockaddr_in m_serverAddr;                   /** server address */
    int m_maxFd;                                /** maximum file descriptor number */
    int m_listenerSocket;                       /** listening socket descriptor */
    char buf[1024];                             /** buffer for client data */
};

#endif // SELECT_H
