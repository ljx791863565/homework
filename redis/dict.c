#include "dict.h"
#include "zmalloc.h"
/*对不同系统的支持 ,暂时感觉不需要*/	
//#include "fmacros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*主要目的为让函数能够接收可变参数*/
#include <stdarg.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>


static int dict_can_resize = 1;
/*
 * ratio = used / size	哈希表的覆盖比例
 * 当其比例大于这个数的时候，会执行两个ht的强制rehash
 */
static unsigned int dict_force_resize_ratio = 5;

/* ------------------------ private prototypes --------------------------- */
static int _dictExpandIfNeeded(dict *ht);
static unsigned long _dictNextPower(unsigned long size);
static int _dictKeyIndex(dict *ht, const void *key);
static int _dictInit(dict *ht, dictType *type, void *privDataPtr);

/* ------------------------ 两个hashFunction ------------------------------ */
//Tohmas Wang's 32 bit Mix Function
unsigned int dictIntHashFunction(unsigned int key)
{
	key += ~(key << 15);
	key ^= (key >> 10);
	key += (key << 3);
	key ^= (key >> 6);
	key += ~(key << 11);
	key ^= (key >> 16);

	return key;
}

//Identity hash function for integer keys
unsigned int dictIdetityHashFunction(unsigned int key)
{
	return key;
}

static uint32_t dict_hash_function_seed = 5381;

void dictSetHashFunctionSeed(uint32_t seed)
{
	dict_hash_function_seed = seed;
}

uint32_t dictGetHashFunctionSeed(void)
{
	return dict_hash_function_seed;
}

//Austin Appleby MurmurHash2
unsigned int dictGenHashFunction(const void *key, int len)
{
	uint32_t seed = dict_hash_function_seed;
	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	uint32_t h = seed ^ len;

	const unsigned char *data = (const unsigned char *)key;

	while (len >= 4){
		uint32_t k = *(uint32_t*)data;
		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len)
	{
		case 3:
			h ^= data[2] << 16;
		case 2: 
			h ^= data[1] << 8;
		case 1:
			h ^= data[0];
			h *= m;
	}

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return (unsigned int)h;
}

//不分大小写
//DJB Hash
unsigned int dictGenCaseHashFunction(const unsigned char *buf, int len)
{
	unsigned int hash = (unsigned int)dict_hash_function_seed;

	while (len--){
		hash = ((hash << 5) + hash) + (tolower(*buf++));	//hash *33 + c 
	}
	return hash;
}

/* -------------------------- API implementation ------------------------------- */

//重置hashTbale的各项属性
//O(1)
static void _dictReset(dictht *ht)
{
	ht->table = NULL;
	ht->size = 0;
	ht->sizemask = 0;
	ht->used = 0;
}

//创建一个新的dict
//O(1)
dict *dictCreate(dictType *type, void *privDataPtr)
{
	dict *d = zmalloc(sizeof(struct dict));
	if (d == NULL){
		return NULL;
	}

	//初始化dict
	_dictInit(d, type, privDataPtr);

	return d;
}

//初始化dict
//O(1)
int _dictInit(dict *d, dictType *type, void *privDataPtr)
{
	//初始化ht
	_dictReset(&d->ht[0]);
	_dictReset(&d->ht[1]);

	//初始化dict属性
	d->type = type;
	d->privdata = privDataPtr;
	d->rehashidx = -1;
	d->iterators = 0;

	return DICT_OK;
}

//对dict进行紧缩，使得 used / size比率接近 <=1
//O(N)
int dictResize(dict *d)
{
	int minimal;

	//不能再dict_can_resize = 0 或者 dict正在rehash时调用
	if (!dict_can_resize || dictIsRehashing(d))
		return DICT_ERR;

	//按照目前ht[0]所使用的大小来确定收缩大小
	minimal = d->ht[0].used;

	//字典中ht的大小最小都要为 DICT_HT_INITIAL_SIZE 4
	if (minimal < DICT_HT_INITIAL_SIZE)
		minimal = DICT_HT_INITIAL_SIZE;

	return dictExpand(d, minimal);
}

