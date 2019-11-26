/***************************
Copyright(C), 2019, zbt
Author:ljx
Version:1.0.0
Date:2019.11.26
***************************/
#ifndef __BIG_NUMBER_H__
#define __BIG_NUMBER_H__

#include <iostream>
#include <string.h>
#include <assert.h>

using namespace std;

class BigNumber
{
	char* m_data;
	int m_len;
public:
	BigNumber();
	~BigNumber();
	BigNumber(const BigNumber &b);
	BigNumber(const char *str);
	BigNumber(char ch, int n);
	friend ostream& operator<<(ostream &out, BigNumber &b);
	friend const BigNumber& operator=(const BigNumber &b);
	friend BigNumber& operator+(const BigNumber &r, const BigNumber &l);
};


#endif
