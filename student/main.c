#include "student.h"
#include "teacher.h"
#include "root.h"
#include "common.h"

int main()
{
	char option = 0;
	system("clear");
	while (1)
	{
		menu_position();
		option = getchar();
		switch (option)
		{
			case '1':
				{
					StudentOption();
					break;
				}
			case '2':{
					TeacherOption();
					break;
				}
			case '3':{
					RootOption();
					break;
				}
			case '4':
				{
					returnSystem();
					return;
				}
			default :
				{
//					inputError();
					break;
				}
		}
	}
}
