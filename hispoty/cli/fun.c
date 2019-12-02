#include "root.h"
int sockfd;
int num;
void printf_root3()
{
	printf("\t1.显示医生信息\t\n");//只能看到基本信息
	printf("\t2.查询医生信息\t\n");
	printf("\t3.修改医生资料\t\n");
	printf("\t4.删除医生\t\n");
	printf("\t5.医生密码初始化\t\n");
	printf("\t6.退出\t\n");
	printf("请选择\n");
}
void printf_root6()
{
	printf("\t1.查看护士基本信息\t\n");
	printf("\t2.查看护士护理的病人信息\t\n");
	printf("\t3.按护理等级查询护士信息\t\n");
	printf("\t4.退出\t\n");
	printf("请选择");
}
void printf_root5()
{
	printf("\t1.显示护士信息\t\n");//只能看到基本信息
	printf("\t2.查询护士信息\t\n");
	printf("\t3.修改护士资料\t\n");
	printf("\t4.删除护士\t\n");
	printf("\t5.护士密码初始化\t\n");
	printf("\t6.退出\t\n");
	printf("请选择\n");
}
void printf_root4()
{
	printf("\t1.查看医生基本信息\t\n");
	printf("\t2.查看患者诊断信息\t\n");
	printf("\t3.按科室查询医生信息\t\n");
	printf("\t4.退出\t\n");
	printf("请选择");
}
void printf_root7()
{
	printf("\t1.显示病人信息\t\n");//只能看到基本信息
	printf("\t2.查询病人信息\t\n");
	printf("\t3.删除病人\t\n");
	printf("\t4.病人密码初始化\t\n");
	printf("\t5.退出\t\n");
	printf("请选择\n");
}
void printf_root8()
{
	printf("\t1.查看病人基本信息\t\n");
	printf("\t2.查看病人诊断信息\t\n");
	printf("\t3.退出\t\n");
	
	printf("请选择");
}


void fun()
{
	while(1)
	{
	printf_menu1();
	scanf("%d",&num);
	if(num == 1)
	{
		fun_doctor();
	}
	else if(num == 2)
	{
		fun_sicker();
	}
	else if(num ==3)
	{
		fun_nures();
	}
	else if(num == 4)
	{
		fun_root();
	}
	else if(num ==5)
	{
		printf("正在退出\n");
		sleep(1);
		return;
	}
	else
	{
		printf("输入有误重新输入\n");
	}
	}
}

/*
void fun_doctor()
{
	while(1)
	{
		printf_doctor1();
		scanf("%d",&num);
		if(num == 1)
		{
			myRegister_doctor(sockfd);
		}
		else if(num == 2)
		{
			myLogin_doctor(sockfd);
			while(flag1 = 0);
//			printf("109 flag = %d\n",flag);
			if(flag == 0)
			{
				while(1)
				{
					printf_doctor2();
					scanf("%d",&num);
					if(num == 1)
					{
						mySelect_doctor(sockfd);
					}
					else if(num == 2)
					{
						myTurn_doctor(sockfd);
					}
					else if(num == 3)
					{
						myYizhu_doctor(sockfd);
					}
					else if(num == 4)
					{
						myYizhu_update_doctor(sockfd);
					}
					else if(num == 5)
					{
						myYizhu_delete_doctor(sockfd);
					}
					else if(num == 6)
					{
						mySelect_all_doctor(sockfd);
					}
					else if(num == 7)
					{
						printf("正在退出\n");
						sleep(1);
						return;
					}
					else
					{
						printf("输入有误重新输入\n");
					}
				}
			}
//			break;
		}
		else if(num == 3)
		{
			printf("正在退出\n");
			sleep(1);
			return;
		}
		else
		{
			printf("输入有误重新输入\n");
		}
	}
}
void fun_nures()
{
	while(1)
	{
		printf_doctor1();
		scanf("%d",&num);
		if(num == 1)
		{
			myRegister_nures(sockfd);
		}
		else if(num == 2)
		{
			while(1)
			{
				myLogin_nures(sockfd);
				while(flag = 0);
				{
				if(flag == 0)
				{
					while(1)
					{
					printf_nures1();
					scanf("%d",&num);
					if(num == 1)
					{
						mySelect_nures(sockfd);
					}
					else if(num == 2)
					{
						myTurn_nures(sockfd);
					}
					else if(num == 3)
					{
						mySelect_all_nures(sockfd);
					}
					else if(num == 4)
					{
						mySelect_yizhu_nures(sockfd);
					}
					else if(num == 5)
					{
						myDo_yizhu_nures(sockfd);
					}
					else if(num == 6)
					{
						printf("正在退出\n");
						sleep(1);
						return;
					}
					else
					{
						printf("输入有误重新输入\n");
					}
					}
				}
//				break;
				}
			}
		}
		else if(num == 3)
		{
			printf("正在退出\n");
			sleep(1);
			return;
		}
		else
		{
			printf("输入有误重新输入\n");
		}
	}
}
void fun_sicker()
{
	while(1)
	{
		printf_doctor1();
		scanf("%d",&num);
		if(num == 1)
		{
			myRegister_sicker(sockfd);
		}
		else if(num == 2)
		{
			while(1)
			{
				myLogin_sicker(sockfd);
				while(flag = 0);
				if(flag == 0)
				{
					while(1)
					{
					printf_sicker1();
					scanf("%d",&num);
					if(num == 1)
					{
						myGuahao_sicker(sockfd);
					}
					else if(num == 2)
					{
						mySelect_sicker(sockfd);
					}
					else if(num == 3)
					{
						myLook_sicker(sockfd);
					}
					else if(num == 4)
					{
						myTurn_sicker(sockfd);
					}
					else if(num == 5)
					{
						printf("正在退出\n");
						sleep(1);
						return;
					}
					else
					{
						printf("输入有误重新输入\n");
					}
					}
				}
//				break;
			}
		}
		else if(num == 3)
		{
			printf("正在退出\n");
			sleep(1);
			return;
		}
		else
		{
			printf("输入有误重新输入\n");
		}
	}
}
void fun_root()
{
	while(1)
	{
		printf_root1();
		scanf("%d",&num);
		if(num == 1)
		{
			while(1)
			{
				mylogin_root(sockfd);
				while(flag = 0);
				if(flag == 0)
				{
					while(1)
					{
					printf_root2();
					scanf("%d",&num);
					if(num == 1)
					{
						fun_root_doctor();
					}
					else if(num == 2)
					{
						fun_root_nures();
					}
					else if(num == 3)
					{
						fun_root_sicker();
					}
					else if(num == 4)
					{
						printf("正在退出\n");
						sleep(1);
						return;
					}
					else
					{
						printf("输入有误重新输入\n");
					}
					}
				}
//				break;
			}
		}
		else if(num == 2)
		{
			printf("正在退出\n");
			sleep(1);
			return;
		}
		else
		{
			printf("输入有误重新输入\n");
		}

	}
}

*/
void fun_root_doctor()
{
	while(1)
	{
		printf_root3();
		scanf("%d",&num);
		switch(num)
		{
			case 1:
				{
					mySelect_root(sockfd);
					break;
				}
			case 2:
				{
					fun_root_doctor_1();
					break;
				}
			case 3:
				{
					myUpdate_doctor_root(sockfd);
					break;
				}	
			case 4:
				{
					myDelete_doctor_root(sockfd);
					break;
				}	
				case 5:
				{
					mySelect_passwd_doctor_root(sockfd);
					break;
				}	
			case 6:
				{
					printf("正在退出\n");
					sleep(1);
					return;
				}
			default:
				{
					printf("输入错误重新输入\n");
					break;
				}
		}
	}
		
}

