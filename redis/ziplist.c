/*
 * ziplist 压缩表 一系列特殊编码的内存块构成的链表
 * Ziplist 是为内存占用而特别优化的双链表。
 *
 * 它可以保存字符串和整数，其中整数以整数类型而不是字符串来进行编码和保存。
 *
 * 对 ziplist 的两端进行 push 和 pop 的复杂度都为 O(1) ，
 * 不过，因为对 ziplist 的每次修改操作都需要进行内存重分配，
 * 因此，实际的时间复杂度与 ziplist 使用的内存大小有关。
 *
 * ----------------------------------------------------------------------------
 *
 * 以下是 ziplist 的内存结构：
 *
 * <zlbytes><zltail><zllen><entry><entry><zlend>
 *
 * <zlbytes> 是一个无符号整数(uint32_t)，用于记录整个 ziplist 所占用的字节数量。
 * 通过保存这个值，可以在不遍历整个 ziplist 的前提下，对整个 ziplist 进行内存重分配。
 *
 * <zltail> 是到列表中最后一个节点的偏移量(同样为 uint32_t)。
 * 有了这个偏移量，就可以在常数复杂度内对表尾进行操作，而不必遍历整个列表。
 *
 * <zllen> 是节点的数量，为 ``uint16_t`` 。
 * 当这个值大于 2**16-2 时，需要遍历整个列表，才能计算出列表的长度
 *
 * <zlend> 是一个单字节的特殊值，等于 255 ，它标识了列表的末端。
 *
 * ZIPLIST ENTRIES:
 *
 * Ziplist 中的每个节点，都带有一个 header 作为前缀。
 *
 * Header 包括两部分：
 * 1) 前一个节点的长度，在从后往前遍历时使用
 * 2) 当前节点所保存的值的类型和长度
 *
 * 前一个节点的长度的储存方式如下：
 * 1) 如果节点的长度 < 254 字节，那么直接用一个字节保存这个值。
 * 2) 如果节点的长度 >= 254 字节，那么将第一个字节设置为 254 ，
 *    再在之后用 4 个字节来表示节点的实际长度（共使用 5 个字节）。
 *
 * 另一个 header 域保存的信息取决于这个节点所保存的内容本身。
 *
 * 当节点保存的是字符串时，header 的前 2 位用于指示保存内容长度所使用的编码方式，
 * 之后跟着的是内容长度的值。
 *
 * 当节点保存的是整数时，header 的前 2 位都设置为 1 ，
 * 之后的 2 位用于指示保存的整数值的类型（这个类型决定了内容所占用的空间）。
 *
 * 以下是不同类型 header 的概览：
 * 
 * |00pppppp| - 1 byte
 *      String value with length less than or equal to 63 bytes (6 bits).
 *      长度 <= 63 字节(6 位)的字符串值
 * |01pppppp|qqqqqqqq| - 2 bytes
 *      String value with length less than or equal to 16383 bytes (14 bits).
 *      长度 <= 16383 字节(14 位)的字符串值
 * |10______|qqqqqqqq|rrrrrrrr|ssssssss|tttttttt| - 5 bytes
 *      String value with length greater than or equal to 16384 bytes.
 *      长度 >= 16384 字节， <= 4294967295 的字符串值，
 *
 * |11000000| - 1 byte
 *      Integer encoded as int16_t (2 bytes).
 *      以 int16_t (2 字节)类型编码的整数
 * |11010000| - 1 byte
 *      Integer encoded as int32_t (4 bytes).
 *      以 int32_t (4 字节)类型编码的整数
 * |11100000| - 1 byte
 *      Integer encoded as int64_t (8 bytes).
 *      以 int64_t (8 字节)类型编码的整数
 * |11110000| - 1 byte
 *      Integer encoded as 24 bit signed (3 bytes).
 *      24 位(3 字节)有符号编码整数
 * |11111110| - 1 byte
 *      Integer encoded as 8 bit signed (1 byte).
 *      8 位(1 字节)有符号编码整数
 * |1111xxxx| - (with xxxx between 0000 and 1101) immediate 4 bit integer.
 *      Unsigned integer from 0 to 12. The encoded value is actually from
 *      1 to 13 because 0000 and 1111 can not be used, so 1 should be
 *      subtracted from the encoded 4 bit value to obtain the right value.
 *      (介于 0000 和 1101 之间)的 4 位整数，可用于表示无符号整数 0 至 12 。
 *      因为 0000 和 1111 都已经被占用，因此，可被编码的值实际上只能是 1 至 13 ，
 *      要将这个值减去 1 ，才能得到正确的值。
 * |11111111| - End of ziplist.
 *      ziplist 的终结符
 */
