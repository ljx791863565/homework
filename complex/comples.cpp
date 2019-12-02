#include "comples.h"
#include <iostream>

Comples Comples::operator+(Comples &a, Comples &b) 
{
	Comples temp;
	temp.re = a.re + b.re;
	temp.im = a.im + b.im;

	return &temp;
}

void Comples::show()
{
	cout << this.re << " + " << this.im << "i" << endl;  
}
