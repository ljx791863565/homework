

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <poll.h>
using namespace std;
/**********************************
 Poll模型实现回声服务器

 1.服务器IP  127.0.0.1 端口号 8888
 2.客户端与服务端通信  客户端发送QUIT主动断开连接
 3.学习使用，没有处理TCP粘包

***********************************/
const int MAXCLIENT = 10;
const int MESS_SIZE = 1024;

//处理客户端通信的信息
void handle(pollfd *fdset,int maxSock,int nReady)
{
    char messBuf[MESS_SIZE];
    //轮询找到发生事件的套接字
    for(int ii=1;nReady>0 && ii<maxSock;++ii)
    {
      
        if(fdset[ii].revents & POLLRDNORM)
        {
            //读数据
            int res = read(fdset[ii].fd,messBuf,MESS_SIZE);
            messBuf[res] = '\0';
            //断开连接
            if(strcmp("QUIT",messBuf)==0)
            {
                  cout<<"Client Quit"<<endl;
                  close(fdset[ii].fd);
                  fdset[ii].fd = -1;
                  continue;
            }
            cout<<"Client: "<<messBuf<<endl;
            write(fdset[ii].fd,messBuf,res+1);
            --nReady ;
        }
        
    }
}

int main()
{
   //1.建立监听套接字
   int listenSock = socket(AF_INET,SOCK_STREAM,0);
   //2.服务器地址
   sockaddr_in  svrAddr;
   svrAddr.sin_family = AF_INET;
   svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   svrAddr.sin_port = htons(8888);
   //3.绑定
   if(bind(listenSock,(sockaddr*)&svrAddr,sizeof(sockaddr))==-1)
   {
        cout<<"Bind Failed"<<endl;
        close(listenSock);
        return -1;
   }

   //4.监听
   if(listen(listenSock,5)==-1)
   {
        cout<<"Listen Failed"<<endl;
        close(listenSock);
        return -1;
   }
   //客户端套接字
   sockaddr_in  clientAddr;
   socklen_t   addrLen;
   //poll集合
   pollfd fdset[MAXCLIENT];
   memset(fdset,0,sizeof(pollfd)*MAXCLIENT);
   //5.将监听套接字加入集合
   for(int ii=0;ii<MAXCLIENT;++ii)
   {
     fdset[ii].fd = -1; 
   }
   fdset[0].fd = listenSock;
   fdset[0].events = POLLRDNORM;
   int maxSock = 1;
   cout<<"-------------Poll Server is Online-------------------"<<endl;
   while(true)
   {
    //6.查询套接字事件 阻塞方式
    int nReady  =  poll(fdset,maxSock,-1);
    if(nReady ==-1)
    {
       cout<<"Poll Failed"<<endl;
       close(listenSock);
       return -1;
    }
    //7.处理客户端连接请求
    if(fdset[0].revents & POLLRDNORM)
    {
         int connfd = accept(listenSock,(sockaddr*)&clientAddr,&addrLen);
         if(connfd<0)
         {
            cout<<"Accept Failed"<<endl;
            return -1;
         }
          cout<<"Client Connect "<<endl;
         int jj=0;
         //8.将客户端套接字加入集合中
         for( jj=1;jj<MAXCLIENT;jj++)
         {
             if(fdset[jj].fd==-1)
             {
                 fdset[jj].fd = connfd;
                 fdset[jj].events = POLLRDNORM;
                 break;
             }
         }
         if(jj==MAXCLIENT)
         {
            //连接客户端数目达到最大值
            close(connfd);
            continue;
         }
         ++maxSock;
         --nReady;
    }
    //9.处理客户端信息
    if(nReady>0)
     handle(fdset,maxSock,nReady);
   }

}
