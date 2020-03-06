#include  <unistd.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <arpa/inet.h>
#include <sys/socket.h>
#include  <signal.h>
#include <iostream>
#include  <sys/wait.h>
using namespace std;
typedef int SOCKET;
#define  PORT  8888
#define  MESS_SIZE 1024
/******************************
*  非阻塞多进程服务器模型
*  1.多进程形式实现服务器，回声服务器，即是 服务器将客户端发送过来的信息发送回客户端
*  2.多个客户端发送QUIT表示断开与服务器的连接
*  3.服务器默认端口号为 8888
*  4.服务器和客户端运行于同一台电脑上时，IP地址为127.0.0.1
*  5.作为练习使用，没有做TCP粘包处理。
***********************************/
//子进程退出 即是客户端断开连接
void doChildExit(int sig)
{
    int  status;
    waitpid(-1,&status,WNOHANG);  //处理僵尸进程
    cout<<"ChildProcess  Exited"<<endl;
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
   //3.定义信号处理事件函数
   struct  sigaction  childExit;
   memset(&childExit,0,sizeof(childExit));
   childExit.sa_handler = doChildExit;
   //4.注册事件
   sigaction(SIGCHLD,&childExit,0);   
   //5.创建套接字
   listenSock = socket(AF_INET,SOCK_STREAM,0);
   //6.绑定
   if(bind(listenSock,(sockaddr*)&svrAddr,sizeof(svrAddr))==-1)
    {
        cout<<"Bind Failed"<<endl;
    }
    //7.监听    
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
         cout<<"Connect From :"<<inet_ntoa(clientAddr.sin_addr)<<endl;         

         //10.创建新的进程处理连接
         pid_t pid = fork();
         if(pid==-1)
          {
            cout<<"Create SubProcess Failed"<<endl;
            close(clientSock);
            continue;
          }         

         //11.定义子进程和父进程代码块
          if(pid == 0)
           {
               //子进程执行代码段
        
             //注意：  进来以后先关闭监听套接字
             close(listenSock);
             //与客户端进行读写控制
             char MessBuf[MESS_SIZE];
             int messLen=0;
             //与客户端循环读写，直至客户端发送QUIT断开与服务器的连接
             while(true)
             {
               //读写操作
               memset(MessBuf,0,MESS_SIZE);
               messLen = read(clientSock,MessBuf,MESS_SIZE-1);
               if(messLen==0)
                {
                    cout<<inet_ntoa(clientAddr.sin_addr)<<": " <<"UnFriendly Disconnected"<<endl;  
                    close(clientSock);
                    return 0;
                 }
               else
                {
                     if(messLen >0 && strcmp(MessBuf,"QUIT")==0)
                      {
                        //客户端停止数据发送
                        cout<<"Client Quit"<<endl;  
                        close(clientSock);
                        return 0;
                       }
                       else 
                       {
                        cout<<inet_ntoa(clientAddr.sin_addr)<<": " << MessBuf<<endl;   
                       //回写到客户端
                         write(clientSock,MessBuf,messLen+1);
                       }
          
                  }              
             }
              
           }else
           {
              //父进程执行代码段
              close(clientSock);
           }
    }

   close(listenSock);

}
