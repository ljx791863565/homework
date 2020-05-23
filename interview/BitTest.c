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

#define bwMCDR2_ADDRESS 4
#define bsMCDR2_ADDRESS 17
int BIT_MASK (int _bf)
{
	return ((1U<<(bw##_bf))-1)<<(bs##_bf);
}
void SET_BITS(int _dst,int _bf,int _val)
{
	_dst=((_dst) & ~(BIT_MASK(_bf))) | (((_val)<<<(bs##_bf))&(BIT_MASK(_bf)));
}
SET_BITS(MCDR2,MCDR2_ADDRESS,RegisterNumber);

/*
#define bwMCDR2_ADDRESS 4
#define bsMCDR2_ADDRESS 17
#define bmMCDR2_ADDRESS BIT_MASK(MCDR2_ADDRESS)
#define BIT_MASK(_bf)(((1U<<(bw##_bf))-1)<<(bs##_bf)
#define SET_BITS(_dst,_bf,_val)\
		((_dst)=((_dst)& ~(BIT_MASK(_bf)))| \
		 (((_val)<<(bs##_bf))&(BIT_MASK(_bf))))
SET_BITS(MCDR2,MCDR2_ADDRESS,RegisterNumber);
*/

int main(int argc, const char *argv[])
{
	int a = 0x9;

	return 0;
}
