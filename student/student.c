#include "student.h"
#include "common.h"
void SaveStudentInfo(StudentInfo *sInfo)
{
	char infoaddr[30];
	strcpy(infoaddr, PWD);
	strcat(infoaddr, sInfo->id);
	FILE *fp = fopen(infoaddr, "w+");
	if (fp == NULL){
		perror("fopen");
		return;
	}
	int ret = fwrite(sInfo, sizeof(struct StudentInfo), 1, fp);
	if (ret < 0){
		perror("fwrite");
		return;
	}
}

struct StudentInfo* LoadStudentInfo(char *id)
{
	int ret = isID(id);
	if (ret == 2){
		printf("学号输入有误\n");
		return;
	}
	char infoaddr[30];
	strcpy(infoaddr, PWD);
	strcat(infoaddr, id);

	FILE *fp = fopen(infoaddr, "r");
	if (fp == NULL){
		printf("请先注册\n");
		return ;
	}
	
}

int isID(char *id)
{
	char ch = id[0];
	if (flag == 1){
		if (ch == '1'){
			return 1;		//yes
		}else {
			return 2;
		}
	}else if (flag == 2){
		if (ch == '2'){
			return 3;		//yes
		}else{
			return 4;
		}	
	}
}
void StudentRegister()
{
	printf("student\n");
	struct StudentInfo sInfo;
	memset(&sInfo, 0, sizeof(sInfo));	
	printf("请输入姓名\n");
	scanf("%s", sInfo.name);
	printf("请输入性别\n");
	scanf("%s", sInfo.sex);
	printf("请输入年龄\n");
	scanf("%d", &sInfo.age);
	printf("请输入学号\n");
	scanf("%s", sInfo.id);
	printf("请输入所在学院\n");
	scanf("%s", sInfo.college);
	printf("请输入年级\n");
	scanf("%s", sInfo.grade);
	printf("请输入密码\n");
	scanf("%s", sInfo.passwd);
	printf("请输入电子邮箱\n");
	scanf("%s", sInfo.email);
	SaveStudentInfo(&sInfo);
	printf("%s 同学， 注册成功，请登录\n", sInfo.id);
}

