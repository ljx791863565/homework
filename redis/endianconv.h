#ifndef _ENDIANCONV_H_
#define _ENDIANCONV_H_ 

#include <stdint.h>
//大小端转换 
//在不做平台移植的情况下 用不上
void memrev16(void *p);
void memrev32(void *p);
void memrev64(void *p);

uint16_t intrev16(uint16_t v);
uint32_t intrev32(uint32_t v);
uint64_t intrev64(uint64_t v);

#define memrev16ifbe(p)
#define memrev32ifbe(p)
#define memrev64ifbe(p)
#define intrev16ifbe(v) (v)
#define intrev32ifbe(v) (v)
#define intrev64ifbe(v) (v)
#endif
