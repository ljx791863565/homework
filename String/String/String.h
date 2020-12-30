#ifndef String_String_h
#define String_String_h
#include <string.h>

class String{
public:
    //默认构造函数
    String();
    //带参构造函数
    String(const char *str);
    //析构函数
    ~String();
    //拷贝构造
    String(const String &obj);
    //复制初始化
    String &operator=(const String &obj);
public:
    //把后面字符串加在*this上
    String &operator+=(const String &obj);
    //把后面字符串加在前面
    friend String operator+(const String &obj,const String &obj1);
    //==判断字符串是否相等
    bool operator==(const String &str)const;
    //!=判断字符串是否相等
    bool operator!=(const String &str)const;
    //[]下标运算符重载
    char operator[](int index);
    //<<运算符重载
    friend std::ostream &operator<<(std::ostream &os,const String &obj);
    //>>运算符重载
    friend std::istream &operator>>(std::istream &is,const String &obj);    
    //返回字符串长度
    int leng()const;
    //取从position所指位置连续取len个字符组成子串返回
    String& subString(int position,int len);
private:
    char *_str;
    int _len;
};


#endif
