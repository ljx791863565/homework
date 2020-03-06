#include "complex.h"

int main()
{
	Complex x1(2, 6);
	Complex x2(3, 5);

	x1.Show();
	x2.Show();
	
	Complex x3 = x1;
	x3.Show();

	x3.SetReal(3);
	x3.Show();

	Complex x4 = x1 + x2;
	cout << "x1 + x2  = ";
	x4.Show();

	Complex x5 = x1 - x2;
	cout << "x1 - x2  = ";
	x5.Show();

	Complex x6 = x1 * x2;
	cout << "x1 * x2  = ";
	x6.Show();

	Complex x7 = x1 / x2;
	cout << "x1 / x2  = ";
	x7.Show();
	return 0;
}
