#include "intset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zmalloc.h"
//各类型整数在大小端系统下需要切换 
//#include "endianconv.h"            


#define INTSET_ENC_INT16 (sizeof(int16_t))
#define INTSET_ENC_INT32 (sizeof(int32_t))
#define INTSET_ENC_INT64 (sizeof(int64_t))

//返回 编码 v所需要的intset encoding长度
static uint8_t _intsetValueEncoding(int64_t v)
{
	if (v < INT32_MIN || v > INT32_MAX){
		return INTSET_ENC_INT64;
	} else if (v < INT16_MIN || v > INT16_MAX){
		return INTSET_ENC_INT32;
	}else {
		return INTSET_ENC_INT16;
	}
}

// 根据给定的编码方式 返回给定位置上的值
// enc 编码方式
// pos 位置
// is 给定的intset
static int64_t _intsetGetEncoding(intset *is, int pos, uint8_t enc)
{
	int64_t v64;
	int32_t v32;
	int16_t v16;

	if (enc == INTSET_ENC_INT64){
		memcpy(&v64, ((int64_t*)is->contents) +pos, sizeof(v64));
		return v64;
	}else if (enc == INTSET_ENC_INT32){
		memcpy(&v32, ((int32_t*)is->contents) +pos, sizeof(v32));
		return v32;
	}else {
		memcpy(&v16, ((int16_t*)is->contents) +pos, sizeof(v16));
		return v16;
	}
}

//返回intset上给定的pos值
static int64_t _intsetGet(intset *is, int pos)
{
	return _intsetGetEncoding(is, pos, is->encoding);
}

//将intset上给定的 pos值设置为value
static void _intsetSet(intset *is, int pos, int64_t value)
{
	uint32_t encoding = is->encoding;

	if (encoding == INTSET_ENC_INT64){
		((int64_t*)is->contents)[pos] = value;
	}else if (encoding == INTSET_ENC_INT32){
		((int32_t*)is->contents)[pos] = value;
	}else {
		((int16_t*)is->contents)[pos] = value;
	}
}

//创建一个新的intset
intset *intsetNew(void)
{
	intset *is = zmalloc(sizeof(struct intset));
	is->encoding = INTSET_ENC_INT16;
	is->length = 0;

	return is;
}

//调整intset大小
//O(N)
static intset *intsetResize(intset *is, uint32_t len)
{
	uint32_t size = len * is->encoding;
	is = zrealloc(is, sizeof(intset) +size);
	return is;
}

//查找value在intset中的索引
//查找成功 索引保存在pos 返回1
//查找失败返回0 并将value可以插入的索引保存到pos
static uint8_t intsetSearch(intset *is, int64_t value, uint32_t *pos)
{
	int min = 0;
	int max = is->length -1;
	int mid = -1;
	int64_t cur = -1;

	if (is->length == 0){
		if (pos){
			*pos = 0;
		}
		return 0;
	} else {
		//超出最大值 查找失败 返回0 如果value要插入 pos保存插入索引
		//小于最小值 查找失败 返回0 如果value要插入 pos保存插入索引
		if (value > _intsetGet(is, is->length -1)){
			if (pos) {
				*pos = is->length;
			}
			return 0;
		}else if (value < _intsetGet(is, 0)){
			if (pos){
				*pos = 0;
			}
			return 0;
		}
	}

	//二分查找
	while (max >= min){
		mid = (min + max) / 2;
		cur = _intsetGet(is, mid);
		if (value >cur) {
			min = mid +1;
		}else if (value <cur){
			max = mid -1;
		}else {
			break;
		}
	}

	//找到了的话
	if (value == cur){
		if (pos){
			*pos = mid;
		}
		return 1;
	}else {
		if (pos){
			*pos = min;
		}
		//没找到
		return 0;
	}
}
//将value插入到intset中
//根据value的大小 确定是否需要升级intset的编码方式
//如果需要 升级编码方式并将原先的数据都迁移到新的intset中 插入value到合适位置 返回新的intset
//如果不需要直接插入 返回原来的intset
static intset *intsetUpgradeAndAdd(intset *is, int64_t value)
{
	//intset中的encoding
	uint8_t curenc = is->encoding;
	//value需要的encoding
	uint64_t newenc = _intsetValueEncoding(value);

	int length = is->length;

	//value要插入的位置的偏移量
	//value < 0 直接从前面开始插入
	//否则从后面插入不需要偏移
	int prepend = value< 0 ? 1 : 0;

	//新的encoding 并以新的容量进行扩容
	//需要1个元素位置
	is->encoding = newenc;
	is = intsetResize(is, is->length +1);


	//		从最大的元素也就是最后一个元素开始迁移到新的编码方式的内存中
	//
	//		没有更改编码方式之前 INTSET_ENC_INT16
	//		bit		0	15	31	47
	//		value	| 1 | 2 | 3 |
	//		要插入的value 65535 超出了INTSET_ENC_INT64
	//		更改了编码方式为 INTSET_ENC_INT32
	//		根据encoding重新分配了contents的内存排列
	//		bit		0	15	31	47	63	95	127
	//		value	| 1 | 2 | 3 | ? | ? | ? |
	//		现在分配到contents总共可以容纳4个INTSET_ENC_INT32值 所以需要将原来的元素重新分配
	//		从后向前 首先是3
	//		bit		0	15	31	47	63	95	127	
	//		value	| 1	| 2	|		|	3	|	?	|
	//		然后是2
	//		bit		0	15	31	47	63		95		127	
	//		value	| 1	| 	|	2	|	3	|	?	|
	//		最后是1
	//		bit		0	15	31	47	63		95		127	
	//		value	|   1	|	2	|	3	|	?	|
	//		最后将需要的value添加到预留出的位置
	//		bit		0	15	31	47	63		95		127	
	//		value	|   1	|	2	|	3	| 65535 |
	//		Add完成
	//		
	//		升级时从 较短整数到较长整数后 可以完全无损 无溢出 地迁移数据不会变更或丢失元素的值
	//		编码方式由元素中最大编码方式的元素决定 即使只有一个 也必须全部升级并迁移
	//		目前没有降级操作 元素个数也有限制 redis.h/#define REDIS_SET_MAX_INTSET_EVTRIES 512
	while (length--){
		_intsetSet(is, length+prepend, _intsetGetEncoding(is, length, curenc));
	}

	//以peropend来决定新的value放在intset的开头或者最末位置
	if (prepend){
		_intsetSet(is, 0, value);
	}else {
		_intsetSet(is, is->length, value);
	}

	is->length = is->length+1;
	return is;
}

