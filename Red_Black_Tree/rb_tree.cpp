#include "rb_tree.h"
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

//以结点x为中心进行左旋转
void RB_Tree::Leaf_Rotate(Tree_Node *x)
{
	Tree_Node *y=x->r_child;
	x->r_child=y->l_child;
	
	if(y->l_child != Nil)
		y->l_child->parent=x;
	y->parent=x->parent;
	
	if(x->parent == Nil)
		root=y;
	else if( x->parent->l_child == x )
		x->parent->l_child=y;
	else
		x->parent->r_child=y;

	y->l_child=x;
	x->parent=y;
}

//以x为中心进行右旋转
void RB_Tree::Right_Rotate(Tree_Node *x)
{
	Tree_Node *y=x->l_child;
	x->l_child=y->r_child;

	if(y->r_child != Nil)
		y->r_child->parent=x;
	y->parent=x->parent;

	if(x->parent == Nil)
		root=y;
	else if( x->parent->l_child == x)
		x->parent->l_child=y;
	else 
		x->parent->r_child=y;

	y->r_child=x;
	x->parent=y;
}

//在红黑树中搜索关键字值为k的结点
bool RB_Tree::RB_Search(int k,Tree_Node *&x)
{
	//红黑树是在二叉排序树的基础上建立的，所以也是有序的
	x=root;
	while(x != Nil)
	{
		if(x->key == k)
			return true;
		else if(x->key < k)
			x=x->r_child;
		else
			x=x->l_child;
	}
	return false;
}

//在红黑树中插入关键字值为k的结点
bool RB_Tree::RB_Insert(Tree_Node *k)
{
	Tree_Node *z;
	if(RB_Search(k->key,z))
	{
		cout<<"含有关键字值为 "<<k->key<<" 的结点已存在，不重复插入"<<endl;
		return false;
	}
	else
	{
		Tree_Node *x=root;
		Tree_Node *y=Nil;
		while(x != Nil)
		{
			y=x;
			if(k->key < x->key)
				x=x->l_child;
			else
				x=x->r_child;
		}
		k->parent=y;
		if(y == Nil)
			root=k;
		else if( k->key <y->key)
			y->l_child=k;
		else
			y->r_child=k;

		k->l_child=Nil;
		k->r_child=Nil;
		k->color=RED;
		if(k == root)
			k->parent=Nil;
		RB_Insert_Fixup(k);
	}
	return true;
}

//插入后调整红黑树结构,使插入新元素后的树满足红黑树的性质要求
void RB_Tree::RB_Insert_Fixup(Tree_Node *z)
{
	Tree_Node *y;
	while( z->parent->color == RED)
	{
		if( z->parent == z->parent->parent->l_child )
		{
			y=z->parent->parent->r_child;
			if(y->color == RED)
			{
				//case 1
				z->parent->color=BLACK;
				y->color=BLACK;
				z->parent->parent->color=RED;
				z=z->parent->parent;
			}
			else 
			{
				// y->color=BLACK
				if(z == z->parent->r_child)
				{
					//case 2 
					z=z->parent;
					Leaf_Rotate(z);
				}
				//case 3
				z->parent->color=BLACK;
				z->parent->parent->color=RED;
				Right_Rotate(z->parent->parent);
			}

		}
		else
		{
            //z->parent == z->parent->parent->r_child
			y=z->parent->parent->l_child;
			if(y->color == RED)
			{
				z->parent->color=BLACK;
				y->color=BLACK;
				z->parent->parent->color=RED;
				z=z->parent->parent;
			}
			else
			{
				if(z == z->parent->l_child)
				{
					z=z->parent;
					Right_Rotate(z);
				}
				z->parent->color=BLACK;
				z->parent->parent->color=RED;
				Leaf_Rotate(z->parent->parent);
			}
		}
	}
	root->color=BLACK;
}

//利用数组A[]中的数据创建红黑树
void RB_Tree::RB_Create(int A[],int n)
{
	for(int i=0;i<n;i++)
	{
		Tree_Node *k=new Tree_Node(A[i]);
		if(RB_Insert(k))
			cout<<"插入 "<<A[i]<<" 成功"<<endl;
	}
	cout<<"红黑树创建完成"<<endl;
}

