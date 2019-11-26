#include "BigNumber.h"
BigNumber::BigNumber()
{
	m_data = new char[1];
	*m_data = '\0';
	m_len = 0;
}

BigNumber::~BigNumber()
{
	assert(m_data != NULL);
	delete []m_data;
	m_data = NULL;
	m_len = 0;
}

BigNumber::BigNumber(const BigNumber &b)
{
	m_len = b.m_len;
	m_data = new char[m_len +1];
	strcpy(m_data, b.m_data);
}

BigNumber::BigNumber(const char *str)
{
	assert(str != NULL);
	m_len = strlen(str);
	m_data = new char[m_len +1];
	strcpy(m_data, str);
}

BigNumber::BigNumber(char ch, int n)
{
	m_len = n;
	m_data = new char[n+1];
	int i;
	for (i=0; i< n; i++)
	{
		m_data[i] = ch;
	}
}

ostream& operator<<(ostream &out, BigNumber &b)
{
	out << b.m_data ;
	return out;
}
/*
BigNumber& operator=(const BigNumber &b)
{
	if (this == &b)
		return *this;
	delete m_data;
	m_len = b.len;
	m_data = new char[m_len +1];
	strcpy(m_data, b.m_data);
	return *this;
}
*/
BigNumber& operator+(const BigNumber &r, const BigNumber &l)
{
	int max = r.m_len >= l.m_len ? r.m_len : l.m_len;
	int min = r.m_len <= l.m_len ? r.m_len : l.m_len;
	int i = 0;
	int temp[max+1];
	for (i=0; i<max+1; i++){
		temp[i] = 0;
	}
	for (; min>=0; min--){
		temp[min] = r.m_data[min] - '0' + l.m_data[min] - '0';
	}
	if (r.m_len > l.m_len){
		for (i = 0; i < max - min; i++){
			temp[i] = r.m_data[i] - '0';
		}
	}else {
		for (i = 0; i < max - min; i++){
			temp[i] = l.m_data[i] = 0;
		}
	}

	for (i=max+1; i>=0; i--){
		if(temp[i] >= 10){
			temp[i] -= 10;
			temp[i-1] +=1;
		}
	}
	BigNumber sum('0', max+1);
	for (i = 0; i < max+1; i++){
		sum.m_data[i] = temp[i] - '0';
	}

	return sum;
}
