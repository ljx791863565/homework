#include "BigNumber.h"

BigNumber::~BigNumber()
{
	delete [] m_data;
	m_data = NULL;
}

BigNumber::BigNumber(const char *str)
{
	if (str == NULL){
		m_data = new char[1];
		*m_data = '\0';
	}
	m_data = new char[strlen(str) * sizeof(char)];
	strcpy(m_data, str);
}

BigNumber::BigNumber(const BigNumber &n)
{
	m_data = new char[strlen(n.m_data) * sizeof(char)];
	strcpy(m_data, n.m_data);
}

BigNumber::BigNumber(const char ch, int n)
{
	assert(n != 0);
	m_data = new char[n * sizeof(char)];
	int i;
	for (i = 0; i < n; i++){
		m_data[i] = ch;
	}
	m_data[n] = '\0';
}


