#include "string.h"

int main()
{
	String s;
	String s1("hello");
	String s2(s1);
	String s3 = s1;
	String s4 = "world";
	String s5 = s1 + s4;
	

	cout << "s:" << s << endl;
	cout << "s1:" << s1 << endl;
	cout << "s2:" << s2 << endl;
	cout << "s3:" << s3 << endl;
	cout << "s4:" << s4 << endl;
	cout << "s5:" << s5 << endl;
	cout << s5[3] << endl;
	return 0;
}
