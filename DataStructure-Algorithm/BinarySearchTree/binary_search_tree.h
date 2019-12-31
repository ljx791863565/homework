#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_ 

typedef int ElementType;
struct TreeNode{
	ElementType Element;
	struct TreeNode *leftChild;
	struct TreeNode *rightChild;
};
typedef struct TreeNode* Position;
typedef struct TreeNode* SearchTree;

SearchTree MakeEmpty(SearchTree T);
Position Find(ElementType x, SearchTree T);
Position FindMin(SearchTree T);
Position FindMax(SearchTree T);
SearchTree Insert(ElementType x, SearchTree T);
SearchTree Delete(ElementType x, SearchTree T);
ElementType Retrieve(Position P);

void PrintTree(SearchTree T);			//中序,在二叉树中会以典型的从小到大打印
void PrePrintTree(SearchTree T);		//先序
void postPrintTree(SearchTree T);		//后序,比如递归删除一个目录，必须保证在删除这个目录时此目录下的所有文件和目录都被删除，典型的后序遍历
#endif
