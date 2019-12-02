#include <iostream>

class Base{

	int a;

public: 

	Base():a(0){}

//	~Base(){}

	void set(int value)

	{

		a = value; 

	}

	int get()

	{

		return a;

	} 

	virtual void display()
	{
	}

};

class Son : public Base

{

	int b;

public:  

	Son():b(0){
		Base();
	}
	Son(int value){
		b = value;
	}

	~Son(){

	}
	virtual void display(){

		std::cout << "this is Son " << "a = " << get() << " b = "<< b << std::endl;

	}

}; 

class Daughter : public Base{

	int c;

public:

	Daughter():c(0){

		Base();

	}
	Daughter(int value){
		c = value;
	}

	~Daughter(){



	}
	virtual void display(){

		std::cout << "this is Daughter " << "a = " << get() << " c = " << c << std::endl;

	}

};



int main()

{

	Base b;

	b.set(5);

	Son s(7);

	Daughter d(10);

	Base *ps = &s;

	Base *pd = &d;
	
	ps->set(6);
	ps->display();
	
	pd->set(8);
	pd->display(); 

	return 0;

}
