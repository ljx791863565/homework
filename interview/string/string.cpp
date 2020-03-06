#include "string.h"

using namespace std;
String::String() : _len(0)
{
	_str = new char[1];
	_str[0] = '\0'; 
}

String::~String()
{
	delete[] _str;
	_len = 0;
}

String::String(const String &s)
{
	if (!empty()){
		_str = new char[1];
		_str[0] = '\0';
		_len = 0;
	}
	_len = s._len;
	_str = new char[_len+1];
	strcpy(_str, s._str);
}

String::String(const char *str)
{
	if (str == NULL){
		_str = new char[1];
		_str[0] = '\0';
		_len = 0;
	}
	_len = strlen(str);
	_str = new char[_len+1];
	strcpy(_str, str);
}

String& String::operator=(const String &s)
{
	if (this == &s)
		return *this;
	_len = s._len;
	delete _str;
	_str = new char[_len+1];
	strcpy(_str, s._str);

	return *this;
}

String& String::operator=(const char *str)
{
	delete[] _str;
	_len = strlen(str);
	_str = new char[_len+1];
	strcpy(_str, str);
	
	return *this;
}

char& String::operator[](const int index)
{
	if (index < _len)
		return _str[index];

}

String& operator+(const String &lhs, const String &rhs)
{
	String tmp;
	delete[] tmp._str;
	tmp._len = lhs._len + rhs._len;
	tmp._str = new char[tmp._len+1];
	strcpy(tmp._str, lhs._str);
	strcat(tmp._str, rhs._str);

	return tmp;
}

ostream& operator<<(ostream &os, const String &s)
{
	os << s._str;
	return os;
}

/*
istream& operator>>(istream &in, String &s)
{
	char tmp[1024];
	memset(tmp, 0, sizeof(tmp));
	in >> tmp;
	if (s._str != NULL){
		delete[] s._str;
	}

	s._str = new char[strlen(tmp)+1];
	strcpy(s._str, tmp);
	return in;
}
*/

bool operator==(const String &lhs, const String &rhs)
{
	if ( 0 == strcmp(lhs._str, rhs._str)){
		return true;
	}else{
		false;
	}
}

bool operator!=(const String &lhs, const String &rhs)
{
	return !(lhs == rhs);
}

bool operator>(const String &lhs, const String &rhs)
{
	if (strcmp(lhs._str, rhs._str) > 0){
		return true;
	}else if (strcmp(lhs._str, rhs._str) < 0){
		return false;
	}
}

bool operator<(const String &lhs, const String &rhs)
{
	return !(lhs > rhs);
}

int String::size()
{
	return _len;
}
bool String::empty()
{
	return _len == 0 ? true : false;
}



















