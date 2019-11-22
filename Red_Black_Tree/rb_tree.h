#include <iostream>
using namespace std;

//A binary search tree is a red-black tree if it satisfies the following red-black properties:
//	Every node is either red or black.
//	The root is black.
//	Every leaf (NIL) is black.
//	If a node is red, then both its children are black.
//	For each node, all paths from the node to descendant leaves contain the same number of black nodes.

//定义两种颜色作为全局变量
enum RB_COLOR{BLACK,RED};

class RB_Tree;   //前置声明

//红黑树节点类
class Tree_Node
{
	friend RB_Tree;
private:
	//红黑树结点的五个属性
	RB_COLOR color;       
	int key;
	Tree_Node *parent;
	Tree_Node *l_child;
	Tree_Node *r_child;
public:
	Tree_Node():color(BLACK),parent(NULL),l_child(NULL),r_child(NULL)
	{
	}
	Tree_Node(int k):color(BLACK),key(k),parent(NULL),l_child(NULL),r_child(NULL)
	{
	}
	Tree_Node(const Tree_Node &p)  //拷贝构造函数
	{
		color=p.color;
		key=p.key;
		l_child=p.l_child;
		r_child=p.r_child;
		parent=p.parent;
	}
	~Tree_Node()
	{
		parent=0;
		l_child=0;
		r_child=0;
	}
};

//红黑树类
class RB_Tree
{
private:
	Tree_Node *root; //根指针
	Tree_Node *Nil; //哨兵结点指针
public:
	RB_Tree()
	{
		Nil=new Tree_Node();	//哨兵结点的颜色是黑色的
		Nil->parent=Nil;
		Nil->l_child=Nil;
		Nil->r_child=Nil;
		root=Nil;
	}
	~RB_Tree()
	{
		delete Nil;
		Nil=NULL;
		root=NULL;
	}

	void Leaf_Rotate(Tree_Node *x);		 //左旋转
	void Right_Rotate(Tree_Node *x);	//右旋转

	bool RB_Search(int k,Tree_Node *&x);     //搜索

	bool RB_Insert(Tree_Node *x);     //插入
	void RB_Insert_Fixup(Tree_Node *z);   //插入后调整红黑树结构

	void RB_Create(int A[],int n);   //创建红黑树

	Tree_Node *RB_Successor(Tree_Node *x);  //返回结点x的直接后继

	bool RB_Delete(int k);    //删除
	void RB_Delete_Fixup(Tree_Node *x);  //删除后调整红黑树结构

	void LevelOrder_Traverse();  //层次遍历红黑树
	void InOrder_Traverse();  //中序遍历红黑树
};
