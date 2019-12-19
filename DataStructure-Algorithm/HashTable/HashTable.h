/*************************************************************************
    > File Name: HashTable.h
    > Author: ljx
    > Mail: liaojia138797@163.com 
    > Created Time: Thu 19 Dec 2019 10:09:33 AM CST
 ************************************************************************/
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKET_COUNT 16

//元素
typedef struct hashEntry{
	char *key;
	char *value;
	struct hashEntry *next;
}entry;

//表
typedef struct hashTable{
	entry bucket[BUCKET_COUNT];
}table;

void initHashTable(table *t);
void freeHashTable(table *t);
int keyToIndex(const char *key);
//char *strDup(const char *key);
int insertEntry(table *t, const char *key, const char *value);
const char *findValueByKey(const table *t, const char *key);
entry *removeEntry(table *t, char *key);
void printTbale(table *t);
#endif
