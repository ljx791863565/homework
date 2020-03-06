#include "string2.h"

String::String(const char *str)
{
	if (!str){
		length = 0;
		data = new char[1];
		*data = '\0';
	}else{
		length = strlen(str);
		data = new char[length +1];
		strcpy(data, str);
	}
}

String::String(const String& rhs)
{
	length = rhs.size();
	data = new char[length +1];
	strcpy(data, rhs.c_str());
}

String::~String()
{
	delete[] data;
	length = 0;
}

String::String operator+(const String& rhs) const
{
	String news;
	news.length = length + rhs.size();
	data = new char[news.length + 1];
	strcpy(news.data, data);
	strcat(news.data, rhs.data);

	return news;
}

String::String& operator=(const String& rhs)
{
	if (this == rhs){
		return *this;
	}
	delete[] data;
	length = rhs.length;
	data = new char[length + 1];
	strcpy(data, rhs.c_str());
	return *this;
}

String::String& operator+=(const String& rhs)
{
	length += rhs.length;
	char *newData = new char[length +1];
	strcpy(newData, data);
	strcat(newData, rhs.data);
	delete[] data;
	data = newData;

	return *this;
}

inline bool String:: operator==(const String& rhs) const 
{
	if (length != rhs.length){
		return false;
	}
	return strcmp(data, rhs.data) ? false : true;
}

inline char& String:: operator[](int n) const
{
	if (n > length){
		return data[length -1];
	}else{
		return data[n];
	}
}

inline size_t Sting::size() const
{
	return length;
}

inline const char* String::c_str() const
{
	return data;
}

istream& operator>>(istream& is, String& rhs)
{
	char tmp[100];
	is >> tmp;
	rhs.length = strlen(tmp);
	rhs.data = new char[length + 1];
	strcpy(rhs.data, tmp);

	return is;
}

istream& operator<<(istream& os, String& rhs)
{
	os << rhs.data;
	return os;
}


