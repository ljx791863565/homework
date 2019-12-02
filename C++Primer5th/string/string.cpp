#include "string.h"

String::String()
{
	m_str = new char[1];
	*m_str = '\0';
	m_count = 0;
}

String::String(const char *str)
{
	if (str == NULL){
		cout << "str is null" << endl;
	} 
	m_count = strlen(str);
	m_str = new char[m_count+1];
	if (m_str == NULL){
		cout << "new error" << endl;
	}
	strcpy(m_str, str);
}

String::String(const String& s)
{
	m_count = s.m_count;
	m_str = new char[m_count +1];
	strcpy(m_str, s.m_str);
}

String::~String()
{
	if (m_str != NULL){
		delete m_str;
		m_count = 0;
	}
}

int String::size() const
{
	return m_count;
}

bool String::empty() const
{
	return m_count == 0;
}

ostream& operator<<(ostream& output, const String &s)
{
	output << s.m_str;
	return output;
}

String& String::operator=(const String& s)
{
	if (this == &s)
		return *this;
	m_count = s.m_count;
	delete m_str;
	m_str = new char[m_count+1];
	strcpy(m_str, s.m_str);

	return *this;
}

String& String::operator=(const char *str)
{
	m_count = strlen(str);
	delete m_str;
	m_str = new char[m_count +1];
	strcpy(m_str, str);
	return *this;
}

char& String::operator[](const int index)
{
	if (index > m_count){
		return *(m_str + m_count);
	}else{
		return *(m_str + index);
	}
}

String& String::operator+(const String &s)
{
	m_count = s.m_count + m_count;
	char *ptr = new char[m_count + 1];
	strcpy(ptr, m_str);
	strcat(ptr, s.m_str);
	delete m_str;
	m_str = ptr;

	return *this;
}

bool String::operator==(const String& s){}
bool String::operator!=(const String& s){}
bool String::operator>(const String& s){}
bool String::operator<(const String& s){}