//-----------------------------------------------------------------------------------------
//每个节点可以保存一个长度受限制的字符数组(没有\0结尾)或整数
//字符数组	-- 长度 <= 63 (2^6-1)
//			-- 长度 <= 16383 (2^14 -1)
//			-- 长度 <= 4294967295 (2^32 -1)
//整数		-- 4位长  0-12之间的无符号整数
//			-- 1字节长 有符号整数、
//			-- 3字节长 有符号整数
//			-- int64_t类型整数
//			-- int32_t类型整数
//			-- int16_t类型整数
//所有整数都以小端表示
//
//ziplist结构
//area		|<--  ziplist hander -->|<--		entries			 -->|<-end->|
//
//size		4bytes	4bytes	2bytes		？		？		?		?	1bytes
//			-----------------------------------------------------------------
//			|zlbytes|zltail	|zllen	|entry1	|entry2	| ...	|entryN	|zlend	|
//			-----------------------------------------------------------------
//address							|						|		| ZIPLIST_ENTRY_END
//								ZIPLIST_ENTRY_HEAD			|
//														ZIPLIST_ENTRY_TAIL
//			ZIPLIST_ENTRY_HEAD  ziplist第一个节点开始的位置
//			ZIPLIST_ENTRY_TAIL	ziplist最后一个节点开始的位置
//			ZIPLIST_ENTRY_END	ziplist末端位置 zlend开始位置
//
//各结构info
//			zlbytes uint32_t	整个ziplist占用内存字节数
//			zltail	uint32_t	到达ziplist节点的偏移量
//			zllen	uint16_t	ziplist中节点数量
//			entry..  ?			ziplist节点 长度根据类型决定
//			zlend	uint8_t		1111 1111 255 用于标记ziplist结尾
//
//			ziplist hander长度是固定的 4+4+2 = 10字节
//
//entry结构
//			|<--		entry						 -->|
//			---------------------------------------------
//			|pre_entry_legth | encoding	|length	|content|
//			---------------------------------------------
//各结构info
//			pre_entry_legth		记录前一个节点长度 如果指针指向这个entry开头 
//								需要访问前一个entry 直接让指针 - pre_entry_legth即可跳转到前一个entry
//								pre_entry_legth有两种编码方式，即两种不同大小
//								1字节	如果前一个节点长度 <254字节 直接用一个字节长度保存它
//								5字节	如果前一个节点长度 >254字节 用5个字节长度保存它 并将第一个字节值设为254 1111 1110
//										用接下来4字节保存实际长度 左边多余的位置用0补完
//								
//								erntry：前一个节点长128  使用1bite
//								-------------------------------------------------
//								|pre_entry_legth	|encoding	|length	|content|
//								|					|			|		|		|
//								|1000 0000			|			|		|		|
//								-------------------------------------------------
//								|	1byte			|
//
//								erntry：前一个节点长10086 使用5bytes
//								-------------------------------------------------------------------------
//								|pre_entry_legth							|encoding	|length	|content|
//								|											|			|		|		|
//								|11111110 00000000000000000010011101100110	|			|		|		|
//								-------------------------------------------------------------------------
//								|	1	|				4bytes				|
//
//			encoding &length	2bit	encoding值可以是 00 01 10 11 
//								00 01 10表示content保存的是字符数组
//								
//								00bbbbbb										1bytes  长度 <= 63字节的字符数组		2^6 -1		
//								01bbbbbbxxxxxxxx								2bytes	长度 <= 16383字节的字符数组		2^14 -1
//								10______aaaaaaaabbbbbbbbccccccccdddddddd		5bytes	长度 <= 4294967295字节的字符数组	2^32 -1
//								_ 表示留空 x a b c d 表示实际的二进制数据 
//								
//								11		表示content保存的是整数
//								11000000										1bytes	int16_t类型整数
//								11010000										1bytes	int32_t类型整数
//								11100000										1bytes	int64_t类型整数
//								11110000										1bytes	24bit有符号整数
//								11111110										1bytes	8bit有符号整数
//								1111xxxx										1bytes	4bit有符号整数	0-12之间 
//																		(介于 0000 和 1101 之间)的 4 位整数，可用于表示无符号整数 0 至 12 
//																		 因为 0000 和 1111 都已经被占用，因此，可被编码的值实际上只能是 1 至 13
//																		 要将这个值减去 1 ，才能得到正确的值。
//			
//			content				保存节点内容 类型和长度由encoding 和 length决定
//								数据采用二进制形式保存
//													
//								以下两个例子content位置均为了方便阅读没有采用二进制形式
//
//														2bytes		6bytes	11bytes
//								------------------------------------------	-------------
//								|pre_entry_legth		|encoding	|length	|content	|
//								|						|			|		|			|
//								|?						|00			|001011	|hello world|
//								---------------------------------------------------------
//								？ 暂时不知道上一节点信息
//								encoding 00表示是字符数组
//								length	 001011 content精确长度 11
//								content	 保存数据本身
//								
//														2bytes		6bytes	2bytes
//								------------------------------------------	-------------
//								|pre_entry_legth		|encoding	|length	|content	|
//								|						|			|		|			|
//								|?						|11			|000000	|10086		|
//								---------------------------------------------------------
//								？ 暂时不知道上一节点信息
//								encoding 11表示保存的信息是整数
//								length	 000000表示 int16_t类型 
//								content  10086 保存数据本身
//
//Ziplist 是为内存占用而特别优化的双链表
//Ziplist 可以保存字符串或者整数 其中整数以整数类型来编码和保存的
//			一个空ziplist
//			
//size		4bytes	4bytes	2bytes	1bytes
//			-------------------------------------
//			|zlbytes|zltail	|zllen	|zlend		|
//			|		|		|		|			|
//			|1011	|1010	|0		|11111111	|
//			-------------------------------------
//									|
//								ZIPLIST_ENTRY_HEAD & ZIPLIST_ENTRY_TAIL & ZIPLIST_ENTRY_END
//	
//			zlbytes		11bytes 空表占用总长度
//			zltail		因为是空表 所以表尾和hander的尾端在同一位置 没有任何entry
//			zllen		0
//			zlend		结尾标识
//
//ziplist创建后就可以添加entyr了
//有两种方式
//			* 将节点添加到ziplist末端	新节点是最后一个节点
//			* 将节点添加到某个节点前面 新节点后至少有一个节点
//
//			末端添加
//			** 记录到达ziplist末端的偏移量
//			** 根据节点的值的格式和大小判断内存大小 前一个节点的大小判断pre_entry_legth 分配空间
//			** entyr各项属性 encoding length content
//			** ziplist各项属性 zlbytes zltai zllen
//
//			
//			要添加的新节点为 "hello world" 长度为11字节字符串 
//			要插入只有一个节点的ziplist上 原表中的一个节点大小为5  首先要定位 ZIPLIST_ENTRY_END
//size		4bytes	4bytes	2bytes	5bytes			1bytes
//			-------------------------------------------------
//			|zlbytes|zltail	|zllen	|entry1		|zlend		|
//			|		|		|		|	?		|			|
//			|10000	|1010	|1		|			|11111111	|
//			-------------------------------------------------
//									|			|			
//									|		ZIPLIST_ENTRY_END
//					ZIPLIST_ENTRY_HEAD & ZIPLIST_ENTRY_TAIL
//
//			然后计算节点总长度 11 + 1 + 1 = 13字节 11字节保存数据 1字节是encoding 00 + length 001011 1字节是pre_entry_legth 5
//			计算ziplist属性 
//size		4bytes	4bytes	2bytes	5bytes		13bytes				1bytes
//			---------------------------------------------------------------------
//			|zlbytes|zltail	|zllen	|entry1		|entry2				|zlend		|
//			|		|		|		|	?		|pre_entry_legth	|			|
//			|11011	|1111	|10		|			|101				|11111111	|
//			|		|		|		|			|					|			|
//			|		|		|		|			|encoding			|			|
//			|		|		|		|			|00					|			|
//			|		|		|		|			|					|			|
//			|		|		|		|			|length				|			|
//			|		|		|		|			|001011				|			|
//			|		|		|		|			|					|			|
//			|		|		|		|			|content			|			|
//			|		|		|		|			|hello world		|			|
//			---------------------------------------------------------------------
//									|			|					|		
//						ZIPLIST_ENTRY_HEAD		|					|
//												|				ZIPLIST_ENTRY_END
//										ZIPLIST_ENTRY_TAIL
//
//-------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include "ziplist.h"
#include "zmalloc.h"
#include "util.h"
unsigned char *ziplistNew(void);				


#define ZIP_END 255		//zlend 标记ziplist结尾
#define ZIP_BIGLEN 254

//用来判断是字符串编码还是整数编码
#define ZIP_STR_MASK 0xc0		//1100 0000
#define ZIP_INT_MASK 0x30		//0011 0000

#define ZIP_STR_06B (0 << 6)	//00000000
#define ZIP_STR_14B (1 << 6)	//01000000
#define ZIP_STR_32B	(2 << 6)	//10000000

#define ZIP_INT_16B (0xc0 | 0 << 4)	//11000000
#define ZIP_INT_32B (0xc0 | 1 << 4)	//11010000
#define ZIP_INT_64B (0xc0 | 2 << 4)	//11100000
#define ZIP_INT_24B (0xc0 | 3 << 4)	//11110000
#define ZIP_INT_8B	0xfe			//11111110

//4bit 0-12无符号整数
#define ZIP_INT_IMM_MASK 0x0f	//00001111
#define ZIP_INT_IMM_MIN	0xf1	//11110001
#define ZIP_INT_IMM_MAX 0xfd	//11111101
#define ZIP_INT_IMM_VAL(v) (v & ZIP_INT_IMM_MASK)

#define INT24_MAX 0x7fffff
#define INT24_MIN (-INT24_MAX -1)

//判断是否是字符串 1 ：是字符串 0：整数
#define ZIP_IS_STR(enc) (((enc) & ZIP_STR_MASK) < ZIP_STR_MASK)

//取出ziplist中以字节计算的列表长度 (内存的 0-31 位)整数
#define ZIPLIST_BYTES(zl) ((*(uint32_t*)(zl)))
//取出ziplist的表尾偏移量 (内存 32-63位)整数
#define ZIPLIST_TAIL_OFFSET(zl) (*((uint32_t*)((zl) + sizeof(uint32_t))))
//取出ziplist的长度 (内存的 64-79位)整数
#define ZIPLIST_LENGTH(zl) (*((uint32_t*)((zl) + sizeof(uint32_t)*2)))
//取出ziplist header长度
#define ZIPLIST_HEADER_SIZE	(sizeof(uint32_t)*2 +sizeof(uint16_t))
//返回ziplist header之后的位置
#define ZIPLIST_ENTRY_HEAD(zl) ((zl) + ZIPLIST_HEADER_SIZE)
//返回ziplist 最后一个元素的开始位置
#define ZIPLIST_ENTRY_TAIL(zl) ((zl) + ZIPLIST_TAIL_OFFSET(zl))
//返回ziplist结束之前的位置
#define ZIPLIST_ENTRY_END(zl) ((zl) + ZIPLIST_BYTES(zl) -1)

//ZIPLIST_LENGTH(zl) 最大值为 UINT16_MAX
#define ZIPLIST_INCR_LENGRH(zl, incr) { \
	if (ZIPLIST_LENGTH(zl) < UINT16_MAX) \
		ZIPLIST_LENGTH(zl) = (ZIPLIST_LENGTH(zl) +incr); \
}

typedef struct zlentry{
	unsigned int prevrawlensize,		//编码前置节点所需要的长度
				 prevrawlen;			//前置节点的长度
	unsigned int lensize,				//编码当前节点所需要的长度
				 len;					//当前节点长度
	unsigned int headersize;			//hander 长度 等于 prerawlen + len
	unsigned char encoding;				//编码方式
	unsigned char *p;					//数据域
}zlentry;

//从ptr指针中取出 编码方式 并保存到encoding
#define ZIP_ENTRY_ENCODING(ptr, encoding) do { \
	(encoding) = (ptr[0]); \
	if ((encoding) < ZIP_STR_MASK) (encoding) &= ZIP_STR_MASK; \
} while (0)

//返回 encoding 指定的编码方式需要的长度
//O(1)
static unsigned int  zipIntSize(unsigned char encoding)
{
	switch (encoding)
	{
		case ZIP_INT_8B : return 1;
		case ZIP_INT_16B : return 2;
		case ZIP_INT_24B : return 3;
		case ZIP_INT_32B : return 4;
		case ZIP_INT_64B : return 8;
		default : return 0;
	}
	assert(NULL);
	return 0;
}

