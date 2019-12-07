#include <stdio.h>

int main()
{
	int a[10] = {
		13, 12, 43, 54, 3, 45, 209, 22, 98, 6		
	};

	BubbleSort(a, 10);
	display(a, 10);
	return 0;
}
