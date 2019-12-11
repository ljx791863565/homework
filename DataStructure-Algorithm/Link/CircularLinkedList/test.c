#include "DoubleLinkedList.h"

int main()
{
	Node *head = NULL;
	head = backInster(head, 63);
	head = backInster(head, 3);
	head = backInster(head, 13);
	head = backInster(head, 54);
	head = backInster(head, 1);
	head = frontInster(head, 109);
	head = frontInster(head, 2);
	head = frontInster(head, 3);
	head = frontInster(head, 45);
	head = frontInster(head, 99);
	head = frontInster(head, 8);

	display(head);
	reversalDisplay(head);

	Node* tmp = searchData(head, 54);
	if (tmp == NULL){
		printf("no search\n");
	}else {
		printf("search 0k. the data is %d\n", tmp->data);
	}

	head = deleteData(head, 3);
	display(head);

	sortList(head);
	display(head);

	return 0;
	
	return 0;
}
