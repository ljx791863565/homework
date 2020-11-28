#include "complex.h"
#include <iostream>
#include <assert.h>

Complex operator+(const Complex &a, const Complex &b) 
{
	Complex temp;
	temp.re = a.re + b.re;
	temp.im = a.im + b.im;

	return temp;
}

Complex operator-(const Complex &a, const Complex &b) 
{
	Complex temp;
	temp.re = a.re - b.re;
	temp.im = a.im - b.im;

	return temp;
}

Complex operator*(const Complex &a, const Complex &b) 
{
	Complex temp;
	temp.re = a.re * a.re - a.im * b.im;
	temp.im = a.re * b.im + b.re * a.im;

	return temp;
}

Complex operator/(const Complex &a, const Complex &b) 
{
	assert((b.re != 0) && (b.im != 0));
	Complex temp;
	double ret = b.re * b.re + b.im * b.im;
	temp.re = (a.re * b.re + b.im * b.im ) / ret;
	temp.im = (a.im * b.re - a.re * b.im) / ret;

	return temp;
}

bool operator==(const Complex &a, const Complex &b)
{
	return ((a.im == b.im) && (a.re == b.re));
}

bool operator!=(const Complex &a, const Complex &b)
{
	return !(a==b);
}

void Complex::show()
{
	std::cout << re << " + " << im << "i" << std::endl;  
}
