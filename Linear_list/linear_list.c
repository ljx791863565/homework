#include "linear_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

//初始化SqList
//成功返回TRUE 失败返回FALSE
int InitList(SqList &L)
{
	L.elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if (L.elem == NULL) {
		perror("malloc");
		exit(1);
	}
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;

	return true;
}

//在L的pos元素后添加一个元素e
//成功返回TRUE 失败返回FALSE
int ListInsert(SqList &L, int pos, ElemType e)
{
	assert(pos >= 1 && pos <= L.length);
	if (L.length >= L.listsize) {
		ElemType *newbase = (ElemType*)reallow(L.elem, (L.listsize+LISTINCREMENT) * sizeof (ElemType));
		if (newbase == NULL) {
			perror("malloc");
			exit(1);
		}
		L.elem = newbase;
		L.listsize += LISTINCRENENT;
	}

	int i;
	//从pos位置整体向后移动一位 腾出空间
	for (i = L.length; i > pos; i--) {
		elem[i+1] = elem[i];
	}
	elem[pos+1] = e;
	L.length ++;
	return true;
}

//删除元素pos 并将删除元素保存在e中
//成功返回TRUE 失败返回FALSE
int ListDelete(SqLit &L, int pos, Elem &e)
{
	assert(L.length > 0);
	if (pos < 1 || pos > L.length) {
		return false;
	}
	int i;
	e = elem[pos-1];
	for (i = pos; i < L.length; i++) {
		elem[i-1] = elem[i];
	}
	L.length--;
	return true;
}

//销毁列表
void DestoryList(SqList &L)
{
	if (L.elem != NULL) {
		free(L.elem);
	}
}

//清空列表所有元素
//成功返回TRUE 失败返回FALSE
int ClearList(SqList &L)
{
	if (L.elem == NULL) {
		return false;
	} else {
		free(L.elem);
		L.elem = (ELemType *)melloc(L.listsize * sizeof(ElemType));
		if (elem == NULL) {
			perror("malloc");
			exit(1);
		}
		L.length = 0;
		return true;
	}
}

//确认列表是否为空 
// TRUE：空 FALSE：非空
int ListEmpty(SqList &L)
{
	return L.elem == NULL ? true : false;
}

//获得列表第pos元素 保存在e中
int GetElem(SqList &L, int pos, Elem &e)
{
	if (L.elem == NULL) {
		return flase;
	} else {
		e = L.elem[pos -1];
		return true;
	}
	
}

//获得列表第pos元素前一个元素 保存在prev_e中
int PrevElem(SqList &L, int pos, Elem &prev_e)
{
	if (L.elem == NULL || pos <= 1 || pos > L.length) {
		return false;
	} else {
		prev_e = L.elem[pos-2];
		return true;
	}
}

//获得列表第pos元素下一个元素 保存在next_e中
int NextElem(SqList &L, int pos, Elem &next_e)
{
	if (L.elem == NULL || pos < 1 || pos >= L.length) {
		return false;
	} else {
		next_e = L.elem[pos+1];
		return true;
	}
}

//升序排序列表
void ListSort(SqList &L)
{
	assert((L.elem != NULL) && L.length > 1);
	int i, j;
	for (i = 0; i < L.length-1; i++) {	
		for (j = 1; j < length; j++) {
			if (L.elem[j] > L.elem[j+1]) {
				Elem e = L.elem[j];
				L.elem[j] = L.elem[j+1];
				L.elem[j+1] = e;
			}
		}
	}
}
