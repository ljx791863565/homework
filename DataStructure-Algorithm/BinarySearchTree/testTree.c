#include <stdio.h>
#include "binary_search_tree.h"

int main()
{
	SearchTree T = NULL;

	T = MakeEmpty(T);

	T = Insert(8, T);
	T = Insert(6, T);
	T = Insert(2, T);
	T = Insert(7, T);
	T = Insert(12, T);
	T = Insert(10, T);
	T = Insert(16, T);

	printf("中序遍历: ");
	PrintTree(T);
	printf("\n");
	printf("前序遍历: ");
	PrePrintTree(T);
	printf("\n");
	printf("后序遍历: ");
	PostPrintTree(T);
	printf("\n");
	return 0;
}
