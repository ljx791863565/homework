/*************************************************************************
    > File Name: HashTable.c
    > Author: ljx
    > Mail: liaojia138797@163.com 
    > Created Time: Thu 19 Dec 2019 10:18:31 AM CST
 ************************************************************************/
#include "HashTable.h"

//初始化哈希表
void initHashTable(table *t)
{
	int i;
	if (t == NULL)
		return;

	for (i = 0; i < BUCKET_COUNT; i++){
		t->bucket[i].key = NULL;
		t->bucket[i].value = NULL;
		t->bucket[i].next = NULL;
	}
}

///释放哈希表内存
void freeHashTable(table *t)
{
	int i;
	entry *e, *ep;
	if (t == NULL)
		return;

	for (i = 0; i < BUCKET_COUNT; i++){
		e = &(t->bucket[i]);
		while (e->next != NULL){
			ep = e->next;
			e->next = ep->next;
			free(ep->key);
			free(ep->value);
			free(ep);
		}
	}
}

//哈希散列方法
int keyToIndex(const char *key)
{
	int index, len, i;
	if (key == NULL)
		return -1;

	len = strlen(key);
	index = (int)key[0];
	for (i = 0; i <len; i++){
		index *= 1103515245 + (int)key[i];
	}
	index >>= 27;
	index &= (BUCKET_COUNT - 1);

	return index;
}
/*
char * strDup(const char *str)
{
	int len;
	char *ret;
	if (str == NULL)
		return NULL;

	len = strlen(str);
	ret = (char *)malloc(len+1);
	if (ret != NULL){
		memcpy(ret, str, len);
		ret[len] = '\0';
	}

	return ret;
}
*/


int insertEntry(table *t, const char *key, const char *value)
{
	int index, vlen1, vlen2;
	entry *e, *ep;

	if (t == NULL || key == NULL || value == NULL)
		return 0;

	//由key获得其散列值
	index = keyToIndex(key);

	//表中对应的节点的key为空，为这个index申请内存并拷贝
	if (t->bucket[index].key == NULL){
		t->bucket[index].key = strdup(key);
		t->bucket[index].value = strdup(value);
	}else {
		//表中key对应的节点有key值
		e = &(t->bucket[index]);
		ep = e;
		while (e != NULL){
			//此key值已经有节点了，申请一个新的节点并接在最后面
			if (!strcmp(e->key, key)){
				vlen1 = strlen(value);
				vlen2 = strlen(e->value);
				//如果要插入的value比已存在value长
				if (vlen1 > vlen2){
					free(e->value);	
					//扩大其value的内存大小
					e->value = (char *)malloc(vlen1 +1);
				}
				//update value
				memcpy(e->value, value, vlen1 +1);
				return index;
			}
			ep = e;
			e = e->next;
		}

		//表中对应的节点key不为空并且没有节点，插入一个节点，并拷贝值
		e = (entry*)malloc(sizeof(entry));
		e->key = strdup(key);
		e->value = strdup(value);
		e->next = NULL;
		ep->next = e;
	}
	return index;
}

//按key查找表中value
const char *findValueByKey(const table *t, const char *key)
{
	int index;
	const entry *e;
	if (t == NULL || key == NULL)
		return NULL;

	index = keyToIndex(key);
	e = &(t->bucket[index]);
	if (e->key == NULL)
		return NULL;	//由上可知t必然为NULL
	while (e != NULL){
		if (!strcmp(key, e->key)){
			return e->value;
		}
		e = e->next;
	}
	return NULL;
}

//按key 删除表中entry，只返回entry地址，若需要，手动free
entry *removeEntry(table *t, char *key)
{
	if (t == NULL || key == NULL)
		return NULL;

	int index = keyToIndex(key);
	entry *e = &(t->bucket[index]);
	entry *ep = NULL;
	while (e != NULL){
		if (!strcmp(key, e->key)){
			//这里表示这个key有多个节点，并且对应的key在第一个节点
			if (e == &(t->bucket[index])){
				ep = e->next;
				//这里表示这个节点后面还有节点
				if (ep != NULL){
					entry tmp = *ep;
					*e = *ep;
					*ep = tmp;

					ep->next = NULL;	//把对应的第一个节点删掉
				}
				//这里表示只有一个节点，把这个节点删掉，并保存地址在ep
				else {
					ep = (entry*)malloc(sizeof(entry));
					*ep = *e;
					e->key = e->value = NULL;
					e->next = NULL;
				}
			}
			//不在第一个节点，遍历找到这个对应节点
			else {
				ep = &(t->bucket[index]);
				while (ep->next != e)
					ep = ep->next;

				//
				ep->next = e->next;
				e->next = NULL;
				ep = e;
			}

			return ep;
		}
		e = e->next;
	}
	//这里表示这个key没有对应的节点
	return NULL;
}

void printTable(table *t)
{
	int i;
	entry *e = NULL;
	if (t == NULL)
		return;
	for (i = 0; i < BUCKET_COUNT; i++){
		printf("bucket[%d]:\n", i);
		e = &(t->bucket[i]);
		while (e->key != NULL){		
			printf("\t%s = %s\n", e->key, e->value);
			if (e->next == NULL)
				break;
			e = e->next;
		}
	}
}


