
#include "string.h"
#include <iostream>

using namespace std;

int main()
{
	String s1;
	s1 = "abs";
	{
		String s2(s1);
		s2  += s1;
		cout << "s2: " << s2 << endl;
	}

	String s3(s1);
	cin >> s3;
	cout << "s3 : " << s3 << endl;
	cout << "s1 : " << s1 << endl;

	String s4 = s1 + s3;
	cout << "s4 : " << s4 << endl;
}
