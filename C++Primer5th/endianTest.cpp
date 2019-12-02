#include <iostream>

int main()
{
	int a = 0x12345678;
	char *p = (char*)&a;

	if (*p == 0x12){
		std::cout << "Big endian" << std::endl;
	}else {
		std::cout << "small endian" << std::endl;
	}
	return 0;
}
