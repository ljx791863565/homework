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
	//如果key冲突 next用于指向相同key的后继节点
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
	dictEntry **table;
	//指针数组的大小
	unsigned long size;
	//指针数组长度掩码，用于计算index值
	unsigned long sizemask;
	//hashTable被使用的节点大小
	unsigned long used;
} dictht;

//字典 
typedef struct dict {
	//特定类型的处理函数
	dictType *type;
	//类型处理函数的私有数据
	void *privdata;
	//hashTable 2个	ht[0]为主要使用表 ht[1]主要作用在rehash时
	dictht ht[2];	
	//记录 rehash 进度标志， 值为-1表示rehash未进行
	int rehashidx;
	//当前正在运行的安全迭代器数量
	int iterators;
} dict;

//字典迭代器
//如果safe属性值为1，表示此迭代器是一个安全迭代器
//如果safe属性值为0，表示此迭代器不是一个安全迭代器
//
//安全迭代器迭代一个dist时，此字典可以调用 dictAdd dictFind和其他函数
//不安全迭代器只可以对字典调用 dictNext函数

typedef struct dictIterator {

	dict *d;              // 正在迭代的字典

	int table,            // 正在迭代的哈希表号，0/1
		index,            // 正在迭代的哈希表数组索引
		safe;             // 安全标志 0：不安全 1：安全

	dictEntry *entry,     // 当前哈希表节点
			  *nextEntry; // 当前哈希表节点的后继节点
} dictIterator;


//hashTable起始大小
#define DICT_HT_INITIAL_SIZE	4


#define dictFreeVal(d, entry) \
	if ((d)->type->valDestructor) \
		((d)->type->valDestructor((d)->privdata, (entry)->v.val))

#define dictSetVal(d,entry, _val_) do{ \
	if ((d)->type->valDup) \
		entry->v.val = (d)->type->valDup((d)->privdata, _val_); \
	else \
		entry->v.val = (_val_); \
} while (0)

#define dictSetSignedIntegetVal(entry, _val_) do{ \
	entry->v.s64 = _val_; \
} while (0)

#define dictSetUnsignedIntegetVal(entry, _val_) do{ \
	entry->v.u64 = _val_; \
} while (0)

#define dictFreeKey(d, entry) \
	if ((d)->type->keyDestructor) \
		(d)->type->keyDestructor((d)->privdata,(entry)->key)

#define dictSetKey(d, entry, _key_) do { \
	if ((d)->type->keyDup) \
		entry->key = (d)->type->keyDup((d)->privdata, _key_); \
	else \
		entry->key = _key_; \
} while (0)

//比较两个key 如果有对应的type函数 使用次函数对比 如果没有 直接对比
#define dictCompareKeys(d, key1, key2) \
	(((d)->type->keyCompare) ? \
	  (d)->type->keyCompare((d)->privdata, key1, key2) : \
	 (key1) == (key2))

//根据key调用适合的hashFunction计算index
#define dictHashKey(d, key) (d)->type->hashFunction(key)
#define dictGetKey(he) ((he)->key)
#define dictGetVal(he) ((he)->v.val)
#define dictGetSignedIntegerVal(he) ((he)->v.s64)
#define dictGetUnsignedIntegerVal(he) ((he)->v.u64)
#define dictSolts(d) ((d)->ht[0].size+(d)->ht[1].size)
#define dictSize(d) ((d)->ht[0].used+(d)->ht[1].used)
//确定dict是否在rehash中 返回0表示不在 返回1表示rehash标识已打开 dict正在rehash 
//这个标识只能确定正在rehash 并不知道dict是以哪种行为在rehash
#define dictIsRehashing(ht) ((ht)->rehashidx != -1)

dict *dictCreate(dictType *type, void *privDataPtr);		//创建一个新的字典 O(1)
int dictAdd(dict *d, void *key, void *val);					//添加新的 key:value 到字典 O(1)
dictEntry *dictAddRaw(dict *d, void *key);					//dictAdd函数的底层实现
int dictReplace(dict *d, void *key, void *val);				//添加或更新给定的key:value对 O(1)
dictEntry *dictReplaceRaw(dict *d, void *key);				//dictReplace函数底层实现
int dictDelete(dict *d, const void *key);					//根据给定key删除键值对 O(1)
int dictDeleteNoFree(dict *d, const void *key);				//删除哈希表中的key,但不释放保存这个key的dictEntry O(1)
void dictRelease(dict *d);									//清空并释放dict O(N)
void dictEmpty(dict *d);									//清空并重置dict 但不释放 O(N)
dictEntry *dictFind(dict *d, const void *key);				//根据给定的key查找dist中对应的dictEntry O(1)
void *dictFetchValue(dict *d, const void *key);				//根据给定的key查找dist中对应的value O(1)
int dictResize(dict *d);									//缩小字典容量 O(N)
int dictExpand(dict *d, unsigned long size);				//扩大字典容量 O(N)

dictIterator *dictGetIterator(dict *d);						//创建一个不安全迭代器 O(1)
dictIterator *dictGetSafeIterator(dict *d);					//创建一个安全迭代器 O(!)
dictEntry *dictNext(dictIterator *iter);					//返回迭代器当前节点 O(1)
void dictReleaseIterator(dictIterator *iter);				//释放迭代器 O(1)
dictEntry *dictGetRandomKey(dict *d);						//从dict随机返回一个dictEntry

void dictPrintStats(dict *d);
unsigned int dictGetHashFunction(const void *key, int len);
unsigned int dictGetCaseHashFunction(const unsigned char *buf, int len);

void dictEnalbeResize(void);								//打开rehash标识 O(1)
void dictDisableResize(void);								//关闭rehash标识 O(1)
int dictRehash(dict *d, int n);								//对dist进行给定步数的rehash O(N)
int dictRehashMilliseconds(dict *d, int ms);				//对dict进行给定时间的rehash O(N)
void dictSetHashFunctionSeed(unsigned int initval);
unsigned int dictGetHashFunctionSeed(void);

extern dictType dictTypeHeapStringCopyKey;
extern dictType dictTypeHeapString;
extern dictType dictTypeHeapStringCopyKeyValue;


#endif	// _DICT_H_
