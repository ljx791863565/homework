#include "student.h"
#include "common.h"
void Student_Login_option(char *id)
{
	char option;
	while(1){
		menu_student();
		option = getchar();
		switch (option)
		{
			case '1':
				display_student_info(id);
				break;
			case '2':
				set_student_passwd(id);
				break;
			case '3':
				printf("正在返回上一级\n");
				sleep(1);
				return;
			default:
//				inputError();
				break;
		}
	}
}

void display_student_info(char *id)
{
	StudentInfo *s = LoadStudentInfo(id);
	if (s == NULL){
		perror("malloc");
		return;
	}
	printf("id:%s\t name:%s\t sex:%s\t college:%s\t grade:%s\t passwd:%s\t email:%s\t age:%d\n",
			s->id, s->name, s->sex, s->college, s->grade, s->passwd, s->email, s->age);
	sleep(3);
}

int set_student_passwd(char *id)
{
	StudentInfo *s = LoadStudentInfo(id);
	if (s == NULL){
		perror("malloc");
		return -1;
	}
	char passwd[20];
	char passwd2[20];
	printf("请输入原密码\n");
	scanf("%s", passwd);
	if (strcmp(s->passwd, passwd) != 0){
		printf("原始密码不正确\n");
		sleep(1);
		return -2;
	}else {
		printf("请输入新密码\n");
		scanf("%s", passwd);
		printf("请再次输入密码\n");
		scanf("%s", passwd2);

		if (strcmp(passwd, passwd2) != 0){
			printf("两次输入密码不正确\n");
			sleep(1);
			return -2;
		}
		strcpy(s->passwd, passwd);
		SaveStudentInfo(s);
		printf("修改密码成功\n");
	}
}
void StudentOption()
{
	char option;
	while (1)
	{
		menu_main();
		option = getchar();
		switch (option)
		{
			case '1':
				{
					StudentRegister();
					break;
				}
			case '2':
				{
					StudentLogin();
					break;
				}
			case '3':
				{
					printf("正在返回上一级\n");
					sleep(1);
					return;
				}
			default:
				{
//					inputError();
					break;
				}
		}
	}
}
void SaveStudentInfo(StudentInfo *sInfo)
{
	printf("保存学生注册信息\n");
	char addr[100];
	strcpy(addr, PWD);
	strcat(addr, sInfo->id);

	int fd = open(addr, O_WRONLY|O_CREAT|O_TRUNC, 0664);
	if (fd  < 0){
		perror("open");
		return;
	}
	int ret = write(fd, sInfo, sizeof(StudentInfo));
	if (ret < 0){
		perror("write");
		return;
	}
	close(fd);
	printf("学生注册信息保存成功，保存在%s\n",addr);
}
StudentInfo* LoadStudentInfo(char *id)
{
	StudentInfo *p =(StudentInfo*)malloc(sizeof(StudentInfo));
	if (p == NULL){
		perror("malloc");
		return NULL;
	}
	memset(p, 0, sizeof(StudentInfo));
	char addr[100];
	strcpy(addr, PWD);
	strcat(addr, id);
	int fd = open(addr, O_RDONLY);
	if (fd < 0){
		perror("open");
		return NULL;
	}	
	int ret = read(fd, p, sizeof(StudentInfo));
	if (ret < 0){
		perror("read");
		return NULL;
	}
	return p;

}
void StudentRegister()
{
	printf("学生注册\n");
	system("clear");

	StudentInfo s;
	memset(&s, 0, sizeof(StudentInfo));

	printf("输入学号\n");
	scanf("%s", s.id);
	printf("输入姓名\n");
	scanf("%s", s.name);
	printf("输入性别\n");
	scanf("%s", s.sex);
	printf("输入所属学院\n");
	scanf("%s", s.college);
	printf("输入年级\n");
	scanf("%s", s.grade);
	printf("输入密码\n");
	scanf("%s", s.passwd);
	printf("输入电子邮箱\n");
	scanf("%s", s.email);
	printf("输入年龄\n");
	scanf("%d", &s.age);

	printf("id:%s\t name:%s\t sex:%s\t college:%s\t grade:%s\t passwd:%s\t email:%s\t age:%d\n",
			s.id, s.name, s.sex, s.college, s.grade, s.passwd, s.email, s.age);
	SaveStudentInfo(&s);
	printf("注册成功，%s同学，请先登\n", s.name);
}

void StudentLogin()
{
	system("clear");
	printf("学生登录\n");

	printf("请输入学号\n");
	char id[20];
	scanf("%s", id);
	printf("请输入密码\n");
	char passwd[20];
	scanf("%s", passwd);

	StudentInfo *stmp = LoadStudentInfo(id);
	if (stmp == NULL){
		printf("id错误，请先注册\n");
		sleep(1);
		return;
	}
	if (strcmp(stmp->passwd, passwd) == 0){
		printf("登录成功\n");
		Student_Login_option(id);
	}else {
		printf("密码错误\n");
		sleep(1);
		return;
	}
}
