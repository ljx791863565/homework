//c99引入，定义了几种扩展整数类型和宏
#include <stdint.h>

#ifndef _DICT_H_
#define _DICT_H_ 

//操作返回状态
#define DICT_OK 0
#define DICT_ERR 1

//哈希表节点
typedef struct dictEntry {
	//键
	void *key;
	//值
	union {
		void *val;
		uint64_t u64;
		int64_t s64;
	}v;
	struct dictEntry *next;
} dictEntry;

//特定类型的一簇处理函数
typedef struct dictType {
	//hash函数，根据key计算index 不同的dist可以拥有不同的hashFunction
	unsigned int (*hashFunction)(const void *key);
	//key复制函数
	void *(*keyDup)(void *privdata, const void *key);
	//value复制函数
	void *(*valDup)(void *privdata, const void *obj);
	//两个key对比函数
	int (*keyCompare)(void *privdata, const void *key1, const void *key2);
	//key析构函数
	void (*keyDestructor)(void *privdata, void *key);
	//value析构函数
	void (*valDestructor)(void *privdata, void *obj);
} dictType;

//hashTable
typedef struct dictht {
	//hashTable节点的指针数组， 即桶 bucket
	dictEntry **tabnle;
	//指针数组的大小
	unsigned long size;
	//指针数组长度掩码，用于计算index值
	unsigned long sizemask;
	//hashTable被使用的节点大小
	unsigned long used;
} dictht;

//字典 
typedef struct dist {
	dictType *type;
	void *privdata;
} dist;
#endif
