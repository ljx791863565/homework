#ifndef __STUDENT_H__
#define __STUDENT_H__
#define PWD "/home/ljx/working/student/info/student/"
typedef struct StudentInfo
{	
	char id[20];
	char name[20];
	char sex[20];
	char college[20];
	char grade[20];
	char passwd[20];
	char email[40];
	int age;
}StudentInfo;

extern StudentInfo* LoadStudentInfo(char *id);
extern void Student_Login_option();
extern void SaveStudentInfo(struct StudentInfo *s);
extern void StudentRegister();
extern void StudentLogin();
extern int isID(char *id);
extern void StudentOption();
extern void display_student_info();
extern int set_student_passwd();
#endif
