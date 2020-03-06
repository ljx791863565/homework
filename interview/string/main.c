#include "string.h"

int main()
{
	String s1 = "def";
	String s2(s1);

	cout << "s1 : " << s1 << endl;
	cout << "s2 : " << s2 << endl;

	String s3 = s1;
	String s4;
	s4 = s1 + s2;
	cout << "s3 : " << s3 << endl;
	cout << "s4 : " << s4 << endl;
	cout << "s3[2] : " << s3[2] << endl;

	String s5 = "we";
	if (s1 == s5){
		cout << "s1 == s5";
	}else {
		cout << "s1 != s5";
	}

	

}
