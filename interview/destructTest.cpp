#include <iostream>

using namespace std;

class A
{
public:
	A()
	{
		cout << "this is  A" << endl;
	}
	~A()
	{
		cout << "destruct A" << endl;
	}
};
class B
{
public:
	B()
	{
		cout << "this is  B" << endl;

	}
	~B()
	{
		cout << "destruct B" << endl;
	}
};
class C
{
public:

	C()
	{
		cout << "this is  C" << endl;

	}
	~C()
	{
		cout << "destruct C" << endl;
	}
};
class D
{
public:
	D()
	{
	
		cout << "this is  D" << endl;
	}
	~D()
	{
		cout << "destruct D" << endl;
	}
};

//c是全局变量，所以调用C构造函数在main函数之前，所以调用析构函数在最后
C c;
int main()
{
	//pa调用new 所以在堆中，需要手动析构
	A *pa = new A();
	//b在栈中，main函数结束后自动调用B的析构函数
	B b;
	
	static D d;
	delete pa;		//此处delete将调用A的析构函数
}
