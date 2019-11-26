#include <iostream>
//#include <string>
#include "String.h"
const int maxLength=128;//字符串的最大长度
//默认构造函数
String::String(){
    _len=0;
    _str=new char[1];
}
//带参构造函数
String::String(const char *str){
    _str=new char[maxLength+1];
    if(!_str)
    {
        cerr<<"Allocation Error!\n";
        exit(1);
    }
    _len=strlen(str);
    strcpy(_str,str);
}
//析构函数
String::~String(){
    delete _str;
    _str=NULL;
}
//拷贝构造
String::String(const String &obj){
    _str=new char[maxLength+1];
    if(!_str)
    {
        cerr<<"Allocation Error!\n";
        exit(1);
    }
    _len=obj._len;
    strcpy(_str,obj._str);
}
//复制初始化
String &String::operator=(const String &obj){
    _str=new char[maxLength+1];
    if(!_str)
    {
        cerr<<"Allocation Error!\n";
        exit(1);
    }
    _len=obj._len;
    strcpy(_str,obj._str);    
    return *this;        
}
//把后面字符串加在前面
String &String::operator+=(const String &obj){
    char *temp=_str;//将*this->ch赋给temp
    _len=_len+obj._len;//*this->curLen为两字符串的总长度
    _str=new char[_len+1];
    if(!_str)
    {
        cerr<<"Out of memory!\n";
        exit(1);
    }
    strcpy(_str,temp);
    strcat(_str,obj._str);
    delete[] temp;
    return *this;
}
//把后面字符串加在前面
String operator+(const String &obj,const String &obj1){
    int len=obj._len+obj1._len;//两个长度加起来
    char *str0=new char[len+1];
    if(!str0)
    {
        cerr<<"Out of memory!\n";
        exit(1);
    }
    strcpy(str0,obj._str);
    strcat(str0,obj1._str);
    String str(str0);
    delete[] str0;
    return str;
}
//==判断字符串是否相等
bool String::operator==(const String &str)const{
    if(strcmp(_str, str._str)==0){
        return true;
    }else{
        return false;
    }    
}
//!=判断字符串是否相等
bool String::operator!=(const String &str)const{
    if(strcmp(_str, str._str)!=0){
        return true;
    }else{
        return false;
    }
}
//[]下标运算符重载
char String::operator[](int index){
    return _str[index];
}
//<<运算符重载
ostream &operator<<(ostream &os,const String &obj){
    os<<obj._str;
    return os;
}
//>>运算符重载
istream &operator>>(istream &is,const String &obj){
    is>>obj._str;
    return is;
}
//返回字符串长度
int String::leng()const{
    return _len;
}
//取从position所指位置连续取len个字符组成子串返回
String& String::subString(int position,int len){
    if(position<0||position+len-1>=maxLength||len<0) //参数不合理，不取子串
    {
        _len=0;
        _str[0]='\0';
    }
    else
    {
        if(position+len-1>=_len)                         //字符串不够长
            len=_len-position;
        _len=len;
        for(int i=0,j=position;i<len;i++,j++)
            _str[i]=_str[j];
        _str[len]='\0';
    }
    return *this;
}
