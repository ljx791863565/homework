#ifndef __STUDENT_H__
#define __STUDENT_H__

typedef struct Student
{
	/* 1109030218
	 * 11 2011级
	 * 09 学院代码
	 * 03 专业代码
	 * 02 班级代码
	 * 18 学号
	 */
	char student_id[12];
	char passwd[20];
	char name[20];
	int age;
	char sexual_distinction[8];
	char id_card[20];
	char phone_number[12];
	char collage[40];
	char specialty[40];
}STUDENT_T;


extern void student(void);
extern void student_register(int fd);
extern void student_login(int fd);
extern void student_register(int result);
extern void student_login(int result);
#endif	//__STUDENT_H__
