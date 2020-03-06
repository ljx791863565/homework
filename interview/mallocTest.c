#include <stdio.h>
#include <stdlib.h>
/*
 * man malloc
 * malloc() allocates size bytes and returns a pointer to the allocated memory. 
 * The memory is not cleared. If size is 0, then malloc() returns either NULL, 
 * or a unique pointer value that can later be successfully passed to free().
 *
 * malloc分配的内存是堆内存，由于堆没有自己的机器指令，所以要有系统自己编写算法来管理这片内存，通常的做法是用链表 
 * 在每片被分配的内存前加个表头，里面存储了被分配内存的起始地址和大小，你的malloc返回的就是表头里的起始指针，这个
 * 地址是由一系列的算法得来了，通常不会为0，一旦分配成功，就返回一个有效的指针，对于分配0空间来说，算法已经算出可
 * 用内存的起始地址，但是你占用0空间，所以对那个指针操作就是错误的，操作系统一般不知道其终止地址，因为有占用大小就
 * 可以推出终止地址，还有就是即使分配0空间也要释放它，其实是释放的链表结点。
 * 还有，返回的指针是可用地址的起始地址，可用大小是固定的，在VC6下是56字节，这个大小可能就是链表的大小
 */
int main()
{
	char *ptr;
	if ((ptr = (char*)malloc(0)) == NULL){
		printf("Got a null pointer\n");
	}else {
		printf("Got a valid pointer\n");
	}
}
