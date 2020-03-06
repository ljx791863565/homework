#ifndef __STRING2__H__
#define __STRING2__H__
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
class String
{
public:
	String(const char *str = NULL);
	String(const String& rhs);
	~String();

	String operator+(const String& rhs) const;
	String& operator=(const String& rhs);
	String& operator+=(const String& rhs);
	bool operator==(const String& rhs) const;
	char& operator[](int n) const;

	size_t size() const;
	const char* c_str();

	friend istream& operator>>(istream& is, String& rhs);
	friend istream& operator<<(istream& os, String& rhs);
private:
	char *data;
	size_t length;
}
#endif 