//创建一个新哈希表，并看情况执行下面行为之一
//
//	1)如果dict中ht[0]为空，新哈希表作为ht[0[
//	2)如果dict中ht[0]不为空，将新哈希表赋值给ht[1],并打开rehash标志
//	
//	O(N)
int dictExpand(dict *d, unsigned long size)
{
	//一个新的ht
	dictht n;
	//计算ht的真实大小 即适合size的最小ht大小
	unsigned long realsize = _dictNextPower(size);

	//如果表中使用的元素大小比size大 这个size就是无效的 因为这样expand的表也无法放下所有的ht[0]中的元素
	if (dictIsRehashing(d) || d->ht[0].used > size)
		return DICT_ERR;

	//确定新表的属性 为新表分配空间
	n.size = realsize;
	n.sizemask = realsize -1;
	n.table = zmalloc(realsize * sizeof(dictEntry*));
	n.used = 0;

	//ht[0]为空 这是一次创建新表行为 将n作为ht[0]后返回
	if (d->ht[0].table == NULL){
		d->ht[0] = n;
		return DICT_OK;
	}
	
	//ht[0]不为空 这是一次扩展字典ht的行为 
	//将n作为ht[1] 并设置rehash标识 表示需要rehash了
	d->ht[1] = n;
	d->rehashidx = 0;

	return DICT_OK;
}

//执行N步渐进式rehash ht可以是一步一步慢慢rehash的 
//在用户的每一次Add find delete 等操作中进行 这样可以不消耗服务器时间 又使得表rehash进行中
//
//如果执行之后哈希表还有元素需要rehash，返回-1
//如果哈希表里的所有元素都已经迁移完毕，返回0
//
//每步rehash都会移动哈希表数组内顺序第一个索引上的整个链表
//
//返回 0：rehash完毕 1：rehash还在进行
//O(N)
int dictRehash(dict *d, int n)
{
	if (!dictIsRehashing(d))
		return 0;
	while (n--){
		dictEntry *de, *nextde;
	
		//ht[0]为空 已经迁移完毕 
		//跳出步骤rehash 用ht[1]替代ht[0] rehash完成
		if (d->ht[0].used == 0){
			zfree(d->ht[0].table);
			d->ht[0] = d->ht[1];
			_dictReset(&d->ht[1]);
			d->rehashidx = -1;
			return 0;
		}
		
		//确保ht[0]的大小要比 rehashidx 大 rehashidx表示要迁移的个数
		assert(d->ht[0].size > (unsigned)d->rehashidx);
		//移动到ht[0]中第一个不为NULL的链表的index上
		while (d->ht[0].table[d->rehashidx] == NULL)
			d->rehashidx++;
		//指向此处的链表头
		de = d->ht[0].table[d->rehashidx];
		
		//依次迁移链表的每一个dictEntry
		while (de){
			unsigned int h;
			nextde = de->next;
			//根据ht[0]此时要迁移的dictEntry的key和sizemask计算出在ht[1]中的index
			h = dictHashKey(d, de->key) & d->ht[1].sizemask;
	
			//将此时de指向的dictEntry添加到ht[1]的table[h]后
			de->next = d->ht[1].table[h];
			d->ht[1].table[h] = de;

			d->ht[0].used--;
			d->ht[1].used++;

			de = nextde;
		}
		//整个链表迁移到了ht[1] 将其指针设置为NULL 下次rehash时这个链表将被跳过不被发现
		d->ht[0].table[d->rehashidx] = NULL;
		//进行到下一个index的迁移
		d->rehashidx++;
	}
	//此时表示n步迁移后rehash还是没有完全完成
	return 1;
}

//返回当前时间 单位ms
long long timeInMilliseconds(void)
{
	struct timeval tv;
	
	//获得当前时间 并以ms为单位返回
	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec /1000);
}

//给定时间内，100步为单位对dict进行rehash
//返回值为rehash的步数
//O(N) N是被rehash的key-value数量
int dictRehashMilliseconds(dict *d, int ms)
{
	long long start = timeInMilliseconds();
	//保存步数
	int rehashes = 0;

	while (dictRehash(d, 100)){
		rehashes += 100;
		//这里表示超过了给定时间 单位时间rehash失败
		if ((timeInMilliseconds() - start) > ms)
			break;
	}

	return rehashes;
}

//将一个元素从ht[0] 迁移至 ht[1]
//渐进式 rehash 
//即条件允许的话 每次调研add find delete时都执行一次 不占用系统时间地慢慢rehash
//O(1)
static void dictRehashStep(dict *d)
{
	//此函数只有在没有安全迭代器的时候可以被执行
	if (d->iterators == 0)
		dictRehash(d, 1);
}

//添加给定 key-value 对到字典
//O(1)
int dictAdd(dict *d, void *key, void *val)
{
	//为此key创建一个新entry
	dictEntry *entry = dictAddRaw(d, key);
	if (entry == NULL)
		return DICT_ERR;
	//将value关联到这个新节点的key上
	dictSetVal(d, entry, val);

	return DICT_OK;
}

