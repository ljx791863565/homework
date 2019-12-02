#ifndef __STRING__
#define __STRING__
#include <iostream>
#include <cstring>

using namespace std;

class String
{
	char *m_str;
	int m_count;

public:
	String();
	String(const char *str);
	String(const String &s);
	~String();

	int size() const;
	bool empty() const;
	String& operator=(const String &s);
	String& operator=(const char *str);
	String& operator+(const String &s);
	char& operator[](const int index);
	bool operator==(const String &s);
	bool operator!=(const String &s);
	bool operator>(const String &s);
	bool operator<(const String &s);
	friend ostream& operator <<(ostream &output, const String &s);
	
};
#endif
