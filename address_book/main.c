#include <stdio.h>
#include <stdlib.h>

typedef struct _ADB {
	char name[20];
	char phone[12];
	char addr[32];
} ADB;
void main_menu_desplay(void)
{
	printf("|=======================================================|\n");
	printf("|			Address Book			|\n");
	printf("|							|\n");
	printf("|							|\n");
	printf("|			1.add				|\n");
	printf("|			2.search			|\n");
	printf("|			3.return			|\n");
	printf("|							|\n");
	printf("|							|\n");
	printf("|			please choice			|\n");
	printf("|=======================================================|\n");
}

void add_people()
{
	printf("this is add func\n");
	sleep(2);
}
void selete_people()
{
	printf("this is selete func\n");
	sleep(2);
}
int main(int argc, const char *argv[])
{
	int choise = 0;
	while (1){
		main_menu_desplay();
		scanf("%d",&choise);
		switch (choise)
		{
			case 1:
				add_people();
				break;
			case 2:
				selete_people();
				break;
			case 3:
				sleep(1);
				return 0;
			default:
				break;
		}
	}
	return 0;
}