//dictAdd底层实现，完成之后返回新节点
//如果key已经存在，返回NULL
//O(1)
dictEntry *dictAddRaw(dict *d, void *key)
{
	int index;
	dictEntry *entry;
	dictht *ht;
	
	//首先渐进式rehash一个元素
	if (dictIsRehashing(d))
		_dictRehashStep(d);

	//找到可容纳新元素的index
	//如果key存在 不添加
	if ((index = _dictKeyIndex(d, key)) == -1){
		return NULL;
	}

	//正在rehash时 新元素放在ht[1] ,不在rehash时 新元素直接放在ht[0]
	//这么做是为了加速rehash效率
	ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0];
	//为新元素分配空间
	entry = zmalloc(sizeof(struct dictEntry));
	if (entry == NULL)
		return NULL;
	//采用头插入将新元素放在表的index位置
	entry->next = ht->table[index];
	//重新设置这个index的表头
	ht->table[index] = entry;
	//更新使用节点数量 表示新节点已经插入成功了
	ht->used++;

	//将key关联到这个entry上
	dictSetKey(d, entry, key);

	//返回这个节点
	return entry;
}

//新key替换原有key
//如果key不存在，添加key-value到hashTable中
//如果是新添加key，返回1 
//如果是更新key，返回0
//O(1)
int dictReplace(dict *d, void *key, void *val)
{
	dictEntry *entry, auxentry;
	//此时key不存在 执行的是dictAdd操作
	if (dictAdd(d, key, val) == DICT_OK)
		return 1;

	//dictAdd操作失败 说明此key已经存在
	//找到这个key关联的entry
	entry = dictFind(d, key);
	//保存到旧的entry
	auxentry = *entry;
	//设置新的value
	dictSetVal(d, entry, val);
	//将旧的entry释放
	dictFreeVal(d, &auxentry);

	//实现了更新key的value操作
	return 0;
}

//dictReplace底层实现
//无论在新添加节点还是更新节点情况下
//都返回key所对应的节点
//O(1)
dictEntry *dictReplaceRaw(dict *d, void *key)
{
	dictEntry *entry = dictFind(d, key);

	//找到了这个entry就直接entry
	//没找到就创建一个新的entry返回
	return entry ? entry : dictAddRaw(d, key);
}

//按key查找并删除节点
//O(1)
static int dictGenericDelete(dict *d, const void *key, int nofree)
{
	unsigned int h, idx;
	dictEntry *he, *prevHe;
	int table;

	//空表 无法查找删除
	if (d->ht[0].size == 0)
		return DICT_ERR;

	//这个操作也要尝试 渐进rehash
	if (dictIsRehashing(d))
		_dictRehashStep(d);
	
	//获得哈希值
	h=dictHashKey(d, key);

	//在两个ht中查找 首先从ht[0]开始
	for (table = 0; table <= 1; table++){
		//计算出index
		idx = h & d->ht[table].sizemask;
		//指向当前表中获得的index对应的表头
		he = d->ht[table].table[idx];
		prevHe = NULL;
		
		//遍历链表
		while (he){
			//对比两个key
			//结果不为0时 表示两个key相等
			if (dictCompareKeys(d, key, he->key)){
				//prevHe不为空的时候，表示这个节点不是链表头
				if (prevHe)
					prevHe->next = he->next;
				else
					//这里表示找到的这个节点是链表头 直接跳过ta指向它的下一个
					d->ht[table].table[idx] = he->next;		//这样就表示将找到的那个key的节点排除在链表之外了

				//nofree为0时 表示要删除并释放这个节点的key-value
				if (!nofree){
					dictFreeKey(d, he);
					dictFreeVal(d, he);
				}
				//把这个节点释放掉
				zfree(he);
				d->ht[table].used--;
				return DICT_OK;
			}
			//两个key不相等 查找链表中的下一个key
			prevHe = he;
			he = he->next;
		}
		//不在rehash时 ht[1]中没有值 是空表 无需遍历查找
		if (!dictIsRehashing(d))
			break;
	}
	//没找到
	return DICT_ERR;
}

//删除哈希表中的key，并释放保存这个key的节点
//O(1)
int dictDelete(dict *ht, const void *key)
{
	return dictGemericDelete(ht, key, 0);
}

//删除哈希表中的key，但不释放保存这个key的节点
//并不知道为什么要有删除但不释放节点的操作 意义在哪里呢
//
//O(1)
int dictDeleteNoFree(dict *ht, const void *key)
{
	return dictGemericDelete(ht, key, 1);
}

