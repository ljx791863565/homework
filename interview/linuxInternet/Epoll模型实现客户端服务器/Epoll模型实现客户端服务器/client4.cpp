#include  "iostream"
#include  <string.h>
#include  <stdlib.h>
#include  <stdio.h>
#include  <unistd.h>
#include  <arpa/inet.h>
#include  <sys/socket.h>
using  namespace std;
typedef  int SOCKET;
#define  MESS_SIZE  1024
int main()
{
  //服务器地址
  cout<<"---------------Client  is Online----------------------"<<endl;
  struct sockaddr_in svrAddr;
  cout<<"请输入服务器IP地址：";
  string strIP;
  cin>>strIP;
  cout<<"请输入服务器端口号：";
  int svrPort=0;
  cin>>svrPort;
  memset(&svrAddr,0,sizeof(sockaddr_in));
  svrAddr.sin_family = AF_INET;
  svrAddr.sin_addr.s_addr = inet_addr(strIP.c_str());
  svrAddr.sin_port = htons(svrPort);
  //创建一个客户端socket
  SOCKET  clientSock = socket(AF_INET,SOCK_STREAM,0);
 //2.连接服务器
  int  res = connect(clientSock,(sockaddr*)&svrAddr,sizeof(svrAddr));
   if(res==-1)
   {
    cout<<"Connect Failed"<<endl;
   }
   else
   {
     cout<<"Connect Succeed"<<endl;  
   }
   //下面发送数据，以QUIT结束
    string mess;
    char messBuff[MESS_SIZE];

   while(true)
   {
     cout<<"请输入发送的信息：";
     memset(messBuff,0,MESS_SIZE);
     getline(cin,mess);
    //判断是否结束
    if(strcmp(mess.c_str(),"QUIT")==0)
    {
     write(clientSock,mess.c_str(),mess.size()+1);
     close(clientSock);
     return 0;
     }
    write(clientSock,mess.c_str(),mess.size()+1);
    //接着读取服务器返回的信息
    read(clientSock,messBuff,MESS_SIZE);
    //输出信息
    cout<<"Server Answer: ";
    cout<<messBuff<<endl;
   }
 
  return 0;
}
