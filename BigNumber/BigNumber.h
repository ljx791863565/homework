#ifndef __BIG_NUMBER_H__
#define __BIG_NUMBER_H__
#include <string.h>
#include <iostream>
#include <assert.h>
class BigNumber
{
public:
	BigNumber()
		:m_data(NULL){}
	~BigNumber();
	BigNumber(const char *str);
	BigNumber(const BigNumber &n);
	BigNumber(const char ch, int n);
	
	ostream& operator<<()
	
private:
	char *m_data;		//保存大数
};
#endif