//销毁给定的哈希表
//O(N)
int _dictClear(dict *d, dictht *ht)
{
	unsigned long i;
	
	//遍历ht 当ht为空或者ht已经被重置时跳出
	for (i = 0; i < ht->size && ht->used > 0; i++){
		dictEntry *he, *nextHe;
		
		//链表为空时不用free
		if ((he = ht->table[i]) == NULL)
			continue;

		//遍历链表 free掉链表中所有的key和value
		while (he){
			nextHe = he->next;
			dictFreeKey(d, he);
			dictFreeVal(d, he);
			zfree(he);
			ht->used--;
			
			he = nextHe;
		}
	}
	//free掉ht的结构并重置ht
	zfree(ht->table);
	_dictReset(ht);

	return DICT_OK;
}

//清空并释放dict
//O(N)
void dictRekease(dict *d)
{
	_dictClear(d, &d->ht[0]);
	_dictClear(d, &d->ht[1]);

	zfree(d);
}

//在dict中查找给定的key所定义的dictEntry
//如果key不存在，返回NULL
//O(1)
dictEntry *dictFind(dict *d, const void *key)
{
	dictEntry *he;
	unsigned int h, idx, table;
	//空表
	if (d->ht[0].size == 0)
		return NULL;
	//这个函数也要尝试渐进rehash
	if (dictIsRehashing(d))
		_dictRehashStep(d);

	//计算哈希值
	h = dictHashKey(d, key);
	//遍历两个ht
	for (table = 0; table <= 1; table++){
		//计算index
		idx = h & d->ht[table].sizemask;
		//获得这个key对应的链表的头
		he = d->ht[table].table[idx];

		while (he){
			//两个key相等 即找到了对应表中的key的dictEntry
			if (dictCompareKeys(d, key, he->key))
				return he;
			//不相等 继续对比下一个
			he = he->next;
		}
		
		//不在rehash状态 ht[1]是空表 无需查找
		if (!dictIsRehashing(d))
			return NULL;
	}
	//没找到
	return NULL;
}

//返回dict中key对应的value
//如果key不存在，返回NULL
//O(1)
void *dictFetchValue(dict *d, const void *key)
{
	dictEntry *he;
	he = dictFind(d, key);
	return he ? dictGetVal(he) : NULL;
}

//创建dict的不安全iterator
//iter->safe = 0
//O(1)
dictIterator *dictGetIterator(dict *d)
{
	dictIterator *iter = zmalloc(sizeof(struct dictIterator));
	if (iter == NULL)
		return NULL;

	iter->d = d;
	iter->table = 0;
	iter->index = -1;
	iter->safe = 0;
	iter->entry = NULL;
	iter->nextEntry = NULL;

	return iter;
}

//创建dict的安全iterator
//iter->safe = 1
//O(1)
dictIterator *dictGetSafeIterator(dict *d)
{
	dictIterator *iter = dictGetIterator(d);
	iter->safe = 1;
	return iter;
}

//返回iter指向的当前dictEntry
//如果dict已经被迭代完成 返回NULL
//O(1)
dictEntry *dictNext(dictIterator *iter)
{
	while (1){
		//当前指向的节点并没有值
		if (iter->entry == NULL){
			//获取到当前正在迭代的ht
			dictht *ht = &iter->d->ht[iter->table];

			//安全迭代器在运行时会增加dict中的迭代器数量属性
			if (iter->safe != 0 && iter->index == -1 && iter->table == 0){
				iter->d->iterators++;
			}
			
			//增加迭代器index index用来表示迭代器进行的长度
			iter->index++;
			
			//index大于这个表的长度 说明迭代完成了
			if (iter->index >= (signed)ht->size){
				//如果是rehash状态 那么继续迭代ht[1]
				if (dictIsRehashing(iter->d) && iter->table == 0){
					iter->table++;
					iter->index = 0;	//从头开始迭代
					ht = &iter->d->ht[1];
				}else {
					//此时说明没有必要迭代ht[1] 或者已经迭代完了ht[1]
					break;
				}
			}
			//根据index指向下一个节点的链表
			iter->entry = ht->table[iter->index];
		}else {
			//这里表示这个迭代器指向的节点有值 直接返回下一个节点
			iter->entry = iter->nextEntry;
		}
		
		//保存到下一个节点的指针以防当前节点的entry被修改了
		if (iter->entry){
			iter->nextEntry = iter->entry->next;
			return iter->entry;
		}
	}
	//迭代完成
	return NULL;
}

//释放迭代器
//如果是安全迭代器 需要修改dict中iterators属性的值
//O(1)
void dictReleaseIterator(dictIterator *iter)
{
	if (iter->safe != 0 && !(iter->index == -1 && iter->table == 0)){
		iter->d->iterators--;
	}
	zfree(iter);
}

