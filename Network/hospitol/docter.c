#include "communal.h"

void doctor_loop()
{

}
void doctor_register_result(int result)
{
	if (result == 0){
		printf("doctor register OK\n");
	}else if (result == 1){
		printf("doctor register Fail\n");
	}

	return;
}
void doctor_register(int fd, char *buf)
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	//消息头后第一部分主体消息是医生信息
	memcpy(&doct, buf+sizeof(MSG_HEAD_T), sizeof(doct));

	MYSQL db;
	MYSQL *pdb;
	mysql_init(&db);

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "138797", "hospitol", 12345, NULL, 0);
	if (pdb == NULL){
		perror("mysql_real_connect");
		send_result(fd, 1, 2);
		return;
	}
	printf("mysql_real_connect OK\n");

	char sql[MAX_BUF];
	memset(sql, 0, sizeof(sql));
	mysql_query(&db, sql);

	char sqll[MAX_BUF];
	memset(sqll, 0, sizeof(sql));
	sprintf(sqll, "insert into doctor values(%s %s %s %d %s %s %s %d);",doct.ID, doct.name, doct.passwd, doct.age, doct.sex, doct.lever, doct.department, doct.room);

	int ret = mysql_query(&db, sqll);
	if (ret != 0){
		perror("mysql_query");
		printf("doct insert Fail\n");
		send_result(fd, 1, 2);
	}
	printf("doct insert OK\n");
	send_result(fd, 0, 2);

	mysql_close(&db);

	return;
}
