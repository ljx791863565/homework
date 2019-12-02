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




void myDelete_root_nures(int fd, char* buf)//删除护士
{

	NURES_T nur;
	memset(&nur, 0, sizeof(nur));

	memcpy(&nur, buf + sizeof(MSGHEAD_T), sizeof(nur));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 206);
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
	sprintf(sql2, "delete from nures where name = '%s';",nur.name);
//	sprintf(sql2, "select * from sicker;");

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("delete is ok:%s\n",sql2);
		send_result(fd, 0, 208);
	}
	else
	{
		printf("delete fail:%s\n", sql2);
		send_result(fd, 1, 208);
	}

	mysql_close(&db);
	return;

}


void mySelect_root_nures(int fd)//查看护士基本信息
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
		send_result(fd, 1, 204);
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
	sprintf(sql2, "select * from nures;");

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 204);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 204);
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
	send_select_result(fd, 204, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}

void Select_root_nures(int fd,char *buf)//查看护士详细信息
{
	NURES_T nur;
	memset(&nur, 0, sizeof(nur));

	memcpy(&nur, buf + sizeof(MSGHEAD_T), sizeof(nur));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 212);
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
	sprintf(sql2, "select * from nures where name = '%s';",nur.name);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 212);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 212);
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
	send_select_result(fd, 212, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void Select_grade_root_nures(int fd,char *buf)//按等级查看护士详细信息
{
	NURES_T nur;
	memset(&nur, 0, sizeof(nur));

	memcpy(&nur, buf + sizeof(MSGHEAD_T), sizeof(nur));
	
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 216);
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
	sprintf(sql2, "select * from nures where grade = '%s';",nur.grade);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 216);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 216);
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
	send_select_result(fd, 216, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void Select_yizhu_root_nures(int fd,char *buf)//查看护士的医嘱详细信息
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
		send_result(fd, 1, 214);
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
	sprintf(sql2, "select * from yizhu where nures = '%s';",yz.nures);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select is ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 214);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 214);
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
	send_select_result(fd, 214, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}
void myUpdate_root_nures(int fd,char *buff)
{
	NURES_T nur;
	memset(&nur, 0, sizeof(nur));

	memcpy(&nur, buff + sizeof(MSGHEAD_T), sizeof(nur));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 206);
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
	sprintf(sql2, "update nures set class = '%d' where name = '%s';",nur.class,nur.name);

	ret = mysql_query(&db, sql2);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 206);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 206);
	}
	char sql3[1024] = {0};
	sprintf(sql3, "update nures set grade = '%s' where name = '%s';",nur.grade,nur.name);

	ret = mysql_query(&db, sql3);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 206);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 206);
	}

	mysql_close(&db);

	return;
}


void mypasswd_root_nures(int fd,char *buff)//护士密码初始化
{
	NURES_T nur;
	memset(&nur, 0, sizeof(nur));

	memcpy(&nur, buff + sizeof(MSGHEAD_T), sizeof(nur));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 208);
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
	sprintf(sql2, "update nures set passwd = '123456' where name = '%s';",nur.name);

	ret = mysql_query(&db, sql2);
	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update passwd ok\n");
		send_result(fd, 0, 210);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 210);
	}

	mysql_close(&db);

	return;
}
