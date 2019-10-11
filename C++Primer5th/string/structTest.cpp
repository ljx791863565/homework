#include <iostream>

/*
struct A{
	int value()
	{
		return 1;
	}
};
*/

//sizeof(A) = 1
struct A{
};

//sizeof(B) = 1
struct B{
	int value();
};

//sizeof(C) = 16
struct C{
	virtual ~C();
	int x;
};

//sizeof(D) = 8
struct D{
	virtual ~D();
	virtual int value();
};

//sizeof(E) = 16
struct E : public C{
	virtual ~E();
};

//sizeof(F) = 24	
struct F : public C, public D{
	virtual ~F();
};



int main()
{
	A *const p1 = new A;
	struct A *p2 = new A;

	std::cout << "sizeof(int) = " << sizeof(int) << std::endl;
	std::cout << "sizeof(A) = " << sizeof(A) 
		<< " sizeof(B) =" << sizeof(B)
		<< " sizeof(C) =" << sizeof(C)
		<< " sizeof(D) =" << sizeof(D)
		<< " sizeof(E) =" << sizeof(E)
		<< " sizeof(F) =" << sizeof(F) << std::endl;

//	std::cout << p1->value() - p2->value() << std::endl;
//	p1 = p2;
}
