#include <iostream>

using namespace std;

class CDummy{
public:
	static int count;
	CDummy()
	{
		count++;
	}
	~CDummy()
	{
		count--;
	}
};

int CDummy::count = 0;

int main()
{
	CDummy a;
	CDummy *c = new CDummy;
	CDummy d[5];
	cout << CDummy::count << endl;
	delete c;

	cout << CDummy::count << endl;

	return 0;
}
