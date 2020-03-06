#include "binarySearchTree.h"

using namespace std;

int main()
{
	CBSTree<int> bstree;

	bstree.Insert(1);
	bstree.Insert(2);
	bstree.Insert(3);

	bstree.Delete(1);

	return 0;
}
