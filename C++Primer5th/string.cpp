#include <iostream>

char * fun(char *str, char ch)
{
	char *itr1 = str;
	char *itr2 = str;
	while (*itr2 != '\0'){
		while (*itr2 == ch){
			++itr2;
		}
		*itr1++ = *itr2++;
	}
	return itr1;
}
int main()
{
	char str[15] = "asadssscsd";
	std::cout << fun(str, 's')<<  std::endl;

	return 0;
}
