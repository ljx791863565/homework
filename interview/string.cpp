#include <iostream>
#include <string.h>
class My_string
{

	My_string()
	{
		m_data = new char[1];
		*m_data = '\0';
		m_len = 0;
	}

	~My_string()
	{
		m_len = 0;
		if (m_data) {
			delete  m_data;
		}
	}
	My_string(const My_string &s)
	{
		m_len = s.m_len;
		m_data = new char[m_len + 1];
		strcpy(m_data, s.m_data);
	}

	My_string(const char *str)
	{
		m_len = strlen(str);
		m_data = new char [m_len + 1];
		strcpy(m_data, str);
	}
	
	My_string& operator=(const My_string &s)
	{
		if (&s == this) {
			return *this;
		}
		delete m_data;
		m_len = s.m_len;
		m_data = new char [m_len +1];
		strcpy(m_data, s.m_data);

		return *this;
	}
private:
	char *m_data;
	int m_len;
};
