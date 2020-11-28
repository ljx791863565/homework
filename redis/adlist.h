#ifndef _ADLIST_H_
#define _ADLIST_H_ 

/*
 * 双端链表节点 具有前后两个指针
 */
typedef struct listNode{
	struct listNode *next;
	struct listNode *prev;
	void *value;
}listNode;

/*
 * 链表迭代器 用于遍历链表
 */
typedef struct listIter{
	//下一节点
	listNode *next;
	//访问方向
	int direction;
}listIter;

typedef struct list{
	//表头节点
	listNode *head;	
	//表尾节点
	listNode *tail;
	//链表包含的节点数量
	unsigned long len;
	//节点复制函数
	void *(*dup)(void *ptr);
	//节点析构函数
	void (*free)(void *ptr);
	//节点对比函数
	int (*match)(void *ptr, void *key);
}list;


#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFreeMethod(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)


//创建一个不包含任何节点的链表 O(1)
list *listCreate(void);		
//释放给定链表和链表上的所有节点 O(N)
void listRelease(list *list);
//将一个给定值的节点插入到链表的表头 O(1)
list *listAddNodeHead(list *list, void *value);
//将一个给定值的节点插入到链表的表尾 O(1)
list *listAddNodeTail(list *list, void *value);
//将一个给定值的节点插入到链表old_node的前或后 after指示前后 O(1)
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
//删除链表的一个节点 O(N)
void listDelNode(list *list, listNode *node);
//创建一个链表迭代器 O(1)
listIter *listGetIterator(list *list, int direction);
//
listNode *liistNext(listIter *iter);
void listReleaseIterator(listIter *iter);
list *listDup(list *orig);
listNode *listSearchKey(list *list, void *key);
listNode *listIndex(list *list, long index);
void listRewind(list *list, listIter *iter);
void listRewindTail(list *list, listIter *iter);
void listRotate(list *list);

#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif	/* _ADLIST_H_ */
