#include "communal.h"

void exit_menu()
{
	printf("正在退出...\n");
	sleep(1);
	return;
}

void default_menu()
{
	printf("输入有误，请重新输入...\n");
}

void main_menu()
{
	printf ("\t1.医生\t\n");
	printf ("\t2.病人\t\n");
	printf ("\t3.护士\t\n");
	printf ("\t4.管理员\t\n");
	printf ("\t5.退出\t\n");
	printf ("\t请选择\t\n");
}

void doctor_loop()
{
}

void patient_loop()
{

}

void nures_loop()
{

}

void root_loop()
{

}

void main_loop()
{
	int option;
	while (1)
	{
		main_menu();
		scanf("%d", &option);

		if (option == 1){
			doctor_loop();
		}else if (option == 2){
			patient_loop();
		}else if (option == 3){
			nures_loop();
		}else if (option == 4){
			root_loop();
		}else if (option == 5){
			exit_menu();	
		}else{
			default_menu();
		}
	}
}
