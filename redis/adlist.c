#include <stdlib.h>
#include "adlist.h"
#include "zmalloc.h"

//创建一个list
//成功返回list，失败返回NULL
//T=O(1)
list *listCreate(void)
{
	list *list;

	if ((list = (zmalloc(sizeof(struct list))) == NULL))
		return NULL;

	list->head = list->tail = NULL;
	list->len = 0;
	list->dup = NULL;
	list->free = NULL;
	list->match = NULL;

	return list;

}

//释放list以及list所有的listNode
//T=O(N)
void listRelease(list *list)
{
	unsigned long len;
	listNode *current, *next;
	current = list->head;
	len = list->len;

	while (len--){
		next = current->next;
		if (list->free)
			list->free(current->value);
		zfree(current);
		current = next;
	}
	zfree(list);
}

//头插
//出错返回NULL，不执行插入操作
//T=O(1)
list *listAddNodeHead(list *list, void *value)
{
	listNode *node;
	if ((node = zmalloc(sizeof(struct listNode))) == NULL)
		return NULL;

	node->value = value;
	if (list->len == 0){
		list->head = list->tail = node;
		node->prev = node->next = NULL;
	}else {
		node->prev = NULL;
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
	}
	list->len++;

	return list;

}

//尾插
//出错返回NULL，不执行插入操作
//T=O(1)
list *listAddNodeTail(list *list, void *value)
{
	listNode *node;
	if ((node = zmalloc(sizeof(struct listNode)) == NULL))
		return NULL;
	node->value = value;

	if (list->len == 0){
		list->head = list->tail = NULL;
		node->prev = node->next = NULL;	
	}else{
		node->next = NULL;
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	list->len++;

	return list;
}

/*
 *根据value值创建一个节点
 *after指示将新节点插入到old_node之前还是之后
 *after	0：之前 1：之后
 * T = O(1)
 */
list *listInsertNode(list *list, listNode *old_node, void *value, int after)
{
	listNode *node;
	if ((node = zmalloc(sizeof(struct listNode))) == NULL)
		return;
	if (after){
		node->prev = old_node;
		node->next = old_node->next;
		
		//old_node是尾节点
		if (list->tail == old_node){
			list->tail = node;
		}
	}else{
		node->next = old_node;
		node->prev = old_node->prev;
		
		//old_node是头节点
		if (list->head == old_node){
			list->head = node;
		}
	}
	
	//更新前后节点的指针指向
	if (node->prev != NULL){
		node->prev->next = node;
	}
	if (node->next != NULL){
		node->next->prev = node;
	}

	list->len++;

	return list;
}

//删除list的某个节点node
//T=O(1)
void listDelNode(list *list, listNode *node)
{
	//判断此节点是不是头节点
	//处理前面节点的指针
	if (node->prev){
		node->prev->next = node->next;
	}else{
		list->head = node->next;
	}

	//判断此节点是不是尾节点
	//处理后继节点的指针
	if (node->next){
		node->next->prev = node->prev;
	}else{
		list->tail = node->prev;
	}

	//既不是头节点也不是尾节点
	if (list->free){
		list->free(node->value);
	}
	zfree(node);
	list->len--;
}


//为列表创建一个迭代器
//direction指示方向 
//每次对迭代器调用 listNext(),迭代器就返回列表下一个节点
//不处理失败情况,默认列表存在且不为空
//T=O(1)
listIter *listGetIterator(list *list, int direction)
{
	listIter *iter;
	
	if ((iter = zmalloc(sizeof(struct listIter))) == NULL)
		return NULL;

	if (direction == AL_START_HEAD)
		iter->next = list->head;
	else
		iter->next = list->tail;

	iter->direction = direction;

	return iter;
}

listNode *liistNext(listIter *iter)
{
	listNode *current = iter->next;
	if (current != NULL){
		if (iter->direction == AL_START_HEAD)
			iter->next = current->next;
		else
			iter->next = current->prev;
	}

	return current;
}

//释放迭代器
void listReleaseIterator(listIter *iter)
{
	zfree(iter);
}

//创建列表的副本
//成功返回这个链表副本的指针，失败或者内存不足返回NULL
//成功或者失败都不会改变原链表的任何内容或属性
//
//T=O(N)
list *listDup(list *orig)
{
	list *copy;
	listIter *iter;
	listNode *node;

	if ((copy = listCreate()) == NULL)
		return;

	copy->dup = orig->dup;
	copy->free = orig->free;
	copy->match = orig->match;

	iter = listGetIterator(orig, AL_START_HEAD);
	while ((node = listNext(iter)) != NULL){
		void *value;
		if (copy->dup){
			value = copy->dup(node->value);
			if (value == NULL){
				listRelease(copy);
				listReleaseIterator(iter);
				return NULL;
			}
		}else{
			value = node->value;
		}

		if (listAddNodeTail(copy, value) == NULL){
			listRelease(copy);
			listReleaseIterator(iter);
			return NULL;
		}
	}
	listReleaseIterator(iter);
	return copy;
}

//在list中查找给定key的节点
//如果list有match，通过这个match匹配
//如果没有，直接将key和value匹配
//返回从head开始匹配到的第一个节点
//如果没找到，返回NULL
//
//T=O(N)
listNode *listSearchKey(list *list, void *key)
{
	listIter *iter;
	listNode *node;

	iter = listGetIterator(list, AL_START_HEAD);
	while ((node = listNext(iter)) != NULL){
		if (list->match){
			if (list->match(node->value, key)){
				//找到了，释放掉iter
				listReleaseIterator(iter);
				return node;
			}
		}else{
			//因为在创建value时就是直接浅拷贝，所以此时直接比较两个指针是否相当来判断是否找到
			if (key == node->value){
				listReleaseIterator(iter);
				return node;
			}
		}
	}

	//遍历完list没找到
	listReleaseIterator(iter);
	return NULL;
}

//根据给定index返回list对应的节点
//index可以是正数也可以是负数
//正数从0开始计数，从表头开始执行，负数从-1开始计数，从表尾开始执行
//如果index超出了list的len范围，返回NULL
//T=O(N) N = list->len
listNode *listIndex(list *list, long index)
{
	listNode *node;

	if (index < 0){
		index = (-index) -1;
		node = list->tail;
		while (index-- && node){
			node = node->prev;
		}
	}else{
		node = list->head;
		while (index-- && node)
			node = node->next;
	}

	return node;
}
//将迭代器的指针倒回到list表头
void listRewind(list *list, listIter *iter)
{
	iter->next = list->head;
	iter->direction = AL_START_HEAD;
}

//将迭代器的指针倒回到list的表尾
void listRewindTail(list *list, listIter *iter)
{
	iter->next = list->tail;
	iter->direction = AL_START_TAIL;
}

//将list表尾节点取出并插入到头节点之前
void listRotate(list *list)
{
	listNode *tl = list->tail;

	if (listLength(list) <= 1)
		return ;

	list->tail = tl->prev;
	list->tail->next = NULL;

	list->head->prev = tl;
	tl->next = list->head;
	list->head = tl;
}