//编码长度1 并将它写入到p
//如果p为NULL 返回rawlen所需要的长度
static unsigned int zipEncodeLength(unsigned char *p, unsigned char encoding, unsigned	rawlen)
{
	unsigned char len = 1;
	unsigned char buf[5];

	//字符串编码方式 encoding 00 01 10
	if (ZIP_IS_STR(encoding)) {
		// rawlen长度 <=63 (2^6 -1)  需6bit长度length空间编码此字符串 长度+编码 8bit 1byte 00aaaaaa
		if (rawlen <= 0x3f) {
			if (!p){
				return len;
			}
			buf[0] = ZIP_STR_06B | rawlen;
			//长度<=16388 (2^14-1)  14bit 长度+编码 16bit 2bytes	01aaaaaa bbbbbbbb
		} else if (rawlen < 0x3fff) {
			len += 1;
			if (!p) {
				return len;
			}
			buf[0] = ZIP_STR_14B | ((rawlen >> 8) & 0x3f);
			buf[1] = rawlen & 0xff;
			//长度<= 4294967295 (2^32 -1) 32bit  长度+编码 40bit 5bytes  10______ aaaaaaaa bbbbbbbb cccccccc dddddddd
		}else {
			len += 4;
			if (!p) {
				return len;
			}
			buf[0] = ZIP_STR_32B;
			buf[1] = (rawlen >> 24) & 0xff;
			buf[3] = (rawlen >> 16) & 0xff;
			buf[4] = (rawlen >> 8) & 0xff;
			buf[4] = rawlen &0xff;
		}
	//整数编码方式 encoding = 11
	}else {
		if (!p) {
			return len;
		}
		buf[0] = encoding;
	}
	memcpy(p, buf, len);
	return len;
}
//从ptr指针中取出 节点的编码方式encoding 保存当前节点长度所需要的长度lensize 节点长度len
//O(1)
#define ZIP_DECODE_LENGTH(ptr, encoding, lensize, len) do { \
	ZIP_ENTRY_ENCODING((ptr), (encoding));	\
	if ((encoding) < ZIP_STR_MASK) { \
			if ((encoding) == ZIP_STR_06B){ \
				(lensize) = 1;	\
				(len) = (ptr)[0] & 0x3f;	\
			}else if ((encoding) == ZIP_STR_14B){ \
				(lensize) = 2;	\
				(len) = (((ptr)[0] & 0x3f) << 8) | (ptr)[1];	\
			}else if ((encoding) == ZIP_STR_32B) { \
				(lensize) = 5;	\
				(len) = ((ptr)[1] << 24) | ((ptr)[2] << 16) | ((ptr)[3] << 8) | ((ptr)[4]);		\
			} else { \
				assert(NULL);	\
			}\
	}else {	\
		(lensize) = 1;	\
		(len) = zipIntSize(encoding);\
	} \
}while (0);

//编码前置节点的长度， 并写入p
//p为NULL 返回编码len需要的字节数
//O(1)
static unsigned int zipPrevEncodingLength(unsigned char *p, unsigned int len)
{
	//1 or 5
	if (p == NULL){
		return (len < ZIP_BIGLEN) ? 1 : sizeof(len)+1;
	}else {
		if (len < ZIP_BIGLEN){
			p[0] = len;
			return 1;
		}else {
			p[0] = ZIP_BIGLEN;
			memcpy(p+1, &len, sizeof(len));
			return 1+sizeof(len);
		}
	}
}

//将前驱节点的长度len写入到p中
//p的空间要比保存len所需要的空间更大
static void zipPrevEncodingLengthForceLength(unsigned char *p, unsigned int len)
{
	if (p == NULL){
		return;
	}
	p[0] = ZIP_BIGLEN;
	memcpy(p+1, &len, sizeof(len));
}

//从指针ptr中取出保存前一个节点长度所需要的长度 prevlensize
//返回值 unsigned int
#define ZIP_DECODE_PREVLENSIZE(ptr, prevlensize) do { \
	if ((ptr)[0] < ZIP_BIGLEN){ \
		(prevlensize) = 1;	\
	}else {	\
		(prevlensize) = 5;	\
	}	\
} while(0);

//从指针ptr中取出前一个节点的长度 prevlen
//返回值 unsigned int
#define ZIP_DECODE_PREVLEN(ptr, prevlensize, prevlen) do { \
	ZIP_DECODE_PREVLENSIZE(ptr, prevlensize);	\
	if ((prevlensize) == 1){ \
		(prevlen) = (ptr)[0];	\
	} else if ((prevlensize) == 5) { \
		assert(sizeof((prevlensize)) == 4);	\
		memcpy(&(prevlen), ((char *)(ptr)) +1, 4);	\
	}\
} while (0);

//返回编码 len所需要的长度 - 编码p的前一个节点的大小所需要的长度 
static int zipPrevLenByteDiff(unsigned char *p, unsigned int len)
{
	unsigned int prevlensize;
	ZIP_DECODE_PREVLENSIZE(p, prevlensize);
	return zipPrevEncodingLength(NULL, len) - prevlensize;
}

//返回p指向节点的空间总长度
static unsigned int zipRawEntryLength(unsigned char *p)
{
	unsigned int prevlensize, encoding, lensize, len;
	ZIP_DECODE_PREVLENSIZE(p, prevlensize);
	ZIP_DECODE_LENGTH(p+prevlensize, encoding, lensize, len);
	return prevlensize + lensize + len;
}

//检测entry值是否能编码为整数
//如果可以 返回1 将值保存在v 编码方式保存在encoding
//否则 返回0
static int zipTryEncoding(unsigned char *entry, unsigned int entrylen, long long *v, unsigned char *encoding)
{
	long long value;
	//为空或超过长度 
	if (entrylen >= 32 || entrylen == 0){
		return 0;
	}
	//尝试转换为long long 类型
	if (string2ll((char*)entry, entrylen, &value)){
		if (value >= 0 && value <= 12){
			*encoding = ZIP_INT_IMM_MIN + value;
		}else if (value >= INT8_MIN && value <= INT8_MAX){
			*encoding = ZIP_INT_8B;
		}else if (value >= INT16_MIN && value <= INT16_MAX){
			*encoding = ZIP_INT_16B;	
		}else if (value >= INT24_MIN && value <= INT24_MAX){
			*encoding = ZIP_INT_24B;
		}else if (value >= INT32_MIN && value <= INT32_MAX){
			*encoding = ZIP_INT_32B;
		}else {
			*encoding = ZIP_INT_64B;
		}
		*v = value;
		return 1;
	}
	return 0;
}

//将value保存到p 并设置 encoding
static void zipSaveInteger(unsigned char *p, int64_t value, unsigned char encoding)
{
	int16_t i16;
	int32_t i32;
	int64_t i64;
	if (encoding == ZIP_INT_8B){
		((int8_t*)p)[0] == (int8_t)value;
	}else if (encoding == ZIP_INT_16B){
		i16 = value;
		memcpy(p, &i16, sizeof(i16));
	}else if (encoding == ZIP_INT_24B){
		i32 = value << 8;
		memcpy(p, ((uint8_t*)&i32)+1, sizeof(i32) - sizeof(uint8_t));
	}else if (encoding == ZIP_INT_32B){
		i32 = value;
		memcpy(p, &i32, sizeof(i32));
	}else if (encoding == ZIP_INT_64B){
		i64 = value;
		memcpy(p, &i64, sizeof(i64));	
	}else if (encoding >= ZIP_INT_IMM_MIN && encoding <= ZIP_INT_IMM_MAX){
		//do nothing, the value is stored in the encoding itself.
	}else {
		assert(NULL);
	}
}

//根据encoding 从p中取出整数值
static int64_t zipLoadInteger(unsigned char *p, unsigned char encoding)
{
	int16_t i16;
	int32_t i32;
	int64_t i64;
	int64_t ret;

	if (encoding == ZIP_INT_8B){
		ret = ((uint8_t*)p)[0];
	}else if (encoding == ZIP_INT_16B){
		mmecpy(&i16, p, sizeof(i16));
		ret = i16;
	}else if (encoding == ZIP_INT_24B){
		i32 = 0;
		memcpy(((uint8_t*)&i32)+1, p, sizeof(i32) - sizeof(uint8_t));
		ret = i32>>8;
	}else if (encoding == ZIP_INT_32B){
		memcpy(&i32, p, sizeof(i32));
		ret = i32;
	}else if (encoding == ZIP_INT_64B){
		memcpy(&i64, p, sizeof(i64));
		ret = i64;
	//不属于上面5中整数类型 那么其值和encoding将保存在一个byte
	}else if (encoding >= ZIP_INT_IMM_MIN && encoding <= ZIP_INT_IMM_MAX){
		ret = (encoding & ZIP_INT_IMM_MASK) - 1;
	}else {
		assert(NULL);
	}
	return ret;
}

