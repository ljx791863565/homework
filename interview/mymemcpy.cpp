#include <stdio.h>
#include <assert.h>
#include <iostream>
//void *memcpy(void *dest, const char *src, unsigned int len);

//缺点没有考虑到内存重叠的情况
void *myMemcpy1(void *dest, const void *src, size_t len)
{
	assert((dest != NULL) && (src != NULL));

	unsigned char *psrc = (unsigned char *)src;
	unsigned char *pdest = (unsigned char *)dest;
	
	while (len-- > 0)
		*pdest++ = *psrc++;

	return dest;
}

//考虑到内存重叠的情况
void *myMemcpy2(void *dest, const void *src, size_t len)
{
	if (dest == NULL || src == NULL) {
		return NULL;
	}
	char *pdest = static_cast <char *>(dest);
	const char *psrc = static_cast <const char *>(src);
	int n = len;

	//内存重叠时 从后向前复制
	if (pdest > psrc && pdest < psrc + n) {
		for (size_t i = n-1; i != -1; i--) {
			pdest[i] = psrc[i];
		}
	}else {
		for (size_t i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	}
	return dest;

}

//大部分认为memcpy是一个char到char的拷贝的循环，担心它的效率。
//实际上，memcpy是一个效率最高的内存拷贝函数，他不会那么傻，来做一个一个字节的内存拷贝，
//在地址不对齐的情况下，他是一个字节一个字节的拷，地址对齐以后，就会使用CPU字长来拷（和dma类似），
//32bit或64bit，还会根据cpu的类型来选择一些优化的指令来进行拷贝。
//总的来说，memcpy的实现是和CPU类型、操作系统、cLib相关的。毫无疑问，它是内存拷贝里效率最高的，请放心使用。

//以32位系统为例
void *myMemcpy3(void *dest, const void *src, size_t n)
{
	assert((dest != NULL) && (src != NULL));
	//按32bit 4字节copy
	int wordlen = n / 4;
	//剩余的字节
	int slice = n % 4;

	int *pIntSrc = (int *)src;
	int *pIntDest = (int *)dest;

	while (wordlen --) {
		*pIntDest++ = *pIntSrc++;
	}
	while (slice--) {
		*((char *)pIntDest++) = *((char *)pIntSrc++);
	}

	return dest;
}
