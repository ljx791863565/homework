#include "endianconv.h"
#include <stdio.h>

//将*p指向的16位unsigned integer 从 littlt endian 切换成big endian
void menrev16(void *p)
{
	unsigned char *x = p, t;

	t = x[0];
	x[0] = x[1];
	x[1] = t;
}

void memrev32(void *p)
{
	unsigned char *x = p, t;
	t = x[0];
	x[0] = x[3];
	x[3] = t;

	t = x[1];
	x[1] = x[2];
	x[2] = t;
}

void memrev64(void *p)
{
	unsigned char *x = p, t;
	t = x[0];
	x[0] = x[7];
	x[7] = t;

	t = x[1];
	x[1] = x[6];
	x[6] = t;
	
	t = x[2];
	x[2] = x[5];
	x[5] = t;

	t = x[3];
	x[3] = x[4];
	x[4] = t;
}

uint16_t intrev16(uint16_t v)
{
	memrev16(&v);
	return v;
}
uint32_t intrev32(uint32_t v)
{
	memrev32(&v);
	return v;
}
uint64_t intrev64(uint64_t v)
{
	memrev64(&v);
	return v;
}
