#include <stdio.h>

#define BIT3 (0x1 << 3)
static int a;
void set_bit3()
{
	a |= BIT3;
}

void clear_bit3()
{
	a &= ~BIT3;
}
