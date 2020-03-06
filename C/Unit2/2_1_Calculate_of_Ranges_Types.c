#include <stdio.h>

/*
   ~0 0默认设置为int型，占4字节，储存为 0x00000000， ~0为0xffffffff
   sizeof(int) >= 2 sizeof(char) = 1 sizeof(short) = 2 sizeof(int) = 4 sizeof(long) = 8
   转换为unsigned char 储存为0xff 
   >>1向右移动以为，左端补0，既除以二以保持符号性
   */
int main()
{
	/*signed*/
	printf("signed char max is %d\n", (char)((unsigned char)~0 >> 1));
	printf("signed char min is %d\n", -(char)((unsigned char)~0 >> 1));

	printf("signed int max is %d\n", (int)((unsigned int)~0 >> 1));
	printf("signed int min is %d\n", -(int)((unsigned int)~0 >> 1));

	printf("signed short int max is %d\n", (short)((unsigned short)~0 >> 1));
	printf("signed short int min is %d\n", -(short)((unsigned short)~0 >> 1));

	printf("signed long int max is %ld\n", (long)((unsigned long)~0 >> 1));
	printf("signed long int min is %ld\n", -(long)((unsigned long)~0 >> 1));

	/*unsigned*/
	printf("unsigned char max is %u\n", (unsigned char)~0);
	printf("unsigned short int max is %u\n", (unsigned short)~0);
	printf("unsigned int max is %u\n", (unsigned int)~0);
	printf("unsigned long int max is %lu\n", (unsigned long)~0);

	return 0;
}
