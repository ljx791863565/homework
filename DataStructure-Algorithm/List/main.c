#include "list.h"

int main()
{
	List L = NULL;
	L = MakeEmpty(L);
	InsterFront(8, L);
	PrintList(L);

	return 0;
}
