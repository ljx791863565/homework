#include "binaryTree.h"
#include <iostream>
using namespace std;

typedef char ElementType;

//回调函数 Visit() = PrintElement()
static void PrintElement(const ElementType &data)
{
	cout << data;
}

int main()
{
	CBTNode<ElementType> *pRoot;
	CBTNode<ElementType> *pLeftChild;
	CBTNode<ElementType> *pRightChild;


	pRoot = new CBTNode<ElementType>;
	if (NULL == pRoot){
		return EXIT_FAILURE;
	}

	pLeftChild = new CBTNode<ElementType>;
	if (NULL == pLeftChild){
		return EXIT_FAILURE;
	}

	pRightChild = new CBTNode<ElementType>;
	if (NULL == pRightChild){
		return EXIT_FAILURE;
	}

	//创建父节点
	pRoot->data = '+';
	pRoot->parent = NULL;
	pRoot->left = pLeftChild;
	pRoot->right = pRightChild;
	
	//创建左子节点
	pLeftChild->data = 'a';
	pLeftChild->parent = pRoot;
	pLeftChild->left = NULL;
	pLeftChild->right = NULL;

	//创建右子节点
	pRightChild->data = 'b';
	pRightChild->parent = pRoot;
	pRightChild->left = NULL;
	pRightChild->right = NULL;
	
	CBTree<ElementType> btree;
	//创建二叉树
	btree.AssignTo(pRoot);

	//输出这棵树
	cout << " (" << btree.GetNodeData(btree.GetRoot()) << ") " << endl;
	cout << "/ \\" << endl;
	cout << " (" << btree.GetNodeData(btree.GetLeftChild(btree.GetRoot())) 
		<< " ) (" << btree.GetNodeData(btree.GetRightChild(btree.GetRoot()))
		<< " ) " << endl << endl;
//	cout << "这棵树的叶子数:" << btree.GetLeafsCount() << endl;

	cout << "这棵树的深度: " << btree.GetDepth() << endl;

	cout << "先序遍历: ";
	btree.PreOrderTraverse(PrintElement);
	cout << endl;

	cout << "中序遍历: ";
	btree.InOrderTraverse(PrintElement);
	cout << endl;

	cout << "后序遍历: ";
	btree.PostOrderTraverse(PrintElement);
	cout << endl;


	return 0;
}
