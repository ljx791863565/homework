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




void myDelete_root_sicker(int fd, char* buf)//删除病人
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
		send_result(fd, 1, 308);
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
	sprintf(sql2, "delete from sicker where name = '%s';",sick.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("delete is ok:%s\n",sql2);
		send_result(fd, 0, 308);
	}
	else
	{
		printf("delete fail:%s\n", sql2);
		send_result(fd, 1, 308);
	}

	mysql_close(&db);
	return;

}


void mySelect_root_sicker(int fd)//查看病人基本信息
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
		send_result(fd, 1, 304);
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
	sprintf(sql2, "select * from sicker;");

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 304);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 304);
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
		for(j = 0;j < 4; j++)
		{
			sprintf(buff, "%s%10.10s",buff, result[j]);
		}
		sprintf(buff,"%s%s",buff,"\n");
	}
	send_select_result(fd, 304, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}

void Select_root_sicker(int fd,char *buf)//查看病人详细信息
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
		send_result(fd, 1, 312);
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

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 312);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 312);
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
	send_select_result(fd, 312, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void Select_yizhu_root_sicker(int fd,char *buf)//查看病人医嘱详细信息
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
		send_result(fd, 1, 314);
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
	sprintf(sql2, "select * from yizhu where name = '%s';",yz.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 314);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 314);
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
	send_select_result(fd, 314, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}


void mypasswd_root_sicker(int fd,char *buff)//病人密码初始化
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
		send_result(fd, 1, 308);
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
	sprintf(sql2, "update sicker set passwd = '123456' where name = '%s';",sick.name);

	ret = mysql_query(&db, sql2);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update passwd ok\n");
		send_result(fd, 0, 310);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 310);
	}

	mysql_close(&db);

	return;
}