//返回结点x的直接后继
Tree_Node *RB_Tree::RB_Successor(Tree_Node *x)
{
	Tree_Node *y;
	if(x->r_child != Nil)
	{
		y=x->r_child;
		while(y->l_child != Nil)
			y=y->l_child;
	}
	else
	{
		y=x->parent;
		while( (y != Nil) && (x == y->r_child) )
		{
			x=y;
			y=y->parent;
		}
	}
	return y;
}

//删除关键字为k的结点
bool RB_Tree::RB_Delete(int k)
{
	Tree_Node *x,*y,*z;
	if(!RB_Search(k,z))
	{
		cout<<"关键字为 "<<k<<" 的结点不存在红黑树中"<<endl;
		return false;
	}
	else
	{
		if( (z->l_child == Nil) || (z->r_child == Nil) )
			y=z;
		else
			y=RB_Successor(z);
		
		if(y->l_child != Nil) //说明z的左右子树只存在一个
			x=y->l_child;
		else
			x=y->r_child;  //说明z的左右子树均存在，y指向的是z的直接后继结点
		
		x->parent=y->parent;
		if(y->parent == Nil)
			root=x;
		else if(y == y->parent->l_child)
			y->parent->l_child=x;
		else
			y->parent->r_child=x;

		if(y!=z)  //y指向的是z的直接后继的时候
			z->key=y->key;
		
		if(y->color == BLACK)  //被删结点是黑色的时候，红黑树性质会被影响，所以需要重新调整
			RB_Delete_Fixup(x);
		
		delete y;
		return true;
	}
}

//删除后红黑树的结构出现不符合要求的情况时，调整树的结构
void RB_Tree::RB_Delete_Fixup(Tree_Node *x)
{
	Tree_Node *w;
	while( (x != root) && (x->color == BLACK) )
	{
		if(x == x->parent->l_child)
		{
			w=x->parent->r_child;
			if(w->color == RED)
			{
				//case 1
				w->color=BLACK;
				x->parent->color=RED;
				Leaf_Rotate(x->parent);
				w=x->parent->r_child;
			}

			if( (w->l_child->color == BLACK) && (w->r_child->color == BLACK) )
			{
				//case 2
				w->color=RED;
				x=x->parent;
			}
			else 
			{
				if (w->r_child->color == BLACK)
				{
					//case 3
					w->l_child->color=BLACK;
					w->color=RED;
					Right_Rotate(w);
					w=x->parent->r_child;
				}
				//case 4
				w->color=x->parent->color;
				x->parent->color=BLACK;
				w->r_child->color=BLACK;
				Leaf_Rotate(x->parent);
				x=root;
			}
		}
		else
		{
			//x=x->parent->r_child
			w=x->parent->l_child;
			if(w->color == RED)
			{
				//case 1
				w->color=BLACK;
				x->parent->color=RED;
				Right_Rotate(x->parent);
				w=x->parent->l_child;
			}
			
			if( (w->l_child->color == BLACK) && (w->r_child->color == BLACK) )
			{
				//case 2
				w->color=RED;
				x=x->parent;
			}
			else 
			{
				if (w->l_child->color == BLACK)
				{
					//case 3
					w->r_child->color=BLACK;
					w->color=RED;
					Leaf_Rotate(w);
					w=x->parent->l_child;
				}
				//case 4
				w->color=x->parent->color;
				x->parent->color=BLACK;
				w->l_child->color=BLACK;
				Right_Rotate(x->parent);
				x=root;
			}
		}
	}
	x->color=BLACK;
}

//层次遍历红黑树，并且输出各个结点的颜色
void RB_Tree::LevelOrder_Traverse()
{
	Tree_Node *y=root;
	queue<Tree_Node *> qu;
	qu.push(y);
	while(!qu.empty())
	{
		y=qu.front();
		qu.pop();
		cout<<"key= "<<y->key<<" ,color= ";
		if(y->color == BLACK)
			cout<<"BLACK"<<endl;
		else
			cout<<"RED"<<endl;
		if(y->l_child != Nil)
			qu.push(y->l_child);
		if(y->r_child != Nil)
			qu.push(y->r_child);
	}
}

//中序遍历红黑树各结点的关键字值
void RB_Tree::InOrder_Traverse()
{
	Tree_Node *y;
	stack<Tree_Node *> s;
	y=root;
	while( (y != Nil) || (!s.empty()) )
	{
		if(y != Nil)
		{
			s.push(y);
			y=y->l_child;
		}
		else
		{
			y=s.top();
			s.pop();
			cout<<y->key<<"  ";
			y=y->r_child;
		}
	}
}
