#ifndef _INTSET_H_
#define _INTSET_H_ 

#include <stdint.h>
#include <stdio.h>

//整数集合 
//用于有序 无重复保存多个整数值
//只保存整数元素 元素数量不多
//会自动根据元素的值选择合适的整数类型来保存
//如果一个新的值无法用目前类型保存 intset会自动升级成更大的类型来保存
//int8_t -> int16_t -> int32_t ->int64_t
typedef struct intset {
	//保存元素使用类型的长度
	//定义了三种不同的长度类型 int16_t int32_t int64_t
	uint32_t encoding;

	//元素的个数
	uint32_t length;

	//保存元素的数组
	//没有重复元素 元素在数组中从小到大排列
	int8_t contents[];
} intset;

intset *intsetNew(void);		//创建intset O(1)
intset *intsetAdd(intset *is, int64_t value, uint8_t *success);		//添加value到intset
intset *intsetRemove(intset *is, int64_t value, int *success);		//删除元素 O(N)
uint8_t intsetFind(intset *is, int64_t value);		//查找intset中的value
int64_t intsetRandom(intset *is);	//随机返回一个intset中的元素
uint8_t intsetGet(intset *is, uint32_t pos, int64_t *value);	//获取intset pos位置的值 保存在 *value中
uint32_t intsetLen(intset *is);		//获取intset长度
size_t intsetBlobLen(intset *is);	//获取intset总占字节大小
#endif
