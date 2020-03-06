#include "iostream"
#include <string.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;
typedef int SOCKET;//宏定义  Socket句柄
#define PORT 8888
/******************************
*回声服务器
*1.接收来自于客户端的数据然后回写到客户端
*2.阻塞式，服务于一个客户端结束后才能接受另外的客户端连接，且需要客户端主动断开连接
*3.客户端发送QUIT结束会话
*4.服务器IP地址 127.0.0.1 端口号 8888
*******************************/
const  int  MESS_SIZE = 1024;
int main()
{
    SOCKET listenSock;//服务器监听套接字
    char  messBuff[MESS_SIZE];
 
    sockaddr_in svrAddr;//服务器地址 客户端地址
    memset(&svrAddr,0,sizeof(svrAddr));//初始化为0
    //创建一个服务器socket
    listenSock = socket(AF_INET,SOCK_STREAM,0);  //基于TCP 
    //初始化服务器地址
    svrAddr.sin_family = AF_INET;  //IPV4
    svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //IP地址
    svrAddr.sin_port = htons(PORT);    //端口号
    //2. 绑定
    if(bind(listenSock,(struct sockaddr*)(&svrAddr),sizeof(svrAddr))==-1)
    {
      //绑定失败
      cout<<"bind  failed"<<endl;
    }
    //3.监听
    listen(listenSock,5);  // 5表示监听队列的个数
   cout<<"--------------Server is Online------------------------"<<endl;
    //创建保存客户端信息的结构体
    SOCKET  clientSock;
    sockaddr_in clientAddr;// 客户端地址
    socklen_t  clientLen = sizeof(clientAddr);
    memset(&clientAddr,0,sizeof(clientAddr));
   // 循环等待客户端的连接
    while(true)
    {
       //4.接受连接
       clientSock = accept(listenSock,(sockaddr*)&clientAddr,&clientLen);
       //判断是否连接成功
        if(clientSock==-1)
        {
         cout<<"Accept Failed"<<endl;
        }else
        {
            cout<<"Connect From ";
            cout<<inet_ntoa(clientAddr.sin_addr)<<endl;
        }
       //5.从clientSock读取内容，即是客户端发送来的数据
       int  len=0;
       while(true)
       {
             memset(messBuff,0,MESS_SIZE);
             len = read(clientSock,messBuff,MESS_SIZE);
             cout<<inet_ntoa(clientAddr.sin_addr)<<":";
             cout<<messBuff<<endl;
            
              //判断是否是结束符
             if(len==0 || len>0 &&strcmp("QUIT",messBuff)==0 )
             {
                 cout<<"Client Quit"<<endl;
                 close(clientSock);
                 break;
             }
              //否则回写到客户端
              write(clientSock,messBuff,len);
        }

     }
    //6.关闭socket
    close(listenSock);
    return 0;

}