//从p中取出entry属性 保存到zlentry结构中 并返回
static zlentry zipEntry(unsigned char *p)
{
	zlentry e;
	//取出前一个节点的长度 prevrawlen
	ZIP_DECODE_PREVLEN(p, e.prevrawlensize, e.prevrawlen);

	//取出当前节点的encoding lensize len
	ZIP_DECODE_LENGTH(p + e.prevrawlensize, e.encoding, e.lensize, e.len);

	//hander长度
	e.headersize = e.prevrawlensize + e.lensize;

	//指针p位置
	e.p = p;
	return e;
}

//创建一个空的ziplist
unsigned char *ziplistNew(void)
{
	//空ziplist需要分配 32bit + 32bit + 16bit 和一个8bit 空间
	//				  <zlbytes><zltail><zllen>     <zlend>	
	unsigned int bytes = ZIPLIST_HEADER_SIZE+1;
	unsigned char *zl = zmalloc(bytes);

	//设置长度属性
	ZIPLIST_BYTES(zl) = bytes;

	//设置表尾偏移量属性
	ZIPLIST_TAIL_OFFSET(zl) = ZIPLIST_HEADER_SIZE;

	//设置节点个数属性
	ZIPLIST_LENGTH(zl) = 0;

	//设置结束标识属性
	zl[bytes-1] = ZIP_END;

	return zl;
}

//对zl进行空间重新分配 并更新相关属性
static unsigned char *ziplistResize(unsigned char *zl, unsigned int len)
{
	//根据len重新分配zl空间
	zl = zrealloc(zl, len);

	//设置为新的bytes属性
	ZIPLIST_BYTES(zl) = len;

	//设置新的结束标识属性
	zl[len-1] = ZIP_END;

	return zl;
}

//将一个新节点添加到某个节点a之前 先做空间扩展工作
//
//如果a节点的prevlen不足以保持新节点的长度
//需要对a进行扩展 1byte -> 5bytes
//同样的假如a节点扩展了同样可能会引发a节点之后的节点prevlen不足以保存a节点长度
//这种情况在多个连续的节点长度都接近ZIP_BIGLEN时就可能发生
//
//同样因为节点长度缩小而引发的连续缩小也可能出现
//这种扩展-缩小-扩展-缩小现象反复出现 flapping 抖动
//
//所以redis不处理缩小情况 任由prevlen占用更大长度(5bytes) 只处理不够长度时的扩展情况
//
//zl	ziplist指针
//p		需要在前添加节点的entry指针、
//
//返回值 zl指针
static unsigned char *__ziplistCascadeUpdate(unsigned char *zl, unsigned char *p)
{
	size_t curlen = ZIPLIST_BYTES(zl);
	size_t rawlen, rawlensize, offset, noffset, extra;
	unsigned char *np;
	zlentry cur, next;

	//遍历到表尾ZIP_END处
	while (p[0] != ZIP_END){
		//取出当前的节点
		cur = zipEntry(p);
		//当前节点的长度
		rawlen = cur.headersize + cur.len;
		//编码当前节点所需要的空间大小
		rawlensize = zipPrevEncodingLength(NULL, rawlen);

		//已经是最后一个节点 不需要扩展空间
		if (p[rawlen] == ZIP_END)
			break;
		//不是最后一个节点 p节点后至少有一个节点  next指向下一个节点开始地址
		next = zipEntry(p+rawlen);
		
		//这里表示下一个节点的prevlen属性和当前节点总长度一致 即不需要扩展
		if (next.prevrawlen == rawlen)
			break;
		//下一个节点的prevrawlensize不足编码当前节点 需要扩展
		if (next.prevrawlensize < rawlensize){
			//先保存当zl开始到当前节点的长度
			offset = p - zl;
			//需要扩展的大小 5-1bytes
			extra = rawlensize - next.prevrawlensize;
			//扩展zl
			zl = ziplistResize(zl, curlen + extra);
			//将p指针重新指向扩展后的zl中当前节点的开始位置
			p = zl + offset;
			
			//保存当前节点结尾到zl开始的大小
			np = p + rawlen;
			noffset = np - zl;

			//当前节点的next节点不是最后一个节点 更新ZIPLIST_ENTRY_TAIL扩展后的位置
			if ((zl + ZIPLIST_TAIL_OFFSET(zl)) != np){
				ZIPLIST_TAIL_OFFSET(zl) = ZIPLIST_TAIL_OFFSET(zl) + extra;
			}
			
			//为获得空间而进行数据移动
			memmove(np+rawlensize, np+next.prevrawlensize, curlen-noffset-next.prevrawlensize-1);
			zipPrevEncodingLength(np, rawlen);
			p += rawlen;
			curlen += extra;
		}else {
			//下一个节点的长度编码大小有余 但不进行收缩操作
			//直接使用这个大小
			if (next.prevrawlensize > rawlensize){
				zipPrevEncodingLengthForceLength(p+rawlen, rawlen);
			}else {
				zipPrevEncodingLength(p+rawlen, rawlen);
			}
			break;
		}
	}
	return zl;
}

//从指针p开始 删除num个节点
//返回删除元素后的zl
static unsigned char *__ziplistDelete(unsigned char *zl, unsigned char *p, unsigned int num)
{
	unsigned int i, totlen, deleted = 0;
	size_t offset;
	int nextdiff = 0;
	zlentry first, tail;

	//第一个节点
	first = zipEntry(p);

	//获取所有要删除节点的总长度 并且移动p指针到要删除的最后一个节点的最后位置
	for (i = 0; p[0] != ZIP_END && i < num; i++){
		p += zipRawEntryLength(p);
		deleted++;
	}

	//需要删除的总bytes
	totlen = p - first.p;
	if (totlen > 0){
		if (p[0] != ZIP_END){
			//将要删除的最后一个节点的后一个节点的prevlensize设置为要删除的第一个节点的前一个节点的长度
			nextdiff = zipPrevLenByteDiff(p, first.prevrawlen);
			p -= nextdiff;
			zipPrevEncodingLength(p, first.prevrawlen);

			//更新ZIPLIST_TAIL_OFFSET 将其设置成删除后需要指向的位置 (是偏移量不是指针 所有需要相对于zl开始位置而更新)
			ZIPLIST_TAIL_OFFSET(zl) = ZIPLIST_TAIL_OFFSET(zl) - totlen;
			tail = zipEntry(p);
			if (p[tail.headersize + tail.len] != ZIP_END){
				ZIPLIST_TAIL_OFFSET(zl) = ZIPLIST_TAIL_OFFSET(zl) + nextdiff;
			}

			//移动数据
			memmove(first.p, p, ZIPLIST_BYTES(zl)-(p-zl)-1);
		}else {
			ZIPLIST_TAIL_OFFSET(zl) = (first.p-zl) - first.prevrawlen;
		}

		//调整ziplist大小 更新删除后的ziplist属性
		offset = first.p-zl;
		zl = ziplistResize(zl, ZIPLIST_BYTES(zl) - totlen + nextdiff);
		ZIPLIST_INCR_LENGRH(zl, -deleted);
		p = zl + offset;

		if (nextdiff != 0){
			zl = __ziplistCascadeUpdate(zl, p);
		}
	}
	return zl;
}

/* Insert item at "p". */
/*
 * 添加保存给定元素 s 的新节点到地址 p
 *
 * 复杂度：O(N^2)
 *
 * 返回值：删除元素后的 ziplist
 */
