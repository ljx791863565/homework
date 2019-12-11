#include "common.h"

void menu_main()
{
	system("clear");
	printf("######################################\n");
	printf("\t1.注册\n");
	printf("\t2.登录\n");
	printf("\t3.退出\n");
	printf("######################################\n");
}
void menu_position()
{
	system("clear");
	printf("######################################\n");
	printf("\t1.你是老师\n");
	printf("\t2.你是学生\n");
	printf("\t3.你是管理员\n");
	printf("\t4.退出\n");
	printf("######################################\n");
}

void menu_root()
{
	system("clear");
	printf("######################################\n");
	printf("\t1.登录\n");
	printf("\t2.退出\n");
	printf("######################################\n");	
}

void returnSystem()
{
	printf("正在退出...\n");
	sleep(2);
}

void inputError()
{
	printf("输入有误1，请重新输入\n");
	sleep(1);
}

