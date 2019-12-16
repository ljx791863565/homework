#include <iostream>

#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace std;

class shared{
private:
	shared_ptr<int> p;

public:
	shared(shared_ptr<int> p_):p(p_){

	}
	void print()
	{
		coot << "count:" << p.use_count() << "v = " << *p << endl;
	}
};

void print_func(shared_ptr<int> p)
{
	cout << "count:" << p.use_count() << "v = " << *p << endl;
}

int main()
{
	shared_ptr<int> p1(new int(100));
	shared s1(p1);
	shaerd s2(p1);

	s1.print();
	s2.print();
	*p1 = 20;

	print_func(p1);
	s1.print();

	return 0;
}