static unsigned char *__ziplistInsert(unsigned char *zl, unsigned char *p, unsigned char *s, unsigned int slen) {
    size_t curlen = ZIPLIST_BYTES(zl),
                    reqlen,
                    prevlen = 0;
    size_t offset;
    int nextdiff = 0;
    unsigned char encoding = 0;
    long long value = 123456789; /* initialized to avoid warning. Using a value
                                    that is easy to see if for some reason
                                    we use it uninitialized. */
    zlentry entry, tail;

    /* Find out prevlen for the entry that is inserted. */
    // 如果 p 之后不是没有节点（不是插入到末端）
    // 那么取出节点相关资料，以及 prevlen
    if (p[0] != ZIP_END) {
        entry = zipEntry(p);
        prevlen = entry.prevrawlen;
    } else {
        // 获取列表最后一个节点（表尾）的地址
        unsigned char *ptail = ZIPLIST_ENTRY_TAIL(zl);
        // 如果地址之后不是末端（也即是，列表至少有一个节点）
        if (ptail[0] != ZIP_END) {
            // 保存 ptail 指向的节点的空间长度
            prevlen = zipRawEntryLength(ptail);
        }
    }

    /* See if the entry can be encoded */
    // 查看能否将新值保存为整数
    // 如果可以的话返回 1 ，
    // 并将新值保存到 value ，编码形式保存到 encoding
    if (zipTryEncoding(s,slen,&value,&encoding)) {
        /* 'encoding' is set to the appropriate integer encoding */
        // s 可以保存为整数，那么继续计算保存它所需的空间
        reqlen = zipIntSize(encoding);
    } else {
        /* 'encoding' is untouched, however zipEncodeLength will use the
         * string length to figure out how to encode it. */
        // 不能保存为整数，直接使用字符串长度
        reqlen = slen;
    }
    /* We need space for both the length of the previous entry and
     * the length of the payload. */
    // 计算编码 prevlen 所需的长度
    reqlen += zipPrevEncodeLength(NULL,prevlen);
    // 计算编码 slen 所需的长度
    reqlen += zipEncodeLength(NULL,encoding,slen);

    /* When the insert position is not equal to the tail, we need to
     * make sure that the next entry can hold this entry's length in
     * its prevlen field. */
    // 如果添加的位置不是表尾，那么必须确定后继节点的 prevlen 空间
    // 足以保存新节点的编码长度
    // zipPrevLenByteDiff 的返回值有三种可能：
    // 1）新旧两个节点的编码长度相等，返回 0
    // 2）新节点编码长度 > 旧节点编码长度，返回 5 - 1 = 4
    // 3）旧节点编码长度 > 新编码节点长度，返回 1 - 5 = -4
    nextdiff = (p[0] != ZIP_END) ? zipPrevLenByteDiff(p,reqlen) : 0;

    /* Store offset because a realloc may change the address of zl. */
    // 保存偏移量，因为重分配空间有可能改变 zl 的内存地址
    offset = p-zl;
    // 重分配空间，并更新长度属性和表尾
    // 新空间长度 = 现有长度 + 新节点所需长度 + 编码新节点长度所需的长度差
    // O(N)
    zl = ziplistResize(zl,curlen+reqlen+nextdiff);
    // 更新 p 的指针
    p = zl+offset;

    /* Apply memory move when necessary and update tail offset. */
    // 如果新节点不是添加到列表末端，那么它后面就有其他节点
    // 因此，我们需要移动这部分节点
    if (p[0] != ZIP_END) {
        /* Subtract one because of the ZIP_END bytes */
        // 向右移动移原有数据，为新节点让出空间
        // O(N)
        memmove(p+reqlen,p-nextdiff,curlen-offset-1+nextdiff);

        /* Encode this entry's raw length in the next entry. */
        // 将本节点的长度编码至下一节点
        zipPrevEncodeLength(p+reqlen,reqlen);

        /* Update offset for tail */
        // 更新 ziplist 的表尾偏移量
        ZIPLIST_TAIL_OFFSET(zl) =
            intrev32ifbe(intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl))+reqlen);

        /* When the tail contains more than one entry, we need to take
         * "nextdiff" in account as well. Otherwise, a change in the
         * size of prevlen doesn't have an effect on the *tail* offset. */
        // 有需要的话，将 nextdiff 也加上到 zltail 上
        tail = zipEntry(p+reqlen);
        if (p[reqlen+tail.headersize+tail.len] != ZIP_END) {
            ZIPLIST_TAIL_OFFSET(zl) =
                intrev32ifbe(intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl))+nextdiff);
        }
    } else {
        /* This element will be the new tail. */
        // 更新 ziplist 的 zltail 属性，现在新添加节点为表尾节点
        ZIPLIST_TAIL_OFFSET(zl) = intrev32ifbe(p-zl);
    }

    /* When nextdiff != 0, the raw length of the next entry has changed, so
     * we need to cascade the update throughout the ziplist */
    if (nextdiff != 0) {
        offset = p-zl;
        // O(N^2)
        zl = __ziplistCascadeUpdate(zl,p+reqlen);
        p = zl+offset;
    }

    /* Write the entry */
    // 写入数据到节点

    // 编码上一节点的长度，并向后移动指针
    p += zipPrevEncodeLength(p,prevlen);
    // 编码本节点的长度和类型，并向后移动指针
    p += zipEncodeLength(p,encoding,slen);
    // 写入内容到节点
    if (ZIP_IS_STR(encoding)) {
        memcpy(p,s,slen);
    } else {
        zipSaveInteger(p,value,encoding);
    }
    // 更新节点数量
    ZIPLIST_INCR_LENGTH(zl,1);

    return zl;
}

/*
 * 将新元素插入为列表的表头节点或者表尾节点
 *
 * 复杂度：O(N^2)
 *
 * 返回值：添加操作完成后的 ziplist
 */
unsigned char *ziplistPush(unsigned char *zl, unsigned char *s, unsigned int slen, int where) {
    unsigned char *p;
    p = (where == ZIPLIST_HEAD) ? ZIPLIST_ENTRY_HEAD(zl) : ZIPLIST_ENTRY_END(zl);
    return __ziplistInsert(zl,p,s,slen);
}

/* Returns an offset to use for iterating with ziplistNext. When the given
 * index is negative, the list is traversed back to front. When the list
 * doesn't contain an element at the provided index, NULL is returned. */
/*
 * 返回指向当前迭代节点的指针，用于 ziplistNext 进行迭代
 * 当偏移值是负数时，表示迭代是从表尾到表头进行的。
 * 当元素被遍历完时，返回 NULL
 *
 * 复杂度：O(1)
 *
 * 返回值：指向节点的指针
 */
unsigned char *ziplistIndex(unsigned char *zl, int index) {

    unsigned char *p;

    zlentry entry;

    // 向前遍历
    if (index < 0) {
        index = (-index)-1;
        p = ZIPLIST_ENTRY_TAIL(zl);
        // 如果 ziplist 不为空。。。
        if (p[0] != ZIP_END) {
            // 那么根据 entry.prevrawlen ，进行向前迭代
            entry = zipEntry(p);
            while (entry.prevrawlen > 0 && index--) {
                // 后退地址
                p -= entry.prevrawlen;
                entry = zipEntry(p);
            }
        }
    // 向后遍历
    } else {
        p = ZIPLIST_ENTRY_HEAD(zl);
        // 根据 entry.prevrawlen 向后进行迭代
        while (p[0] != ZIP_END && index--) {
            p += zipRawEntryLength(p);
        }
    }

    return (p[0] == ZIP_END || index > 0) ? NULL : p;
}

/* Return pointer to next entry in ziplist.
 *
 * zl is the pointer to the ziplist
 * p is the pointer to the current element
 *
 * The element after 'p' is returned, otherwise NULL if we are at the end. */
/*
 * 返回指向 p 的下一个节点的指针，
 * 如果 p 已经到达表尾，那么返回 NULL 。
 *
 * 复杂度：O(1)
 */
unsigned char *ziplistNext(unsigned char *zl, unsigned char *p) {
    ((void) zl);

    /* "p" could be equal to ZIP_END, caused by ziplistDelete,
     * and we should return NULL. Otherwise, we should return NULL
     * when the *next* element is ZIP_END (there is no next entry). */
    if (p[0] == ZIP_END) {
        return NULL;
    }

    // 指向下一节点，O(1)
    p += zipRawEntryLength(p);
    if (p[0] == ZIP_END) {
        return NULL;
    }

    return p;
}

/* Return pointer to previous entry in ziplist. */
/*
 * 返回 p 的前一个节点
 *
 * 复杂度：O(1)
 */
unsigned char *ziplistPrev(unsigned char *zl, unsigned char *p) {
    zlentry entry;

    /* Iterating backwards from ZIP_END should return the tail. When "p" is
     * equal to the first element of the list, we're already at the head,
     * and should return NULL. */
    // 这是表尾
    if (p[0] == ZIP_END) {
        p = ZIPLIST_ENTRY_TAIL(zl);
        return (p[0] == ZIP_END) ? NULL : p;
    // 到达表头，停止
    } else if (p == ZIPLIST_ENTRY_HEAD(zl)) {
        return NULL;
    } else {
        entry = zipEntry(p);
        assert(entry.prevrawlen > 0);
        return p-entry.prevrawlen;
    }
}

/* Get entry pointer to by 'p' and store in either 'e' or 'v' depending
 * on the encoding of the entry. 'e' is always set to NULL to be able
 * to find out whether the string pointer or the integer value was set.
 * Return 0 if 'p' points to the end of the zipmap, 1 otherwise. */
