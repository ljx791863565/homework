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
typedef struct root
{
	char ID[20];
	char passwd[20];
}ROOT_T;




void myLogin_root(int fd, char * buff)
{
	ROOT_T root;
	memset(&root, 0, sizeof(root));

	memcpy(&root, buff + sizeof(MSGHEAD_T), sizeof(root));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 102);
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
	sprintf(sql2, "select passwd from root where ID ='%s';", root.ID);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 102);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	MYSQL_ROW result;
	rows = mysql_num_rows(res);
	if(rows > 0)
	{
		result = mysql_fetch_row(res);
		if(strcmp(result[0], root.passwd) == 0)
		{
			send_result(fd, 0, 102);
		}
		else
		{
			send_result(fd, 2, 102);
		}
	}
	else
	{
		send_result(fd, 1, 102);//4表示登录的返回消息类型, 1表示用户名输错
	}

	mysql_close(&db);

	return;

}
void myDelete_root_doctor(int fd, char* buf)//删除医生
{

	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buf + sizeof(MSGHEAD_T), sizeof(doct));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 106);
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
	sprintf(sql2, "delete from doctor where name = '%s';",doct.name);
//	sprintf(sql2, "select * from sicker;");

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("delete is ok:%s\n",sql2);
		send_result(fd, 0, 108);
	}
	else
	{
		printf("delete fail:%s\n", sql2);
		send_result(fd, 1, 108);
	}

	mysql_close(&db);
	return;

}


void mySelect_root(int fd)//查看医生基本信息
{
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 104);
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
	sprintf(sql2, "select * from doctor;");

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 104);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 104);
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
			if(j == 2)
			{
				j = j+1;
			}
			sprintf(buff, "%s%10.10s",buff, result[j]);
		}
		sprintf(buff,"%s%s",buff,"\n");
	}
	send_select_result(fd, 104, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}

void Select_root_doctor(int fd,char *buf)//查看医生详细信息
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buf + sizeof(MSGHEAD_T), sizeof(doct));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 112);
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
	sprintf(sql2, "select * from doctor where name = '%s';",doct.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 112);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 112);
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
	send_select_result(fd, 112, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void Select_class_root_doctor(int fd,char *buf)//按科室查看医生详细信息
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buf + sizeof(MSGHEAD_T), sizeof(doct));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 116);
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
	sprintf(sql2, "select * from doctor where class = '%d';",doct.class);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 116);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 116);
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
	send_select_result(fd, 116, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void Select_yizhu_root_doctor(int fd,char *buf)//查看医生的医嘱详细信息
{
	YIZHU_T yz;
	memset(&yz, 0, sizeof(yz));

	memcpy(&yz, buf + sizeof(MSGHEAD_T), sizeof(yz));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 114);
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
	sprintf(sql2, "select * from yizhu where doctor = '%s';",yz.doctor);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 114);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 114);
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
	send_select_result(fd, 114, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void myUpdate_root(int fd,char *buff)
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buff + sizeof(MSGHEAD_T), sizeof(doct));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 106);
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
	sprintf(sql2, "update doctor set class = '%d' where name = '%s';",doct.class,doct.name);

	ret = mysql_query(&db, sql2);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 106);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 106);
	}
	char sql3[1024] = {0};
	sprintf(sql3, "update doctor set ZZXM = '%s' where name = '%s';",doct.ZZXM,doct.name);

	ret = mysql_query(&db, sql3);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 106);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 106);
	}

	mysql_close(&db);

	return;
}


void mypasswd_root_doctor(int fd,char *buff)//医生密码初始化
{
	DOCTOR_T doct;
	memset(&doct, 0, sizeof(doct));

	memcpy(&doct, buff + sizeof(MSGHEAD_T), sizeof(doct));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 108);
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
	sprintf(sql2, "update doctor set passwd = '123456' where name = '%s';",doct.name);

	ret = mysql_query(&db, sql2);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update passwd ok\n");
		send_result(fd, 0, 110);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 110);
	}

	mysql_close(&db);

	return;
}
