#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	char str[100];
	int i, n;
	long *l;

	cout <<"type in a number" << endl;
	cin.getline(str, 100);
	i = atoi(str);
	l = new long[i];
	for (n = 0; n < i; n++){
		cout << "enter number" << endl;
		cin.getline(str, 100);
		l[n] = atoi(str);
	}

	cout << "end" << endl;
	for (n = 0; n < i; n++){
		cout << l[n] << endl;
	}

	delete [] l;

	return 0;
}
