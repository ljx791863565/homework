#include <stdio.h>

//连个链表各自有序　请合并成一个新的有序链表

//递归法
Node *MergeList(Node *head1, Node *head2)
{
	if (head1 == NULL) {
		return head2;
	}
	if (head2 == NULL) {
		return head1;
	}

	Node *newhead = NULL;
	if (head1->data < head2->data) {
		newhead = head1;
		newhead->next = MergeList(newhead->next, head2);
	}else {
		newhead = head2;
		newhead->next = MergeList(head1, newhead->next);
	}

	return head;
}


