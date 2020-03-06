#include  <unistd.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <arpa/inet.h>
#include <sys/socket.h>
#include  <signal.h>
#include <iostream>
#include  <sys/wait.h>
#include <pthread.h>
using namespace std;
typedef int SOCKET;
#define  PORT  8888
#define  MESS_SIZE 1024
/******************************
*  非阻塞多线程服务器模型
*  1.多线程形式实现服务器，回声服务器，即是 服务器将客户端发送过来的信息发送回客户端
*  2.多个客户端发送QUIT表示断开与服务器的连接
*  3.服务器默认端口号为 8888
*  4.服务器和客户端运行于同一台电脑上时，IP地址为127.0.0.1
*  5.作为练习使用，没有做TCP粘包处理。
***********************************/



void clientThread(void * args)
{
  char messBuf[MESS_SIZE];
  int  numOfCh;//接收数据的长度
  SOCKET  clientSock = *(int*)args;
  while(true)
  {
       numOfCh = read(clientSock,messBuf,MESS_SIZE-1);
       if(numOfCh ==0)
       {
          //为线程收尸

          //断开连接
          close(clientSock);
       }
       messBuf[numOfCh] = '\0';
       if(numOfCh >0 && strcmp(MessBuf,"QUIT")==0)
        {
            //客户端停止数据发送
             cout<<"Client Quit"<<endl;  
             close(clientSock);
             break;
         }
         else 
         {
             cout<<"From Client: "<<messBuf<<endl;
             write(clientSock,messBuf,numOfCh+1);
         }
  }
    close(clientSock);
}



int main()
{
   //1.创建监听套接字句柄
   SOCKET  listenSock;
   //2.服务器地址
   struct sockaddr_in svrAddr;
   memset(&svrAddr,0,sizeof(svrAddr));
   svrAddr.sin_family = AF_INET;
   svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   svrAddr.sin_port = htons(PORT);
   //3.创建套接字
   listenSock = socket(AF_INET,SOCK_STREAM,0);
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
    cout<<"-----------------Server  is Online---------------------"<<endl;
    struct sockaddr_in   clientAddr;
    SOCKET  clientSock;
    socklen_t clientAddrLen = sizeof(clientAddr);
    //8.主进程循环监听客户端的连接
    while(true)
    {
         //9.接受连接 
         clientSock = accept(listenSock,(sockaddr*)&clientAddr,&clientAddrLen);
         //接受失败
         if(clientSock ==-1 )
         {
            cout<<"Accept Failed"<<endl;
            continue;
         }
         cout<<"Connect From "<<inet_ntoa(clientAddr.sin_addr)<<endl;         
         //10.创建新的线程处理连接
         pthread_t  threadID;

         if(pthread_create(&threadID,NULL,clientThread,&clientSock)==-1)
         {
                cout<<"Create Thread Failed"<<endl;
                close(clientSock);
                continue;
         }
         pthread_detach(threadID);
        
    }

   close(listenSock);
}
