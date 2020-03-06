#include <stdio.h>
#include <limits.h>

/*determine ranges of types*/

int main()
{
	printf("signed int Max range is %d , Min range is %d\n", INT_MAX, INT_MIN);
	printf("signed char Max range is %d , Min range is %d\n", SCHAR_MAX, SCHAR_MIN);
	printf("signed short int Max range is %d , Min range is %d\n", SHRT_MAX, SHRT_MIN);
	printf("signed long int Max range is %ld , Min range is %ld\n", LONG_MAX, LONG_MIN);
	printf("unsigned char Max range is %u\n", UCHAR_MAX);
	printf("unsigned int Max range is %u\n", UINT_MAX);
	printf("unsigned short int Max range is %u\n", USHRT_MAX);
	printf("unsigned long int Max range is %lud\n", ULONG_MAX);

	return 0;
}
