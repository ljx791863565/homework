#include <iostream>

using namespace std;

int main()
{
	try{
		char *mystring;
		mystring = new char[10];
		//在try中规定条件，当条件不符合时就throw出异常，可以是任何类型的数据，也可以是一个常量
		if (mystring == NULL) throw "allocation failure";
		for (int i = 0; i< 100; i++){
			if (i > 9) throw i;		//当throw后会立刻退出try语块，在try语块生成的任何对象都会被销毁
			mystring[i] = 'z';
		}
	}
	//catch要紧跟着try语块，不可以之间插入任何语句
	//catch出上面抛出的异常，括号里面可以是你throw出的数据类型，或任何有效的数据类型









	catch(int i){
		cout << "index" << i << "is out of range" << endl;
	}
	catch(char ch){
		cout << "Execption: " << ch << endl;
	}
	

	return 0;
}