/*
 * 获取 p 所指向的节点，并将相关属性保存至指针
 *
 * 如果节点保存的是字符串值，那么将 sstr 指针指向它，
 * slen 设在为字符串的长度。
 *
 * 如果节点保存的是数字值，那么用 sval 保存它。
 *
 * p 为表尾时返回 0 ，否则返回 1 。
 *
 * 复杂度：O(1)
 */
unsigned int ziplistGet(unsigned char *p, unsigned char **sstr, unsigned int *slen, long long *sval) {
    zlentry entry;
    // 表尾
    if (p == NULL || p[0] == ZIP_END) return 0;
    if (sstr) *sstr = NULL;

    // 获取节点
    entry = zipEntry(p);
    // 字符串
    if (ZIP_IS_STR(entry.encoding)) {
        if (sstr) {
            *slen = entry.len;
            *sstr = p+entry.headersize;
        }
    // 数字值
    } else {
        if (sval) {
            *sval = zipLoadInteger(p+entry.headersize,entry.encoding);
        }
    }
    return 1;
}

/* Insert an entry at "p". */
/*
 * 将节点插入到 p 之后
 *
 * 复杂度：O(N^2)
 *
 * 返回值：添加完成后的 ziplist
 */
unsigned char *ziplistInsert(unsigned char *zl, unsigned char *p, unsigned char *s, unsigned int slen) {
    return __ziplistInsert(zl,p,s,slen);
}

/* Delete a single entry from the ziplist, pointed to by *p.
 * Also update *p in place, to be able to iterate over the
 * ziplist, while deleting entries. */
/*
 * 删除 p 所指向的节点，
 * 并原地更新 p 指针，让它指向被删除节点的后一个节点，
 * 使得可以迭代地进行删除
 * 
 * 复杂度：O(N^2)
 * 
 * 返回值：删除完成后的 ziplist
 */
unsigned char *ziplistDelete(unsigned char *zl, unsigned char **p) {
    size_t offset = *p-zl;
    zl = __ziplistDelete(zl,*p,1);

    /* Store pointer to current element in p, because ziplistDelete will
     * do a realloc which might result in a different "zl"-pointer.
     * When the delete direction is back to front, we might delete the last
     * entry and end up with "p" pointing to ZIP_END, so check this. */
    *p = zl+offset;
    return zl;
}

/* Delete a range of entries from the ziplist. */
/*
 * 定位到索引 index ，并删除这之后的 num 个元素
 *
 * 复杂度：O(N^2)
 * 
 * 返回值：删除完成后的 ziplist
 */
unsigned char *ziplistDeleteRange(unsigned char *zl, unsigned int index, unsigned int num) {
    unsigned char *p = ziplistIndex(zl,index);
    return (p == NULL) ? zl : __ziplistDelete(zl,p,num);
}

/* Compare entry pointer to by 'p' with 'entry'. Return 1 if equal. */
/*
 * 将 p 所指向的节点的属性和 sstr 以及 slen 进行对比，
 * 如果相等则返回 1 。
 *
 * 复杂度：O(N)
 */
unsigned int ziplistCompare(unsigned char *p, unsigned char *sstr, unsigned int slen) {
    zlentry entry;
    unsigned char sencoding;
    long long zval, sval;

    // p 是表尾？
    if (p[0] == ZIP_END) return 0;

    // 获取节点属性
    entry = zipEntry(p);
    // 对比字符串
    if (ZIP_IS_STR(entry.encoding)) {
        /* Raw compare */
        if (entry.len == slen) {
            // O(N)
            return memcmp(p+entry.headersize,sstr,slen) == 0;
        } else {
            return 0;
        }
    // 对比整数
    } else {
        /* Try to compare encoded values. Don't compare encoding because
         * different implementations may encoded integers differently. */
        if (zipTryEncoding(sstr,slen,&sval,&sencoding)) {
          zval = zipLoadInteger(p+entry.headersize,entry.encoding);
          return zval == sval;
        }
    }

    return 0;
}

/* Find pointer to the entry equal to the specified entry. Skip 'skip' entries
 * between every comparison. Returns NULL when the field could not be found. */
/*
 * 根据给定的 vstr 和 vlen ，查找和属性和它们相等的节点
 * 在每次比对之间，跳过 skip 个节点。
 *
 * 复杂度：O(N)
 * 返回值：
 *  查找失败返回 NULL 。
 *  查找成功返回指向目标节点的指针
 */
unsigned char *ziplistFind(unsigned char *p, unsigned char *vstr, unsigned int vlen, unsigned int skip) {
    int skipcnt = 0;
    unsigned char vencoding = 0;
    long long vll = 0;

    // 遍历整个列表
    while (p[0] != ZIP_END) {
        unsigned int prevlensize, encoding, lensize, len;
        unsigned char *q;

        // 编码前一个节点的长度所需的空间
        ZIP_DECODE_PREVLENSIZE(p, prevlensize);
        // 当前节点的长度
        ZIP_DECODE_LENGTH(p + prevlensize, encoding, lensize, len);
        // 保存下一个节点的地址
        q = p + prevlensize + lensize;

        if (skipcnt == 0) {
            /* Compare current entry with specified entry */
            // 对比字符串
            if (ZIP_IS_STR(encoding)) {
                if (len == vlen && memcmp(q, vstr, vlen) == 0) {
                    return p;
                }
            // 对比整数
            } else {
                /* Find out if the searched field can be encoded. Note that
                 * we do it only the first time, once done vencoding is set
                 * to non-zero and vll is set to the integer value. */
                // 对传入值进行 decode
                if (vencoding == 0) {
                    if (!zipTryEncoding(vstr, vlen, &vll, &vencoding)) {
                        /* If the entry can't be encoded we set it to
                         * UCHAR_MAX so that we don't retry again the next
                         * time. */
                        vencoding = UCHAR_MAX;
                    }
                    /* Must be non-zero by now */
                    assert(vencoding);
                }

                /* Compare current entry with specified entry, do it only
                 * if vencoding != UCHAR_MAX because if there is no encoding
                 * possible for the field it can't be a valid integer. */
                if (vencoding != UCHAR_MAX) {
                    // 对比
                    long long ll = zipLoadInteger(q, encoding);
                    if (ll == vll) {
                        return p;
                    }
                }
            }

            /* Reset skip count */
            skipcnt = skip;
        } else {
            /* Skip entry */
            skipcnt--;
        }

        /* Move to next entry */
        p = q + len;
    }

    return NULL;
}

/* Return length of ziplist. */
/*
 * 返回 ziplist 的长度
 *
 * 复杂度：O(N)
 */
unsigned int ziplistLen(unsigned char *zl) {
    unsigned int len = 0;
    // 节点的数量 < UINT16_MAX
    if (intrev16ifbe(ZIPLIST_LENGTH(zl)) < UINT16_MAX) {
        // 长度保存在一个 uint16 整数中
        len = intrev16ifbe(ZIPLIST_LENGTH(zl));
    // 节点的数量 >= UINT16_MAX
    } else {
        // 遍历整个 ziplist ，计算长度
        unsigned char *p = zl+ZIPLIST_HEADER_SIZE;
        while (*p != ZIP_END) {
            p += zipRawEntryLength(p);
            len++;
        }

        /* Re-store length if small enough */
        if (len < UINT16_MAX) ZIPLIST_LENGTH(zl) = intrev16ifbe(len);
    }
    return len;
}

/* Return ziplist blob size in bytes. */
/*
 * 返回整个 ziplist 的空间大小
 * 
 * 复杂度：O(1)
 */
size_t ziplistBlobLen(unsigned char *zl) {
    return intrev32ifbe(ZIPLIST_BYTES(zl));
}

void ziplistRepr(unsigned char *zl) {
    unsigned char *p;
    int index = 0;
    zlentry entry;

    printf(
        "{total bytes %d} "
        "{length %u}\n"
        "{tail offset %u}\n",
        intrev32ifbe(ZIPLIST_BYTES(zl)),
        intrev16ifbe(ZIPLIST_LENGTH(zl)),
        intrev32ifbe(ZIPLIST_TAIL_OFFSET(zl)));
    p = ZIPLIST_ENTRY_HEAD(zl);
    while(*p != ZIP_END) {
        entry = zipEntry(p);
        printf(
            "{"
                "addr 0x%08lx, "
                "index %2d, "
                "offset %5ld, "
                "rl: %5u, "
                "hs %2u, "
                "pl: %5u, "
                "pls: %2u, "
                "payload %5u"
            "} ",
            (long unsigned)p,
            index,
            (unsigned long) (p-zl),
            entry.headersize+entry.len,
            entry.headersize,
            entry.prevrawlen,
            entry.prevrawlensize,
            entry.len);
        p += entry.headersize;
        if (ZIP_IS_STR(entry.encoding)) {
            if (entry.len > 40) {
                if (fwrite(p,40,1,stdout) == 0) perror("fwrite");
                printf("...");
            } else {
                if (entry.len &&
                    fwrite(p,entry.len,1,stdout) == 0) perror("fwrite");
            }
        } else {
            printf("%lld", (long long) zipLoadInteger(p,entry.encoding));
        }
        printf("\n");
        p += entry.len;
        index++;
    }
    printf("{end}\n\n");
}

