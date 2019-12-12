#include "common.h"
void menu_student()
{
	system("clear");
	printf("######################################\n");
	printf("\t1.查看个人信息\n");
	printf("\t2.修改个人密码\n");
	printf("\t3.退出\n");
	printf("\t请选择\n");
	printf("######################################\n");
}
void menu_main()
{
	system("clear");
	printf("######################################\n");
	printf("\t1.注册\n");
	printf("\t2.登录\n");
	printf("\t3.退出\n");
	printf("\t请选择\n");
	printf("######################################\n");
}
void menu_position()
{
	system("clear");
	printf("######################################\n");
	printf("\t1.你是学生\n");
	printf("\t2.你是老师\n");
	printf("\t3.你是管理员\n");
	printf("\t4.退出\n");
	printf("\t请选择\n");
	printf("######################################\n");
}

void menu_root()
{
	system("clear");
	printf("######################################\n");
	printf("\t1.登录\n");
	printf("\t2.退出\n");
	printf("\t请选择\n");
	printf("######################################\n");	
}

void returnSystem()
{
	printf("正在退出...\n");
	sleep(1);
	return;
}

void inputError()
{
	printf("输入有误，请重新输入\n");
	sleep(1);
}

