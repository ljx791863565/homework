#include "BigNumber.h"

int main()
{
	BigNumber b1();
	BigNumber b2("1234");
	BigNumber b3(b1);
	BigNumber b4('2', 8);


	std::cout << "b1 :" << b1 << std::endl; 
	std::cout << "b2 :" << b2 << std::endl; 
	std::cout << "b3 :" << b3 << std::endl; 
	std::cout << "b4 :" << b4 << std::endl; 

	return 0;
}
