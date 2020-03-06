#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include  <sys/epoll.h>
using namespace std;

typedef int SOCKET; 
#define MESS_SIZE 1024
#define PORT 8888
#define NUMOFEVENTS 50
/*****************************************
* Epoll模型实现回声服务器
* 1.客户端发送QUIT主动断开连接
* 2.服务器IP地址127.0.0.1 默认端口为 8888
* 3.供学习使用，没有做TCP粘包处理
******************************************/

int main()
{
    //1.创建监听套接字
    SOCKET listenSock = socket(AF_INET,SOCK_STREAM,0);
    //2.定义服务器IP地址
    sockaddr_in svrAddr;
    memset(&svrAddr,0,sizeof(svrAddr));
    svrAddr.sin_family = AF_INET;
    svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svrAddr.sin_port = htons(PORT);
    //3.创建epoll_event变量
    struct epoll_event *events =(epoll_event *) malloc(sizeof(epoll_event)*NUMOFEVENTS);//发生事件缓冲区
    struct epoll_event event;//定义单个事件
    int epollHandle;//返回的文件描述符
    int numOfEvent;//实际发生事件的个数
    //4.绑定
    if(bind(listenSock,(sockaddr*)&svrAddr,sizeof(svrAddr))==-1)
    {
       cout<<"Bind Failed"<<endl;
    }
    //5.监听
    if(listen(listenSock,5)==-1)
    {
        cout<<"Listen Failed"<<endl;
    }
    //6.向内存申请一块区域
    epollHandle = epoll_create(NUMOFEVENTS);
    event.events = EPOLLIN;//监听可读事件
    event.data.fd = listenSock;//监听的套接字
    //7.将监听的事件加入
    epoll_ctl(epollHandle,EPOLL_CTL_ADD,listenSock,&event);

    sockaddr_in  clientAddr;
    socklen_t  addrLen;
    char messBuf[MESS_SIZE];
    cout<<"---------------------Server is Online---------------------------"<<endl;
    while(true)
    {
           //8.等待
           numOfEvent = epoll_wait(epollHandle,events,NUMOFEVENTS,-1);
           if(numOfEvent ==-1)
           {
              cout<<"Epoll_wait Failed"<<endl;
              break;
           }

           //9.处理发生的事件
           for(int ii=0;ii<numOfEvent;++ii)
           {
                        if(events[ii].data.fd==listenSock)  //判断是否是监听套接字发生事件
                        {
                             SOCKET  clientSock = accept(listenSock,(sockaddr*)&clientAddr,&addrLen); 
                             cout<<inet_ntoa(clientAddr.sin_addr)<<" Connect"<<endl;
                             event.events = EPOLLIN;
                             event.data.fd = clientSock;
                            //将客户端套接字加入监听队列
                             epoll_ctl(epollHandle,EPOLL_CTL_ADD,clientSock,&event);
                        }
                        else
                        {
                            //不是服务器套接字，则说明是客户端发送数据
                            //读取数据
                            memset(messBuf,0,MESS_SIZE);
                            int numOfCh = read(events[ii].data.fd,messBuf,MESS_SIZE);
                     
                            if(numOfCh==0)//说明客户端断开连接
                            {
                              epoll_ctl(epollHandle,EPOLL_CTL_DEL,events[ii].data.fd,NULL);
                              close(events[ii].data.fd);
                            }
                            else
                            {
                                messBuf[numOfCh++] = '\0';
                                cout<<"From Client: "<<messBuf<<endl;
                                if(strcmp("QUIT",messBuf)==0)
                                {
                                  //客户端主动断开连接
                                  epoll_ctl(epollHandle,EPOLL_CTL_DEL,events[ii].data.fd,NULL);
                                  close(events[ii].data.fd);
                                  break;
                                }
                                write(events[ii].data.fd,messBuf,numOfCh);
                            }
                }
             
           }
    }    

     close(listenSock);
     close(epollHandle);
     return 0;
}



