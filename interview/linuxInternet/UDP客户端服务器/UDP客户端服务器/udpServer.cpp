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

const  int  MESS_SIZE = 1024;
int main()
{
    SOCKET listenSock;//服务器监听套接字
    char  messBuff[MESS_SIZE];
 
    sockaddr_in svrAddr;//服务器地址 客户端地址
    memset(&svrAddr,0,sizeof(svrAddr));//初始化为0
    //创建一个服务器socket
    listenSock = socket(AF_INET,SOCK_DGRAM,0);  //基于TCP 
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
 
   cout<<"--------------Server is Online------------------------"<<endl;
    //创建保存客户端信息的结构体
    SOCKET  clientSock;
    sockaddr_in clientAddr;// 客户端地址
    socklen_t  clientLen = sizeof(clientAddr);
    memset(&clientAddr,0,sizeof(clientAddr));
   // 循环等待客户端的连接
    while(true)
    {
             memset(messBuff,0,MESS_SIZE);
             //接收数据
             ssize_t len = recvfrom(listenSock,messBuff,MESS_SIZE,0,(sockaddr*)&clientAddr,&clientLen);
             cout<<inet_ntoa(clientAddr.sin_addr)<<":";
             cout<<messBuff<<endl;
              //否则回写到客户端
             sendto(listenSock,messBuff,len+1,0,(sockaddr*)&clientAddr,clientLen);
     }
    //6.关闭socket
    close(listenSock);
    return 0;

}
