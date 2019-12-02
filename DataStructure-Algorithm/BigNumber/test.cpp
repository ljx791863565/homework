#include "BigNumber.h"

int main()
{
	BigNumber b1;
	BigNumber b2("123456");
	BigNumber b3(b2);
	BigNumber b4('6', 6);

	std::cout << b1 << std::endl;
	std::cout << b2 << std::endl;
	std::cout << b3 << std::endl;
	std::cout << b4 << std::endl;
	
	BigNumber a("111111");
	BigNumber b("222222222");

//	BigNumber c = a + b;
	std::cout << a + b << std::endl;
	return 0;
}
