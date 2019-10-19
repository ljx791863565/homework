#include <iostream>
#include <unistd.h>
int main()
{
	std::cout << "hello world " << std::endl;
	sleep(100);
	std::cout << "bye bye" << std::endl;
}
