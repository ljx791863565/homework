#include "complex.h"
#include <iostream>
using namespace std;


int main() 
{
	Complex a(2, 3);
	Complex b(3, 2);

	cout << a.Module() << endl;
	cout << b.Module() << endl;

	a.show();
	b.show();


	Complex c;
	c = a + b;
	c.show(); 

	c = a - b;
	c.show();

	c = a * b;
	c.show();

	c = a / b;
	c.show();

	bool test = a == b;
	if (test == true) {
		cout << "a == b" << endl;
	}else {
		cout << "a != b" << endl;
	}
}