//从dict中返回一个随机节点
//如果dict为空，返回NULL
//O(N)
dictEntry *dictGetRandomKey(dict *d)
{
	dictEntry *he, *orighe;
	unsigned int h;
	int listlen, listele;
	
	//空表 直接NULL
	if (dictSize(d) == 0)
		return NULL;

	//这个函数也可以渐进rehash
	if (dictIsRehashing(d))
		_dictRehashStep(d);

	//在rehash状态 两个表都非空 需要使用两个表
	if (dictIsRehashing(d)){
		do {
			//随机选取一个表
			h = random() % (d->ht[0].size + d->ht[1].size);
			//从选中表中获得一个非空头指针 如果随机数比ht[0]大 就选ht[1] 否则选ht[0]
			he = (h >= d->ht[0].size) ? d->ht[1].table[h - d->ht[0].size] : d->ht[0].table[h];
		}while (he == NULL);
	} else {
		//不在rehash状态 直接选中ht[0]中随机一个表头指针
		do {
			h = random() % d->ht[0].sizemask;
			he = d->ht[0].table[h];
		}while (he == NULL);
	}

	listlen = 0;
	orighe = he;
	//计算选中的这个链表的长度
	while (he){
		he = he->next;
		listlen++;
	}
	//根据长度随机获得一个数
	listele = random() % listlen;
	
	//根据这个随机数获得链表的一个节点
	he = orighe;
	while (listlen--)
		he = he->next;

	return he;
}

//根据需要扩展dict大小
//就是对ht[0]进行rehash
//O(N)
static int _dictExpandIfNeeded(dict *d)
{
	//如果在rehash中了 就没必要扩展ht[0]了 直接返回成功
	if (dictIsRehashing(d))
		return DICT_OK;

	//空表 直接扩展到DICT_HT_INITIAL_SIZE 大小即可
	if (d->ht[0].size == 0)
		return dictExpand(d, DICT_HT_INITIAL_SIZE);

	//当ht[0]已使用的节点 >= 规定的大小
	//并且ratio已经 > 给定的大小 
	//必须要对ht[0]进行扩展了
	//扩展的大小至少是 2*used
	//如果只是节点数比较多但是比率不大的话 可以不执行 否则必须要强制执行 不然影响查找效率
	if (d->ht[0].used >= d->ht[0].size && 
			(d->ht[0].used /d->ht[0].size > dict_force_resize_ratio)){
		return dictExpand(d, d->ht[0].used * 2);
	}

	return DICT_OK;
}

//计算哈希表的真实体积
//如果size < DICT_HT_INITIAL_SIZE 返回 DICT_HT_INITIAL_SIZE
//否则返回 >=size 最小的2的幂的大小  如size=24 最小要返回到32
//O(N)
static unsigned long _dictNextPower(unsigned long size)
{
	unsigned long i = DICT_HT_INITIAL_SIZE;

	if (size >= LONG_MAX)
		return LONG_MAX;
	while (1){
		if (i >= size)
			return i;
		i *= 2;
	}
}


//返回给定key 在哈希表数组存放的index
//如果key已经存在于哈希表 返回-1
//如果正在执行 rehash时
//返回的 index 总是应用于第二个哈希表
//O(1)
static int _dictKeyIndex(dict *d, const void *key)
{
	unsigned int h, idx, table;
	dictEntry *he;
	
	//要先对dict进行扩展
	if (_dictExpandIfNeeded(d) == DICT_ERR)
		return -1;

	//计算哈希值
	h = dictHashKey(d, key);

	//遍历两个ht
	for (table =0; table <= 1; table++){
		//计算出index
		idx = h & d->ht[table].sizemask;
		//获得index对应的链表的表头
		he = d->ht[table].table[idx];

		//表头不为NULL 
		while (he){
			//比较两个key 相等表示表中已经有此key 返回-1
			if (dictCompareKeys(d, key, he->key))
				return -1;
			//遍历
			he = he->next;
		}
		
		//ht[0]已经遍历完了
		//不在rehash状态 ht[1]为NULL
		if (!dictIsRehashing(d))
			break;
	}
	//返回这个index
	return idx;
}

//清空整个dict
//O(N)
void dictEmpty(dict *d)
{
	_dictClear(d, &d->ht[0]);
	_dictClear(d, &d->ht[1]);
	d->rehashidx = -1;
	d->iterators = 0;
}

//打开 rehash 标识
//O(1)
void dictEnableResize(void)
{
	dict_can_resize = 1;
}

//关闭 rehash 标识
//O(1)
void dictDisableResize(void)
{
	dict_can_resize = 0;
}


