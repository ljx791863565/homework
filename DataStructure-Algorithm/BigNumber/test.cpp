/*************************************************************************
    > File Name: test.cpp
    > Author: ljx
    > Mail: liaojia138797@163.com 
    > Created Time: Mon 16 Dec 2019 02:49:15 PM CST
 ************************************************************************/

#include "BigNumber.h"

int main()
{

	//BigNum b1("8329487777777777742836842777777");
	BigNum b1("7");
	cout << b1 << endl;

	/*
	BigNum b2("-888800000000000000000000000002");
	cout << b2 << endl;

	BigNum b3 = b2;
	cout << b3 << endl;
	
	BigNum b4(1);
	cout << b4 << endl;

	BigNum b5(1234);
	cout <<b5 << endl;
	
	BigNum b6(-1234);
	cout <<b6 << endl;

	BigNum b7(-123456);
	cout <<b7 << endl;
	
	BigNum b11 = b4 + b5;
	cout << "b4 + b5 = " << b4 << "+" << b5 << b11 << endl;
	BigNum b12 = b4 - b5;
	cout << "b4 - b5 = " << b4 << "-" << b5 << b12 << endl;
	BigNum b13 = b6 - b7;
	cout << "b6 - b7 = " << b6 << "-" << b7 << b13 << endl;
	BigNum b14 = b6 + b7;
	cout << "b6 - b7 = " << b6 << "+" << b7 << b14 << endl;
*/


	BigNum a1("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
	cout << a1 << endl;
	BigNum a2 = a1 + b1;
	cout << a2 << endl;
/*
	BigNum a3("         88388777777777777777777777777");
	cout << a3 << endl;

	BigNum a4("000000000000748375739fdf453535353535345");
	cout << a4 <<endl;
*/
	return 0;
}
