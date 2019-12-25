#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * 实现双向循环链表的查找并移除
 */

typedef struct doublecyclelink{
	int key;
	struct doublecyclelink *prev;
	struct doublecyclelink *next;
}DoubleCycleLinkT;
DoubleCycleLinkT *findKey(DoubleCycleLinkT *link,int key)
{
	if (link == NULL){
		return NULL;
	}
	DoubleCycleLinkT *tmp = link->next;
	while (tmp->next != link){
		if (tmp->key == key){
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
			free(tmp);
			return link;
		}
		tmp = tmp->next;
	}
	return NULL;
}

