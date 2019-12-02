#ifndef CMDCONTROL_H_INCLUDED
#define CMDCONTROL_H_INCLUDED

#endif // CMDCONTROL_H_INCLUDED
#include<iostream>
#include<deque>
using namespace std;

template <class T>
struct CmdNode{
  typedef bool (T::*pFun)(string *);//该指针所指函数均以字符串数组为参数
  string command;                   //并返回布尔值，CmdControl类以该值判断是否结束对程序的控制
  pFun fun;                         //若函数返回false则表示执行该函数后停止对程序的控制
};

template <class T>
class CmdControl {
 protected:
  T holder;
  string prompt;
  string cmdNotExist;
  int cmds;
  bool running;
  CmdNode<T> *cmdList;
  string* getOrder();//返回输入的命令及其参数，用字符串的数组分别存储
  int match(string order){
    for(int i = 0;i < cmds;i++){
      if(cmdList[i].command == order) return i;
    }
    return -1;
  }
 public:
  void run() {
    running = true;
    while(running){
      cout<<prompt<<":";
      string *orderInput = getOrder();
      //cout<<(int)orderInput[1].at(0)<<endl;
      if(orderInput[1].length() == 0) continue;//用户仅只敲了个回车
      //将加工后的参数数据传递给相应函数
      int funNum = match(orderInput[1]);
      if(funNum == -1) cout<<cmdNotExist<<endl;
      else running = (holder.*(cmdList[funNum].fun))(orderInput);
    }
  }
  //接收参数:命令条数、命令持有类、命令与函数的关联数组
  CmdControl(string prmpt,int num,T Holder,CmdNode<T> *CMD){
    prompt = prmpt;
    cmds = num;
    holder = Holder;
    cmdList = CMD;
    running = false;
    cmdNotExist = "不存在该指令";
  }
};

template <class T>
string* CmdControl<T>::getOrder(){
  string order;
  getline(cin,order);
  int num = 1;
  deque<string> orders;
  int from = 0;
  int len = 1;
  for(int i = 0; i < order.size();i++){
    if(order[i] == ' '){
      orders.push_back(order.substr(from,len-1));
      from = i+1;
      len = 1;
      num++;
    }
    else len++;
  }orders.push_back(order.substr(from));
  string *result = new string[num+1];
  result[0].append(1,(char)num);//0号字符串存储的是数量信息
  deque<string>::iterator itr = orders.begin();
  for(int i = 1;i <= num; i++){
    result[i] = *itr;
    itr++;
  }
  //for(int i =0; i <= int(result[0].at(0)); i++) cout<<i<<":"<<result[i]<<endl;
  return result;
}
