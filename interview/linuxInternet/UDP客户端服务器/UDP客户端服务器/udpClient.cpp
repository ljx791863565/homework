
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
using namespace std;
#define  MESS_SIZE  1024
int main()
{
   //1.建立套接字
   int  clientfd =  socket(AF_INET,SOCK_DGRAM,0);
   //2.服务器地址
   sockaddr_in  svrAddr;
   svrAddr.sin_family = AF_INET;
   svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   svrAddr.sin_port = htons(8888); 
   
   //3.建立连接的UDP
   //connect(clientfd,(sockaddr*)&svrAddr,sizeof(svrAddr));
   //3.非连接的UDP
   char messBuff[MESS_SIZE];
   string mess;
   cout<<"--------------Client  is Online-----------------"<<endl;
   while(true)
   {
     cout<<"Please Enter Message:";
     memset(messBuff,0,MESS_SIZE);
     getline(cin,mess);
    //判断是否结束
    if(strcmp(mess.c_str(),"QUIT")==0)
    {
       sendto(clientfd,mess.c_str(),mess.size()+1,0,(sockaddr*)&svrAddr,sizeof(svrAddr));
     close(clientfd);
     return 0;
     }
    sendto(clientfd,mess.c_str(),mess.size()+1,0,(sockaddr*)&svrAddr,sizeof(svrAddr));
  
    //接着读取服务器返回的信息
    read(clientfd,messBuff,MESS_SIZE);
    //输出信息
    cout<<"Server Answer: ";
    cout<<messBuff<<endl;
   }
}
