#include <iostream>
#include <stdlib.h>
using namespace std;

template <typename T>
inline void show(T &a, T &b)
{
	cout << "a = " << a << " " << "b = " << b << endl;
}

//最普通的交换函数
template <typename T>
inline void myswap(T &a, T &b)
{
	T tmp;
	tmp = a;
	a = b;
	b = tmp;
}


//不用临时变量的交换函数
//缺陷是如果超出数据的上限，结果不准确
template <typename T>
inline void myswap2(T &a, T&b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}

//更精确的交换函数
//但是不可以用在非整型的数据中，float和double的储存数据方式不一样
template <typename T>
inline void myswap3(T &a, T &b)
{
	a = a ^ b;	//异或，相异为1 相同为0
	b = a ^ b;
	a = a ^ b;
}
int main()
{
	int a, b;
	cout << "输入两个整数" << endl;
	cin >> a >> b;
	
	show(a, b);
	myswap3(a, b);
	show(a, b);

	return 0;
}