void fun_root_doctor_1()
{
	while(1)
	{
		printf_root4();
		scanf("%d",&num);
		switch(num)
		{
			case 1:
				{
					mySelect_doctor_root(sockfd);
					break;
				}
			case 2:
				{
					mySelect_sick_doctor_root(sockfd);
					break;
				}
			case 3:
				{
					mySelect_class_doctor_root(sockfd);
				}
			case 4:
				{
					printf("正在退出\n");
					sleep(1);
					return;
				}
			default:
				{
					printf("输入有误重新输入\n");
					break;
				}
		}
	}
}
void fun_root_nures()
{
	while(1)
	{
		printf_root5();
		scanf("%d",&num);
		switch(num)
		{
			case 1:
				{
					mySelect_root_nures(sockfd);
					break;
				}
			case 2:
				{
					fun_root_nures_1();
					break;
				}
			case 3:
				{
					myUpdate_nures_root(sockfd);
					break;
				}
			case 4:
				{
					myDelete_nures_root(sockfd);
					break;
				}
			case 5:
				{
					mySelect_passwd_nures_root(sockfd);
					break;
				}
			case 6:
				{
					printf("正在退出\n");
					sleep(1);
					return;
				}
			default:
				{
					printf("输入错误重新输入\n");
					break;
				}
		}
	}
		
}

void fun_root_nures_1()
{
	while(1)
	{
		printf_root6();
		scanf("%d",&num);
		switch(num)
		{
			case 1:
				{
					mySelect_root_nures(sockfd);
					break;
				}
			case 2:
				{
					mySelect_sick_nures_root(sockfd);
					break;
				}
			case 3:
				{
					mySelect_grade_nures_root(sockfd);
				}
			case 4:
				{
					printf("正在退出\n");
					sleep(1);
					return;
				}
			default:
				{
					printf("输入有误重新输入\n");
					break;
				}
		}
	}
}


void fun_root_sicker()
{
	while(1)
	{
		printf_root7();
		scanf("%d",&num);
		switch(num)
		{
			case 1:
				{
					mySelect_root_sicker(sockfd);
					break;
				}
			case 2:
				{
					fun_root_sicker_1();
					break;
				}
			case 3:
				{
					myDelete_sicker_root(sockfd);
					break;
				}
			case 4:
				{
					mySelect_passwd_sicker_root(sockfd);
					break;
				}
			case 5:
				{
					printf("正在退出\n");
					sleep(1);
					return;
				}
			default:
				{
					printf("输入错误重新输入\n");
					break;
				}
		}
	}
		
}

void fun_root_sicker_1()
{
	while(1)
	{
		printf_root8();
		scanf("%d",&num);
		switch(num)
		{
			case 1:
				{
					mySelect_sicker_root(sockfd);
					break;
				}
			case 2:
				{
					mySelect_sick_sicker_root(sockfd);
					break;
				}
			case 3:
				{
					printf("正在退出\n");
					sleep(1);
					return;
				}
			default:
				{
					printf("输入有误重新输入\n");
					break;
				}
		}
	}
}
