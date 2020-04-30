
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "student.h"

typedef struct StudentInfo {
	char name[20];
	int age;
	int grade;
	//student number 
	// 11020108  11:grade 02:major 01:class 08:id
	char id[9];	
	char identiy_card[19];
	char major[20];
	char phone[12];
} ST;

int checkMajor(const char *s)
{

}
void student_register()
{
	printf("注册\n");
	ST s;
	memset(&s, sizeof(ST), 0);
	printf("姓名\n");
	scanf("%s", s.name);
	printf("年龄\n");
	scanf("%d", &s.age);
	printf("年级\n");
	scanf("%d", &s.grade);
	printf("学号\n");
	scanf("%s", s.id);
	printf("身份证号\n");
	scanf("%s", s.identiy_card);
	printf("所属学院/专业\n");
	scanf("%s", s.major);
	printf("手机号码\n");
	scanf("%s", s.phone);

		
}
void save_info(ST &s)
{
	
}
