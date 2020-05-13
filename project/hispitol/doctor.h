#ifndef _DOCTOR_H_
#define _DOCTOR_H_

typedef struct Doctor{
	char name[20];				//姓名
	unsigned int age;			//年龄
	char gender[4];				//性别
	char office[10];			//科室
	char title[10];				//职称

	char AttendingProject[20];	//主治项目
}DOCT;


#endif