#ifdef ZIPLIST_TEST_MAIN
#include <sys/time.h>
#include "adlist.h"
#include "sds.h"

#define debug(f, ...) { if (DEBUG) printf(f, __VA_ARGS__); }

unsigned char *createList() {
    unsigned char *zl = ziplistNew();
    zl = ziplistPush(zl, (unsigned char*)"foo", 3, ZIPLIST_TAIL);
    zl = ziplistPush(zl, (unsigned char*)"quux", 4, ZIPLIST_TAIL);
    zl = ziplistPush(zl, (unsigned char*)"hello", 5, ZIPLIST_HEAD);
    zl = ziplistPush(zl, (unsigned char*)"1024", 4, ZIPLIST_TAIL);
    return zl;
}

unsigned char *createIntList() {
    unsigned char *zl = ziplistNew();
    char buf[32];

    sprintf(buf, "100");
    zl = ziplistPush(zl, (unsigned char*)buf, strlen(buf), ZIPLIST_TAIL);
    sprintf(buf, "128000");
    zl = ziplistPush(zl, (unsigned char*)buf, strlen(buf), ZIPLIST_TAIL);
    sprintf(buf, "-100");
    zl = ziplistPush(zl, (unsigned char*)buf, strlen(buf), ZIPLIST_HEAD);
    sprintf(buf, "4294967296");
    zl = ziplistPush(zl, (unsigned char*)buf, strlen(buf), ZIPLIST_HEAD);
    sprintf(buf, "non integer");
    zl = ziplistPush(zl, (unsigned char*)buf, strlen(buf), ZIPLIST_TAIL);
    sprintf(buf, "much much longer non integer");
    zl = ziplistPush(zl, (unsigned char*)buf, strlen(buf), ZIPLIST_TAIL);
    return zl;
}

long long usec(void) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000000)+tv.tv_usec;
}

void stress(int pos, int num, int maxsize, int dnum) {
    int i,j,k;
    unsigned char *zl;
    char posstr[2][5] = { "HEAD", "TAIL" };
    long long start;
    for (i = 0; i < maxsize; i+=dnum) {
        zl = ziplistNew();
        for (j = 0; j < i; j++) {
            zl = ziplistPush(zl,(unsigned char*)"quux",4,ZIPLIST_TAIL);
        }

        /* Do num times a push+pop from pos */
        start = usec();
        for (k = 0; k < num; k++) {
            zl = ziplistPush(zl,(unsigned char*)"quux",4,pos);
            zl = ziplistDeleteRange(zl,0,1);
        }
        printf("List size: %8d, bytes: %8d, %dx push+pop (%s): %6lld usec\n",
            i,intrev32ifbe(ZIPLIST_BYTES(zl)),num,posstr[pos],usec()-start);
        zfree(zl);
    }
}

void pop(unsigned char *zl, int where) {
    unsigned char *p, *vstr;
    unsigned int vlen;
    long long vlong;

    p = ziplistIndex(zl,where == ZIPLIST_HEAD ? 0 : -1);
    if (ziplistGet(p,&vstr,&vlen,&vlong)) {
        if (where == ZIPLIST_HEAD)
            printf("Pop head: ");
        else
            printf("Pop tail: ");

        if (vstr)
            if (vlen && fwrite(vstr,vlen,1,stdout) == 0) perror("fwrite");
        else
            printf("%lld", vlong);

        printf("\n");
        ziplistDeleteRange(zl,-1,1);
    } else {
        printf("ERROR: Could not pop\n");
        exit(1);
    }
}

int randstring(char *target, unsigned int min, unsigned int max) {
    int p, len = min+rand()%(max-min+1);
    int minval, maxval;
    switch(rand() % 3) {
    case 0:
        minval = 0;
        maxval = 255;
    break;
    case 1:
        minval = 48;
        maxval = 122;
    break;
    case 2:
        minval = 48;
        maxval = 52;
    break;
    default:
        assert(NULL);
    }

    while(p < len)
        target[p++] = minval+rand()%(maxval-minval+1);
    return len;
}

void verify(unsigned char *zl, zlentry *e) {
    int i;
    int len = ziplistLen(zl);
    zlentry _e;

    for (i = 0; i < len; i++) {
        memset(&e[i], 0, sizeof(zlentry));
        e[i] = zipEntry(ziplistIndex(zl, i));

        memset(&_e, 0, sizeof(zlentry));
        _e = zipEntry(ziplistIndex(zl, -len+i));

        assert(memcmp(&e[i], &_e, sizeof(zlentry)) == 0);
    }
}

