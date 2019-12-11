#include "student.h"
#include "teacher.h"
#include "root.h"
#include "common.h"


int main()
{
	int option = 0;
	while (1){
		printf("hello workd\n");
		flag = 0;
		menu_position();
		option = getchar();
		switch (option)
		{
			case '1':
				flag = 1;
				menu_main();
				int option1 = getchar();
				switch (option1)
				{
					case '1':
						StudentRegister();
						break;
					case '2':
						StudentLogin();
						break;
					case '3':
						returnSystem();
						break;
					default:
						inputError();
				}
				break;
			case '2':
				flag = 2;
				menu_main();
				int option2 = getchar();
				switch (option2)
				{
					case '1':
						TeacherRegister();
						break;
					case '2':
						TeacherLogin();
						break;
					case '3':
						returnSystem();
						break;
					default:
						inputError();
				break;
			case '3':
				flag = 3;
				menu_root();
				int option3 = getchar();
				switch (option1)
				{
					case '1':
						RootLogin();
						break;
					case '2':
						returnSystem();
						break;
					default:
						inputError();
				break;
			case '4':
				returnSystem();
				goto done;
			default:
				inputError();
		}
	}
done:
	return 0;
}
