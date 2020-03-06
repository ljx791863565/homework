#ifndef __STRING__H__
#define __STRING__H__
#include <cstring>
#include <iostream>
#include <cstdlib>
using namespace std;
class String
{
public:
	String();//默认构造函数
	String(const char *str);//以char字符数组为参数的构造
	String(const String &s);//拷贝构造函数
	~String();//析构函数

	//运算符重载
	String& operator=(const char *str);
	String& operator=(const String &s);
	char& operator[](int index);
	
	//友元函数	
	friend String& operator+(const String &lhs, const String &rhs);
	friend std::ostream& operator<<(std::ostream &os, const String &s);
	friend std::istream& operator>>(std::istream &in, const String &s);
	friend bool operator==(const String &lhs, const String &rhs);
	friend bool operator!=(const String &lhs, const String &rhs);
	friend bool operator>(const String &lhs, const String &rhs);
	friend bool operator<(const String &lhs, const String &rhs);
	int size();
	bool empty();

private:
	char *_str;
	int _len;
};
#endif //__STRING__H__