int main(int argc, char **argv) {
    unsigned char *zl, *p;
    unsigned char *entry;
    unsigned int elen;
    long long value;

    /* If an argument is given, use it as the random seed. */
    if (argc == 2)
        srand(atoi(argv[1]));

    zl = createIntList();
    ziplistRepr(zl);

    zl = createList();
    ziplistRepr(zl);

    pop(zl,ZIPLIST_TAIL);
    ziplistRepr(zl);

    pop(zl,ZIPLIST_HEAD);
    ziplistRepr(zl);

    pop(zl,ZIPLIST_TAIL);
    ziplistRepr(zl);

    pop(zl,ZIPLIST_TAIL);
    ziplistRepr(zl);

    printf("Get element at index 3:\n");
    {
        zl = createList();
        p = ziplistIndex(zl, 3);
        if (!ziplistGet(p, &entry, &elen, &value)) {
            printf("ERROR: Could not access index 3\n");
            return 1;
        }
        if (entry) {
            if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            printf("\n");
        } else {
            printf("%lld\n", value);
        }
        printf("\n");
    }

    printf("Get element at index 4 (out of range):\n");
    {
        zl = createList();
        p = ziplistIndex(zl, 4);
        if (p == NULL) {
            printf("No entry\n");
        } else {
            printf("ERROR: Out of range index should return NULL, returned offset: %ld\n", p-zl);
            return 1;
        }
        printf("\n");
    }

    printf("Get element at index -1 (last element):\n");
    {
        zl = createList();
        p = ziplistIndex(zl, -1);
        if (!ziplistGet(p, &entry, &elen, &value)) {
            printf("ERROR: Could not access index -1\n");
            return 1;
        }
        if (entry) {
            if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            printf("\n");
        } else {
            printf("%lld\n", value);
        }
        printf("\n");
    }

    printf("Get element at index -4 (first element):\n");
    {
        zl = createList();
        p = ziplistIndex(zl, -4);
        if (!ziplistGet(p, &entry, &elen, &value)) {
            printf("ERROR: Could not access index -4\n");
            return 1;
        }
        if (entry) {
            if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            printf("\n");
        } else {
            printf("%lld\n", value);
        }
        printf("\n");
    }

    printf("Get element at index -5 (reverse out of range):\n");
    {
        zl = createList();
        p = ziplistIndex(zl, -5);
        if (p == NULL) {
            printf("No entry\n");
        } else {
            printf("ERROR: Out of range index should return NULL, returned offset: %ld\n", p-zl);
            return 1;
        }
        printf("\n");
    }

    printf("Iterate list from 0 to end:\n");
    {
        zl = createList();
        p = ziplistIndex(zl, 0);
        while (ziplistGet(p, &entry, &elen, &value)) {
            printf("Entry: ");
            if (entry) {
                if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            } else {
                printf("%lld", value);
            }
            p = ziplistNext(zl,p);
            printf("\n");
        }
        printf("\n");
    }

    printf("Iterate list from 1 to end:\n");
    {
        zl = createList();
        p = ziplistIndex(zl, 1);
        while (ziplistGet(p, &entry, &elen, &value)) {
            printf("Entry: ");
            if (entry) {
                if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            } else {
                printf("%lld", value);
            }
            p = ziplistNext(zl,p);
            printf("\n");
        }
        printf("\n");
    }

    printf("Iterate list from 2 to end:\n");
    {
        zl = createList();
        p = ziplistIndex(zl, 2);
        while (ziplistGet(p, &entry, &elen, &value)) {
            printf("Entry: ");
            if (entry) {
                if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            } else {
                printf("%lld", value);
            }
            p = ziplistNext(zl,p);
            printf("\n");
        }
        printf("\n");
    }

    printf("Iterate starting out of range:\n");
    {
        zl = createList();
        p = ziplistIndex(zl, 4);
        if (!ziplistGet(p, &entry, &elen, &value)) {
            printf("No entry\n");
        } else {
            printf("ERROR\n");
        }
        printf("\n");
    }

    printf("Iterate from back to front:\n");
    {
        zl = createList();
        p = ziplistIndex(zl, -1);
        while (ziplistGet(p, &entry, &elen, &value)) {
            printf("Entry: ");
            if (entry) {
                if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            } else {
                printf("%lld", value);
            }
            p = ziplistPrev(zl,p);
            printf("\n");
        }
        printf("\n");
    }

    printf("Iterate from back to front, deleting all items:\n");
    {
        zl = createList();
        p = ziplistIndex(zl, -1);
        while (ziplistGet(p, &entry, &elen, &value)) {
            printf("Entry: ");
            if (entry) {
                if (elen && fwrite(entry,elen,1,stdout) == 0) perror("fwrite");
            } else {
                printf("%lld", value);
            }
            zl = ziplistDelete(zl,&p);
            p = ziplistPrev(zl,p);
            printf("\n");
        }
        printf("\n");
    }

    printf("Delete inclusive range 0,0:\n");
    {
        zl = createList();
        zl = ziplistDeleteRange(zl, 0, 1);
        ziplistRepr(zl);
    }

    printf("Delete inclusive range 0,1:\n");
    {
        zl = createList();
        zl = ziplistDeleteRange(zl, 0, 2);
        ziplistRepr(zl);
    }

    printf("Delete inclusive range 1,2:\n");
    {
        zl = createList();
        zl = ziplistDeleteRange(zl, 1, 2);
        ziplistRepr(zl);
    }

    printf("Delete with start index out of range:\n");
    {
        zl = createList();
        zl = ziplistDeleteRange(zl, 5, 1);
        ziplistRepr(zl);
    }

    printf("Delete with num overflow:\n");
    {
        zl = createList();
        zl = ziplistDeleteRange(zl, 1, 5);
        ziplistRepr(zl);
    }

    printf("Delete foo while iterating:\n");
    {
        zl = createList();
        p = ziplistIndex(zl,0);
        while (ziplistGet(p,&entry,&elen,&value)) {
            if (entry && strncmp("foo",(char*)entry,elen) == 0) {
                printf("Delete foo\n");
                zl = ziplistDelete(zl,&p);
            } else {
                printf("Entry: ");
                if (entry) {
                    if (elen && fwrite(entry,elen,1,stdout) == 0)
                        perror("fwrite");
                } else {
                    printf("%lld",value);
                }
                p = ziplistNext(zl,p);
                printf("\n");
            }
        }
        printf("\n");
        ziplistRepr(zl);
    }

    printf("Regression test for >255 byte strings:\n");
    {
        char v1[257],v2[257];
        memset(v1,'x',256);
        memset(v2,'y',256);
        zl = ziplistNew();
        zl = ziplistPush(zl,(unsigned char*)v1,strlen(v1),ZIPLIST_TAIL);
        zl = ziplistPush(zl,(unsigned char*)v2,strlen(v2),ZIPLIST_TAIL);

        /* Pop values again and compare their value. */
        p = ziplistIndex(zl,0);
        assert(ziplistGet(p,&entry,&elen,&value));
        assert(strncmp(v1,(char*)entry,elen) == 0);
        p = ziplistIndex(zl,1);
        assert(ziplistGet(p,&entry,&elen,&value));
        assert(strncmp(v2,(char*)entry,elen) == 0);
        printf("SUCCESS\n\n");
    }

    printf("Regression test deleting next to last entries:\n");
    {
        char v[3][257];
        zlentry e[3];
        int i;

        for (i = 0; i < (sizeof(v)/sizeof(v[0])); i++) {
            memset(v[i], 'a' + i, sizeof(v[0]));
        }

        v[0][256] = '\0';
        v[1][  1] = '\0';
        v[2][256] = '\0';

        zl = ziplistNew();
        for (i = 0; i < (sizeof(v)/sizeof(v[0])); i++) {
            zl = ziplistPush(zl, (unsigned char *) v[i], strlen(v[i]), ZIPLIST_TAIL);
        }

        verify(zl, e);

        assert(e[0].prevrawlensize == 1);
        assert(e[1].prevrawlensize == 5);
        assert(e[2].prevrawlensize == 1);

        /* Deleting entry 1 will increase `prevrawlensize` for entry 2 */
        unsigned char *p = e[1].p;
        zl = ziplistDelete(zl, &p);

        verify(zl, e);

        assert(e[0].prevrawlensize == 1);
        assert(e[1].prevrawlensize == 5);

        printf("SUCCESS\n\n");
    }

    printf("Create long list and check indices:\n");
    {
        zl = ziplistNew();
        char buf[32];
        int i,len;
        for (i = 0; i < 1000; i++) {
            len = sprintf(buf,"%d",i);
            zl = ziplistPush(zl,(unsigned char*)buf,len,ZIPLIST_TAIL);
        }
        for (i = 0; i < 1000; i++) {
            p = ziplistIndex(zl,i);
            assert(ziplistGet(p,NULL,NULL,&value));
            assert(i == value);

            p = ziplistIndex(zl,-i-1);
            assert(ziplistGet(p,NULL,NULL,&value));
            assert(999-i == value);
        }
        printf("SUCCESS\n\n");
    }

    printf("Compare strings with ziplist entries:\n");
    {
        zl = createList();
        p = ziplistIndex(zl,0);
        if (!ziplistCompare(p,(unsigned char*)"hello",5)) {
            printf("ERROR: not \"hello\"\n");
            return 1;
        }
        if (ziplistCompare(p,(unsigned char*)"hella",5)) {
            printf("ERROR: \"hella\"\n");
            return 1;
        }

        p = ziplistIndex(zl,3);
        if (!ziplistCompare(p,(unsigned char*)"1024",4)) {
            printf("ERROR: not \"1024\"\n");
            return 1;
        }
        if (ziplistCompare(p,(unsigned char*)"1025",4)) {
            printf("ERROR: \"1025\"\n");
            return 1;
        }
        printf("SUCCESS\n\n");
    }

    printf("Stress with random payloads of different encoding:\n");
    {
        int i,j,len,where;
        unsigned char *p;
        char buf[1024];
        int buflen;
        list *ref;
        listNode *refnode;

        /* Hold temp vars from ziplist */
        unsigned char *sstr;
        unsigned int slen;
        long long sval;

        for (i = 0; i < 20000; i++) {
            zl = ziplistNew();
            ref = listCreate();
            listSetFreeMethod(ref,sdsfree);
            len = rand() % 256;

            /* Create lists */
            for (j = 0; j < len; j++) {
                where = (rand() & 1) ? ZIPLIST_HEAD : ZIPLIST_TAIL;
                if (rand() % 2) {
                    buflen = randstring(buf,1,sizeof(buf)-1);
                } else {
                    switch(rand() % 3) {
                    case 0:
                        buflen = sprintf(buf,"%lld",(0LL + rand()) >> 20);
                        break;
                    case 1:
                        buflen = sprintf(buf,"%lld",(0LL + rand()));
                        break;
                    case 2:
                        buflen = sprintf(buf,"%lld",(0LL + rand()) << 20);
                        break;
                    default:
                        assert(NULL);
                    }
                }

                /* Add to ziplist */
                zl = ziplistPush(zl, (unsigned char*)buf, buflen, where);

                /* Add to reference list */
                if (where == ZIPLIST_HEAD) {
                    listAddNodeHead(ref,sdsnewlen(buf, buflen));
                } else if (where == ZIPLIST_TAIL) {
                    listAddNodeTail(ref,sdsnewlen(buf, buflen));
                } else {
                    assert(NULL);
                }
            }

            assert(listLength(ref) == ziplistLen(zl));
            for (j = 0; j < len; j++) {
                /* Naive way to get elements, but similar to the stresser
                 * executed from the Tcl test suite. */
                p = ziplistIndex(zl,j);
                refnode = listIndex(ref,j);

                assert(ziplistGet(p,&sstr,&slen,&sval));
                if (sstr == NULL) {
                    buflen = sprintf(buf,"%lld",sval);
                } else {
                    buflen = slen;
                    memcpy(buf,sstr,buflen);
                    buf[buflen] = '\0';
                }
                assert(memcmp(buf,listNodeValue(refnode),buflen) == 0);
            }
            zfree(zl);
            listRelease(ref);
        }
        printf("SUCCESS\n\n");
    }

    printf("Stress with variable ziplist size:\n");
    {
        stress(ZIPLIST_HEAD,100000,16384,256);
        stress(ZIPLIST_TAIL,100000,16384,256);
    }

    return 0;
}

#endif
