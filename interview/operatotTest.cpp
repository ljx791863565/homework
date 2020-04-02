#include <iostream>

using namespace std;

int main(int argc, const char *argv[])
{
	int x = 2, y, z;
	x *= (y = z = 5);	//将5赋值给z 将z赋值给y x*=y  x=10
	cout << x << endl;
	z = 3;
	x == (y = z);		//将z赋值给y 比较x==y x不变 x=10
	cout << x << endl;
	x = (y == z);		//比较y和z值 结果赋值给x x=0
	cout << x << endl;
	x = (y & z);		//y和z按位与 结果赋值给x x=1
	cout << x << endl;
	x = (y && z);		//y和z与操作 结果赋值给x x=1
	cout << x << endl;
	y = 4;
	x = (y | z);		//y和z按位或 结果赋值给x x=7
	cout << x << endl;
	x = (y || z);		//y和z或操作 结果赋值给x x=1
	cout << x << endl;
	return 0;
}
