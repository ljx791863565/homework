/*
   复数 a + bi
   定义 i * i = -1;
   */

#ifndef __COMPLEX__H__
#define __COMPLEX__H__
#include <iostream>

using namespace std;
class Complex
{
private:
	int real;	//复数的实数部分
	int imag;	//复数的虚数部分
public:
	Complex()
		:real(0), imag(0)
	{
	}
	Complex(int a, int b)
		:real(a), imag(b)
	{
	}
	Complex(const Complex &c)
		:real(c.real), imag(c.imag)
	{
	}
	void SetReal(int r);
	void SetImag(int i);
	int GetReal();
	int GetImag();
	void Show();
	Complex operator=(const Complex &c);
	friend Complex  operator+(const Complex &left, const Complex &right);
	friend Complex  operator-(const Complex &left, const Complex &right);
	friend Complex  operator*(const Complex &left, const Complex &right);
	friend Complex  operator/(const Complex &left, const Complex &right);
};
	void Complex::SetReal(int r)
	{
		real = r;
	}
	void Complex::SetImag(int i)
	{
		imag = i;
	}
	int Complex::GetReal()
	{
		return real;
	}
	int Complex::GetImag()
	{
		return imag;
	}
	void Complex::Show()
	{
		cout << real << "+" << imag << "i" << endl;
	}
	Complex Complex::operator=(const Complex &c)
	{	
		Complex tmp;
		tmp.real = c.real;
		tmp.imag = c.imag;
		return tmp;
	}
	Complex operator+(const Complex &left, const Complex &right)
	{
		// (a + bi) + (c + di) = (a + c) + (b + d)i
		Complex c;
		c.real = left.real + right.real;
		c.imag = left.imag + right.imag;	

		return c;
	}
	Complex operator-(const Complex &left, const Complex &right)
	{
		// (a + bi) - (c + di) = (a - c) + (b - d)i 
		Complex c;
		c.real = left.real - right.real;
		c.imag = left.imag - right.imag;	

		return c;
	}
	Complex operator*(const Complex &left, const Complex &right)
	{
		// (a + bi) * (c + di) = (ac - bd) + (bc + ad)i
		Complex c;
		c.real = left.real*right.real - (left.imag * right.imag);
		c.imag = left.imag * right.real + left.real * right.imag;	

		return c;
	}
	Complex operator/(const Complex &left, const Complex &right)
	{
		//	(a + bo) / (c + di) = ((ac+bd) / (c^2 + d^2)) + ((bc - ad) / (c^2 + d^2))i
		//	c != 0 && d != 0
		Complex c;
		if (right.real == 0 && right.imag == 0){
			cout << "底数为0 不能相除" << endl;
			return c;
		}
		int denominator = right.real * right.real + right.imag * right.imag;
		c.real = (left.real * right.real + left.imag * right.imag) / denominator;
		c.imag = (left.imag * right.real - (left.real * right.imag)) / denominator;	

		return c;
	}
#endif 
