#include <iostream>
#include "String.h"
using namespace std;
int main (int argc, const char * argv[])
{
    String string="abc";//调用带参构造
    String string1="abd";//调用带参构造   
    String string2(string);//调用拷贝构造
    string2=string1;//调用=运算符重载
    string2+=string;//调用+=运算符重载
    String str=string+string1;//string+string1调用+运算符重载,然后String str调用带参构造
    if(string!=string1){//比较是否不相等
        cout<<"两个字符串不相等"<<endl;
    }else{
        cout<<"两个字符串相等"<<endl;
    }
    cout<<str[0]<<endl;//调用下标运算符重载
    cout<<string<<endl;//调用<<运算符重载
    cout<<"请输入字符串："<<endl;
    cin>>str;//调用>>运算符重载
    cout<<str<<endl;
    int len=string2.leng();//得到长度
    cout<<"string2的长度为："<<len<<endl;
    cout<<string.subString(1, 2)<<endl;
    return 0;
}

