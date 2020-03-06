#ifndef __MY_STRING__H__
#define __MY_STRING__H__
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;
class myString
{
public:
	MyString();
	~MyString();
	MyString(const MyString &s);
	MyString(const char *str);

	String& operator=(const MyString &s);
	String& operator=(const char *str);
	char& operator[](int index);

	int size();
	bool empry();
private:
	char * m_data;
	int m_len;
}
#endif 
