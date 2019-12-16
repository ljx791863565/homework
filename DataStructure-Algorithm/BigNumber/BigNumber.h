/*************************************************************************
  > File Name: BigNumber.h
  > Author: ljx
  > Mail: liaojia138797@163.com 
  > Created Time: Mon 16 Dec 2019 02:29:40 PM CST
 ************************************************************************/
#ifndef __BIGNUM__H__
#define __BIGNUM__H__

#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

typedef long long INT_64;
const long long INT_64_MAX = 0x7fffffffffffffff;	//9223372036854775807
const long long INT_64_MIN = 0x8000000000000000;	//-9223372036854775808
class BigNum
{

	friend ostream & operator<<(ostream &os, BigNum &big);

	friend BigNum operator+(const BigNum &left, const BigNum &right);
	friend BigNum operator-(const BigNum &left, const BigNum &right);
	friend BigNum operator*(const BigNum &left, const BigNum &right);
	friend BigNum operator/(const BigNum &left, const BigNum &right);

	friend string Mul(const string &left, const string &right);
	friend string Div(const string &left, const string &right);


public:
	BigNum(INT_64 value);
	BigNum(const char *num_str = "");
	BigNum(const BigNum &big);
	BigNum & operator=(const BigNum &big);

private:	
	bool IsINT64Overflow() const;

private:
	string _strData;
	INT_64 _value;
};

string Add(const string &left, const string &right);
string Sub(const string &left, const string &right);
string Mul(const string &left, const string &right);
string Div(const string &left, const string &right);

bool IsLeftBig(const char *left, int lSize, const char *right, int rSize);
char SubLoop(char *left, int lSize, const char *right, int *rSize);


#endif //__BIGNUM__H__
