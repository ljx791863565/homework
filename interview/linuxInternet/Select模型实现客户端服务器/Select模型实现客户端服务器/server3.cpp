#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/time.h>
using namespace std;

typedef int SOCKET; 
#define MESS_SIZE 1024
#define PORT 8888
/*****************************************
* Select模型实现回声服务器
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
    //3.创建fd_set变量
    fd_set reads;
    FD_ZERO(&reads);//初始化为0
    FD_SET(listenSock,&reads);//监听读操作
    int numOfSock =  listenSock;
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
    fd_set copyOfReads;//监听套接字副本，因为每次select操作都会修改fd_set集合，因此需要建立副本
    sockaddr_in  clientAddr;
    socklen_t  addrLen;
    char messBuf[MESS_SIZE];
    cout<<"---------------------Server is Online---------------------------"<<endl;
    while(true)
    {
           copyOfReads = reads;
           timeval timeOut; //超时时间
           timeOut.tv_sec = 5;
           timeOut.tv_usec = 0;//5秒钟
            int numOfFd=select(numOfSock+1,&copyOfReads,0,0,&timeOut);

           if(numOfFd==-1)
           {
               cout<<"Select Error"<<endl;
           }
           if(numOfFd==0)
           {
              continue;
           }
           
           for(int ii=0;ii<numOfSock+1;++ii)
           {
                if(FD_ISSET(ii,&copyOfReads)) //判断ii套接字是否在集合中
                {
                        if(ii==listenSock)  //判断是否是监听套接字发送事件
                        {
                             SOCKET  clientSock = accept(listenSock,(sockaddr*)&clientAddr,&addrLen);
                             FD_SET(clientSock,&reads);
                             cout<<"Client Connect"<<endl;
                             if(numOfSock<clientSock)
                             {
                                  numOfSock = clientSock;
                             }
                        }
                        else
                        {
                            //不是服务器套接字，则说明是客户端发送数据
                            //读取数据
                            memset(messBuf,0,MESS_SIZE);
                            int numOfCh = read(ii,messBuf,MESS_SIZE);
                     
                            if(numOfCh==0)//说明客户端断开连接
                            {
                              FD_CLR(ii,&reads);//将ii从套接字集合中移除
                              close(ii);
                            }
                            else
                            {
                                messBuf[numOfCh++] = '\0';
                                cout<<"From Client: "<<messBuf<<endl;
                                if(strcmp("QUIT",messBuf)==0)
                                {
                                  //客户端主动断开连接
                                  FD_CLR(ii,&reads);
                                  close(ii);
                                  break;
                                }
                                write(ii,messBuf,numOfCh);
                            }
                        }
                }
             
           }
    }    

     close(listenSock);
     return 0;
}



