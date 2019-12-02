#ifndef SHELLFUNSET_H_INCLUDED
#define SHELLFUNSET_H_INCLUDED

#endif // SHELLFUNSET_H_INCLUDED

#include "CmdControl.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include <stdio.h>

class ShellFunSet{
 public:
  bool pwd(string order[]){//获取当前工作路径
    char buffer[100];
    getcwd(buffer,sizeof(buffer));
    cout<<"当前工作路径："<<buffer<<endl;
    return true;
  }
  bool dir(string order[]){
    DIR *dir;
    dirent *ptr;
    //string dir_input = order[2];
    const char *dir_input = order[2].c_str();
    //cout<<dir_input<<"]"<<endl;
    dir = opendir(dir_input);
    //cout<<"dir="<<dir<<"]"<<endl;
    if(dir != NULL){
        while((ptr = readdir(dir)) != NULL){
          printf("%s\n",ptr->d_name);
        }
    }
    else cout<<"不存在该目录："<<dir_input<<endl;
    //cout<<"dir:功能开发中"<<endl;
    return true;
  }
  bool cd(string order[]){
     const char *dir_input = order[2].c_str();
     int fd = open(dir_input,O_RDONLY);
     if(fchdir(fd) != -1) cout<<"改变当前路径到："<<getcwd(NULL,NULL)<<endl;
     else cout<<"不存在该路径："<<dir_input<<endl;
     close(fd);
     return true;
  }
  bool newdir(string order[]){
    if(mkdir(order[2].c_str(),S_IRUSR) == -1)
      cout<<"newdir:ERROR"<<endl;
    return true;
  }
  bool deldir(string order[]){
    if(rmdir(order[2].c_str()) == -1)
      cout<<"deldir:ERROR"<<endl;
    return true;
  }
  bool exit(string order[]){
    return false;
  }
  bool date(string order[]){
    cout<<"date:功能开发中"<<endl;
    return true;
  }
  bool rename(string order[]){
    cout<<"rename:功能开发中"<<endl;
    return true;
  }
  bool find(string order[]){
    cout<<"find:功能开发中"<<endl;
    return true;
  }
};
