#include "server1.h"
#include "server2.h"
typedef struct yizhu
{
	int sickID;
	char name[20];
	char sex[20];
	int age;
	char doctor[20];
	char nures[20];
	char time[20];
	char ZD[50];
	int State;
}YIZHU_T;



void myRegister_sicker(int fd, char * buff)
{
	SICKER_T sick;
	memset(&sick, 0, sizeof(sick));

	memcpy(&sick, buff + sizeof(MSGHEAD_T), sizeof(sick));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 22);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	
	char sql[1024] = "select *from sicker";
	char sql2[1024] = {0};
	ret = mysql_query(&db,sql);
	if(ret == 0)
	{
		printf ("have data\n");
	}
	else
	{
		printf ("no data\n");
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;
	rows = mysql_num_rows(res);
	if(rows > 0)
	{
		sprintf(sql2, "insert into sicker values('NULL','%s','%s','%s',%d,'NULL','NULL','NULL','NULL');", sick.name, sick.passwd,sick.sex,sick.age,sick.doctor,sick.nures,sick.atTime,sick.CBZD);
	}
	else
	{
		sick.ID = 1001;
		sprintf(sql2, "insert into sicker values('1001','%s','%s','%s',%d,'NULL','NULL','NULL','NULL');", sick.name, sick.passwd,sick.sex,sick.age,sick.doctor,sick.nures,sick.atTime,sick.CBZD);

	}


	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("insert ok\n");
		send_result(fd, 0, 22);
	}
	else
	{
		printf("insert fail:%s\n", sql2);
		send_result(fd, 1, 22);
	}

	mysql_close(&db);

	return;

}


void myLogin_sicker(int fd, char * buff)
{
	SICKER_T sick;
	memset(&sick, 0, sizeof(sick));

	memcpy(&sick, buff + sizeof(MSGHEAD_T), sizeof(sick));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 24);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "select passwd from sicker where name ='%s';", sick.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 24);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	MYSQL_ROW result;
	rows = mysql_num_rows(res);
	if(rows > 0)
	{
		result = mysql_fetch_row(res);
		if(strcmp(result[0], sick.passwd) == 0)
		{
			send_result(fd, 0, 24);
		}
		else
		{
			send_result(fd, 2, 24);
		}
	}
	else
	{
		send_result(fd, 1, 24);//4表示登录的返回消息类型, 1表示用户名输错
	}

	mysql_close(&db);

	return;

}
void myLook_sicker(int fd, char* buf)//病人查看自己信息
{

	SICKER_T sick;
	memset(&sick, 0, sizeof(sick));

	memcpy(&sick, buf + sizeof(MSGHEAD_T), sizeof(sick));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 26);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "select * from yizhu where name = '%s';",sick.name);
//	sprintf(sql2, "select * from sicker;");

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 30);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_select_result(fd, 28, NULL);
	}

	int column;
	column = mysql_num_fields(res);
	char buff[1000] = {0};
	memset(buff,0,sizeof(buff));
	int i;
	int j;
//	printf("rows = %d\n",rows);
//	printf("column = %d\n",column);
	MYSQL_ROW result;
	for(i = 0; i < rows; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0;j < column; j++)
		{
			if(result[8] == 0)
			{
//				sprintf(buff, "%s%s", buff,'\0');
				send_select_result(fd, 28, NULL);
			}
			else
			{
				sprintf(buff, "%s%10.10s",buff, result[j]);
			}
		}
		sprintf(buff,"%s%s",buff,"\n");
	}
	send_select_result(fd, 30, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}


void mySelect_sicker(int fd, char* buf)//病人查看自己信息
{

	SICKER_T sick;
	memset(&sick, 0, sizeof(sick));

	memcpy(&sick, buf + sizeof(MSGHEAD_T), sizeof(sick));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 26);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);

	char sql2[1024] = {0};
	sprintf(sql2, "select * from sicker where name = '%s';",sick.name);
//	sprintf(sql2, "select * from sicker;");

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 28);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_select_result(fd, 28, NULL);
	}

	int column;
	column = mysql_num_fields(res);
	char buff[1000] = {0};
	memset(buff,0,sizeof(buff));
	int i;
	int j;
//	printf("rows = %d\n",rows);
//	printf("column = %d\n",column);
	MYSQL_ROW result;
	for(i = 0; i < rows; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0;j < column; j++)
		{
			sprintf(buff, "%s%10.10s",buff, result[j]);
		}
		sprintf(buff,"%s%s",buff,"\n");
	}
	send_select_result(fd, 28, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}

void myTurn_sicker(int fd,char *buff)
{
	SICKER_T sick;
	memset(&sick, 0, sizeof(sick));

	memcpy(&sick, buff + sizeof(MSGHEAD_T), sizeof(sick));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 32);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);
//	printf ("new passwd = %s\n",doct.passwd)

	char sql2[1024] = {0};
	sprintf(sql2, "update sicker set passwd = '%s' where name = '%s';",sick.passwd,sick.name);

	ret = mysql_query(&db, sql2);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 32);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 32);
	}

	mysql_close(&db);

	return;
}


void myGuahao_sicker(int fd, char *buff)//挂号
{
	SICKER_T sick;
	DOCTOR_T doct;
	NURES_T nur;
	memset(&sick, 0, sizeof(sick));
	memset(&doct, 0, sizeof(doct));
	memset(&nur, 0, sizeof(nur));

	memcpy(&sick, buff + sizeof(MSGHEAD_T), sizeof(sick));
	memcpy(&doct, buff + sizeof(MSGHEAD_T) + sizeof(sick), sizeof(doct));
	memcpy(&nur, buff + sizeof(MSGHEAD_T) + sizeof(sick) + sizeof(nur), sizeof(nur));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 32);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int ret;
	char sql1[1024] = "set names utf8;";
	mysql_query(&db, sql1);
//	printf ("new passwd = %s\n",doct.passwd)
	char sql3[1024] = {0};
	char sql4[1024] = {0};
	char sql5[1024] = {0};
	char sql6[1024] = {0};
	sprintf(sql3,"update doctor set num = num + 1 where name = '%s';",doct.name);

	ret = mysql_query(&db, sql3);
	if(ret == 0)
	{
		printf("doctor num update ok\n");
		send_result(fd,0,26);
	}
	else
	{
		printf("doctor num update fail\n");
		send_result(fd,1,26);
	}
	sprintf(sql4,"update nures set num = num + 1 where name = '%s';",nur.name);

	ret = mysql_query(&db, sql4);
	if(ret == 0)
	{
		printf("nur num update ok\n");
		send_result(fd,0,26);
	}
	else
	{
		printf("nur num update fail\n");
		send_result(fd,1,26);
	}

	sprintf(sql5,"update sicker set doctor = '%s' where name = '%s';",doct.name,sick.name);

	ret = mysql_query(&db, sql5);
	if(ret == 0)
	{
		printf("sicker update ok\n");
		send_result(fd,0,26);
	}
	else
	{
		printf("sicker update fail:%s\n",sql5);
		send_result(fd,1,26);
	}
	sprintf(sql6,"update sicker set nures = '%s' where name = '%s';",nur.name,sick.name);

	ret = mysql_query(&db, sql6);
	if(ret == 0)
	{
		printf("sicker update ok\n");
		send_result(fd,0,26);
	}
	else
	{
		printf("sicker update fail:%s\n",sql6);
		send_result(fd,1,26);
	}

	mysql_close(&db);
}

