#include "SingleLinkedList.h"

int main()
{
	Node *head = NULL;
	head = frontInster(head, 8);
	head = frontInster(head, 0);
	head = frontInster(head, 3);
	head = frontInster(head, 21);
	head = frontInster(head, 9);
	head = backInster(head,9);
	head = backInster(head,54);
	head = backInster(head,43);
	head = backInster(head,32);
	head = backInster(head,3);
	head = backInster(head,32);

	display(head);

	Node *tmp = searchData(head, 0);
	if (tmp == NULL){
		printf("list has no the data\n");
	}
	printf("search the data is %d\n", tmp->data);

	head = deleteData(head, 43);
	display(head);
	head = deleteData(head, 3);
	display(head);
	head = reversalList(head);
	display(head);
	head = sortList(head);
	display(head);
	return 0;
}
