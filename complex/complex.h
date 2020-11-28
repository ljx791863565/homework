#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <stdlib.h>
#include <math.h>

class Complex
{
private:
	double re;
	double im;
public:
	Complex(double r, double i)
	{
		re = r;
		im = i; 
	}
	
	//纯虚数
	Complex(double i)
	{
		re = 0;
		im = i;
	}

	Complex()
	{
		re = im = 0;
	}
	
	double Module()
	{
		return sqrt(re*re + im*im);
	}

	Complex conjugate()
	{
		Complex temp;
		temp.im = -im;
		temp.re = re;
		return temp;
	}

	double get_re()
	{
		return re;
	}

	double get_im()
	{
		return im;
	}

	void set_re(double r)
	{
		re = r;
	}

	void set_im(double i)
	{
		im = i;
	}

	friend Complex operator+(const Complex &a, const Complex &b);
	friend Complex operator-(const Complex &a, const Complex &b);
	friend Complex operator*(const Complex &a, const Complex &b);
	friend Complex operator/(const Complex &a, const Complex &b);

	friend bool operator==(const Complex &a, const Complex &b);
	friend bool operator!=(const Complex &a, const Complex &b); 

	void show(); 
};


#endif //__COMPLEX_H__
