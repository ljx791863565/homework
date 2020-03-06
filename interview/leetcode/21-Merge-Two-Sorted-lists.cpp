/**
*   struct ListNode
*  {
*		int val;
		ListNode *next;
*  };
	将给定的两个有序链表合成一个
   */

#include <iostream>
using namespace std;
class Solution
{
public:
	ListNode* mergeTwoLists(ListNode *l1, LisyNode *l2)
	{
		if (l1 == NULL)
			return l2;
		else if (l2 == NULL)
			return l1;
		else if (l1->val <= l2->val){
			l1->next = mergeTwoLists(l1->next, l2);
			return l1;
		}else{
			l2->next = mergeTwoLists(l1, l2->next);
			return l2;
		}
	}
private:
	int val;

};

int main()
{
	return 0;
}
