#ifndef __STUDENT_H__
#define __STUDENT_H__
#define PWD "/home/ljx/working/student/info/student/"
typedef struct StudentInfo
{	
	char name[20];
	char sex[10];
	int age;
	char id[8];
	char college[20];
	char grade[20];
	char passwd[20];
	char email[50];
}StudentInfo;

extern void SaveStudentInfo(struct StudentInfo *s);
extern StudentInfo* LoadStudentInfo(char *id);
extern void StudentRegister();
extern void StudentLogin();
extern int isID(char *id);
#endif
