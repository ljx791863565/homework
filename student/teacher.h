#ifndef __TEACHER_H__
#define __TEACHER_H__
typedef struct TeacherInfo
{	
	char name[20];
	char sex[10];
	int age;
	char id[8];
	char college[20];
	char subject[20];
	char passwd[20];
	char email[50];
}TeacherInfo;

extern SaveTeacherInfo(struct TeacherInfo *s);
extern TeacherInfo* LoadTeacherInfo(char *id);
extern void TeacherRegister();
extern void TeacherLogin();
extern void TeacherOption();
#endif
