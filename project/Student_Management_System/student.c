#include "student.h"
#include "common.h"
#include "unp.h"
extern sockfd;

static login_result = 0;

void student_register_result(int result)
{
	if (result == 0) {
		printf("学生注册成功\n");
	}else {
		printf("学生注册失败\n");
	}
}

void student_login_result(int result)
{
	if (result == 0) {
		printf("学生登录成功\n");
		login_result = 1;
	}else if (result == 1){
		printf("学生登录密码错误\n");
		login_result = 2;
	}else if (result == 2){
		printf("账号不存在，请先注册\n");
		login_result = 3;
	}
}
void student()
{
	int option;
	while (1) {
		student_menu();
		scanf("%d", &option);
		if (option == 1) {
			student_register(sockfd);
		}else if (option == 2){
			//这里直接进入登录界面 后续账号密码不对将返回到登录界面
			while (1){
				student_login(sockfd);
				if (login_result == 1){
					printf("登录成功\n");
					student_after_login_menu();
				}else if (login_result == 2){
					printf("密码错误\n");
					continue;
				}else if (login_result == 3){
					printf("账号不存在,请先注册\n");
					break;
				}
			}
		}else if (option == 3){
			printf("返回上一级...\n");
			sleep(1);
			break;
		}else {
			printf("输入有误，请重新输入...\n");
		}
	}
}

void student_register(int fd)
{
	printf("this is student_register\n");
	sleep(1);
	int len;
	MSGHEAD_T msghead;
	memset(&msghead, 0, sizeof(msghead));
	STUDENT_T stu;
	memset(&stu, 0, sizeof(stu));

	printf("请填写以下信息...\n");
	printf("请输入学号\n");
	scanf("%s", stu.student_id);
	printf("请输入密码\n");
	scanf("%s", stu.passwd);
	printf("请输入姓名\n");
	scanf("%s", stu.name);
	printf("请输入年龄\n");
	scanf("%d", &stu.age);
	printf("请输入性别\n");
	scanf("%s", stu.sexual_distinction);
	printf("请输入身份证号码\n");
	scanf("%s", stu.id_card);
	printf("请输入手机号码\n");
	scanf("%s", stu.phone_number);
	printf("请输入学院\n");
	scanf("%s", stu.collage);
	printf("请输入专业\n");
	scanf("%s", stu.specialty);
	
	//封装消息报文
	//
	char buf[BUFSIZE];
	memset(buf, 0, BUFSIZE);
	len = sizeof(msghead) + sizeof(stu);
	msghead.len = htonl(msghead.len);
	msghead.msgtype = 1;
	msghead.msgtype = htonl(msghead.msgtype);

	memcpy(buf, &msghead, sizeof(msghead));
	memcpy(buf+sizeof(msghead), &stu, sizeof(stu));

	int ret = write(fd, buf, len);
	if (len <= 0) {
		perror("student register write");
		return;
	}
}

void student_login(int fd)
{
	printf("this is student_rogin\n");
	sleep(1);
	int len;
	MSGHEAD_T msghead;
	memset(&msghead, 0, sizeof(msghead));
	STUDENT_T stu;
	memset(&stu, 0, sizeof(stu));
	printf("请输入账号(学号)\n");
	scanf("%s", stu.student_id);
	printf("请输入密码\n");
	scanf("%s", stu.passwd);
	char buf[BUFSIZE];
	memset(buf, 0, BUFSIZE);
	len = sizeof(msghead) + sizeof(stu);
	msghead.len = htonl(msghead.len);
	msghead.msgtype = 3;
	msghead.msgtype = htonl(msghead.msgtype);

	memcpy(buf, &msghead, sizeof(msghead));
	memcpy(buf+sizeof(msghead), &stu, sizeof(stu));

	int ret = write(fd, buf, len);
	if (len <= 0) {
		perror("student login write");
		return;
	}
}
