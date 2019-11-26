#include "rb_tree.h"

int main()
{
	int A[6]={41,38,31,12,19,8};
	RB_Tree tree;
	tree.RB_Create(A,6);
	cout<<"按照层次遍历的顺序输出红黑树结构"<<endl;
	tree.LevelOrder_Traverse();
	cout<<endl;
	cout<<"中序遍历红黑树：";
	tree.InOrder_Traverse();
	cout<<endl;

	int k;
	while(1)
	{
		cout<<"输入要插入红黑树的关键字的值，输入-1表示结束输入：";
		cin>>k;
		if(k == -1)
			break;
		else
		{
			Tree_Node *t=new Tree_Node(k);
			if(tree.RB_Insert(t))
				cout<<"值为 "<<k<<" 的结点插入成功"<<endl;
		}
	}

	cout<<"按照层次遍历的顺序输出红黑树结构"<<endl;
	tree.LevelOrder_Traverse();
	cout<<endl;
	cout<<"中序遍历红黑树：";
	tree.InOrder_Traverse();
	cout<<endl;

	while(1)
	{
		cout<<"输入要删除的结点的关键字值，输入-1表示结束：";
		cin>>k;
		if(k == -1)
			break;
		else
		{
			if(tree.RB_Delete(k))
				cout<<"关键字为 "<<k<<" 的结点删除成功"<<endl;
		}
	}

	cout<<"按照层次遍历的顺序输出红黑树结构"<<endl;
	tree.LevelOrder_Traverse();
	cout<<endl;
	cout<<"中序遍历红黑树：";
	tree.InOrder_Traverse();
	cout<<endl;

	return 0;
}