#include "binary_search_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

SearchTree MakeEmpty(SearchTree T)
{
	if (T != NULL){
		MakeEmpty(T->leftChild);
		MakeEmpty(T->rightChild);
		free(T);
	}
	return NULL;			
}

Position FindMin(SearchTree T)
{
	if (T == NULL)
		return NULL;
	else if (T->leftChild == NULL)
		return T;
	else 
		return FindMin(T->leftChild);
}

Position FindMax(SearchTree T)
{
	if (T != NULL){
		while (T->rightChild != NULL)
			T = T->rightChild;
	}

	return T;
}

Position Find(ElementType x, SearchTree T)
{
	if (T == NULL)
		return NULL;
	else if (x < T->Element)
		return Find(x, T->leftChild);
	else if (x > T->Element)
		return Find(x, T->rightChild);
	else 
		return T;
}

SearchTree Insert(ElementType x, SearchTree T)
{
	if (T == NULL){
		T = (SearchTree)malloc(sizeof(struct TreeNode));
		if (T == NULL){
			perror("Insert");
		}else {
			T->Element = x;
			T->leftChild = T->rightChild = NULL;
		}
	}else if (x < T->Element){
		T->leftChild = Insert(x, T->leftChild);
	}else if (x > T->Element){
		T->rightChild = Insert(x, T->rightChild);
	}
	//else x is in the tree already ,do nothing
	return T;
}

SearchTree Delete(ElementType x, SearchTree T)
{
	Position tmpCell;
	if (T == NULL)
		perror("Delete: Element is not find");
	else if (x < T->leftChild->Element){
		T->leftChild = Delete(x, T->leftChild);
	}else if (x > T->rightChild->Element){
		T->rightChild = Delete(x, T->rightChild);
	}else if(T->leftChild && T->rightChild){
		tmpCell = FindMin(T->rightChild);
		T->Element = tmpCell->Element;
		T->rightChild = Delete(T->Element, T->rightChild);
	}else {
		tmpCell = T;
		if (T->leftChild == NULL)
			T = T->rightChild;
		else if (T->rightChild == NULL)
			T = T->leftChild;
		else
			free(tmpCell);
	}

	return T;
}

void PrintTree(SearchTree T)
{
	if (T == NULL)
		return;
	PrintTree(T->leftChild);
	printf("%d  ", T->Element);
	PrintTree(T->rightChild);
}

void PrePrintTree(SearchTree T)
{
	if (T == NULL)
		return;
	printf("%d  ", T->Element);
	PrePrintTree(T->leftChild);
	PrePrintTree(T->rightChild);
}

void PostPrintTree(SearchTree T)
{
	if (T == NULL)
		return;

	PostPrintTree(T->leftChild);
	PostPrintTree(T->rightChild);
	printf("%d  ", T->Element);
}