//移动
//from表示移动开始的索引
//to表示要移动到的位置的索引
//移动数量为从from到intset结尾的所有元素
//
//假设有 intset is length=4
//	 0	 1	 2	 3			//因为是数组 索引从0开始
//	| a | b | c | d |
//	那么在执行intsetMoveTail(&is, 2, 1);之后
//	 0	 1	 2	 3
//	| a | c | d | d |
//	将2开始的到结尾的两个元素 c d 移动到1索引即b的地方 所以b被c覆盖 c被d覆盖 
static void intsetMoveTail(intset *is, uint32_t from, uint32_t to)
{
	void *src, *dst;

	//需要移动的元素个数
	uint32_t bytes = is->length - from;
	uint32_t encoding = is->encoding;

	//根据encoding 来确定源地址src和目的地址dst
	//并计算出移动的字节数
	if (encoding == INTSET_ENC_INT64){
		src = (int64_t*)is->contents + from;
		dst = (int64_t*)is->contents + to;
		bytes *= sizeof(int64_t);
	} else if (encoding == INTSET_ENC_INT32){
		src = (int32_t*)is->contents + from;
		dst = (int32_t*)is->contents + to;
		bytes *= sizeof(int32_t);
	} else {
		src = (int16_t*)is->contents + from;
		dst = (int16_t*)is->contents + to;
		bytes *= sizeof(int16_t);
	}
	
	//移动
	memmove(dst, src, bytes);
}

//将value添加进is中
//如果元素已经存在	添加失败 *success = 0
//如果元素不存在	添加成功 *success = 1
//O(N)
intset *intsetAdd(intset *is, int64_t value, uint8_t *success)
{
	uint8_t valenc = _intsetValueEncoding(value);
	uint32_t pos;
	if (success){
		*success = 1;
	}
	
	//需要升级编码方式
	if (valenc > is->encoding){
		return intsetUpgradeAndAdd(is, value);
	}else {
		//查找value是否存在 如存在 保存在pos中
		if (intsetSearch(is, value, &pos)){
			if (success){
				*success = 0;
			}
			return is;
		}
		//扩大intset
		is = intsetResize(is, is->length +1);
		if (pos < is->length){
			intsetMoveTail(is, pos, pos+1);
		}
	}

	//添加value并更新intset属性
	_intsetSet(is, pos, value);
	is->length = is->length + 1;
	return is;
}

//在intset中移除value
//如果移除成功 *success = 1
//value不存在 移除失败 *success = 0
//O(N)
intset *intsetRemove(intset *is, int64_t value, int *success)
{
	uint8_t valenc = _intsetValueEncoding(value);
	uint32_t pos;
	if (success){
		*success = 0;
	}
	if (valenc <= is->encoding && intsetSearch(is, value, &pos)){
		uint32_t len = is->length;
		if (success) {
			*success = 1;
		}

		if (pos < (len-1)){
			intsetMoveTail(is, pos+1, pos);
		}

		is = intsetResize(is, len-1);
		is->length -= 1;
	}			
	return is;
}
//查找value 不需要保存pos
uint8_t intsetFind(intset *is, int64_t value)
{
	uint8_t valenc = _intsetValueEncoding(value);
	return valenc <= is->encoding && intsetSearch(is, value, NULL);
}

//随机返回一个intset的数组元素
int64_t intsetRandom(intset *is)
{
	return _intsetGet(is, (rand() % is->length));
}

//获取is中pos地址值
//将 is中 pos位置的值保存到 value地址 返回1
//如果pos > length 返回0
uint8_t intsetGet(intset *is, uint32_t pos, int64_t *value)
{
	if (pos < is->length){
		*value = _intsetGet(is, pos);
		return 1;
	}
	return 0;
}

//返回is->length
uint32_t intsetLen(intset *is)
{
	return is->length;
}

//以字节形式返回 intset占空间大小
size_t intsetBlobLen(intset *is)
{
	//				头		数组元素	元素大小
	return sizeof(intset) + is->length * is->encoding;
}


