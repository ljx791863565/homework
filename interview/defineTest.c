#define X 3
#define Y X*2
#undef X
#define X 2

#include <stdio.h>
int main()
{
	int y = Y;
	printf("%d\n", y);
	
}
