#include "server2.h"
#include "server1.h"
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



void myRegister_nures(int fd, char * buff)
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
		send_result(fd, 1, 52);
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

	sprintf(sql2, "insert into nures(ID,name,passwd,class,sex,grade,num,YZGS) values('%s','%s','%s','%d','%s','%s','NULL','NULL');", nur.ID,nur.name, nur.passwd,nur.class,nur.sex,nur.grade);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("insert ok\n");
		send_result(fd, 0, 52);
	}
	else
	{
		printf("insert fail:%s\n", sql2);
		send_result(fd, 1, 52);
	}

	mysql_close(&db);

	return;

}

void myDO_yizhu_nures(int fd, char * buff)
{
	YIZHU_T yz;
	NURES_T nur;
	memset(&nur, 0, sizeof(nur));
	memset(&yz, 0, sizeof(yz));

	memcpy(&yz, buff + sizeof(MSGHEAD_T), sizeof(yz));

	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 64);
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
	char sql3[1024] = {0};

	sprintf(sql2, "update yizhu set State = '1' where sickerID = '%d';", yz.sickID);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("do yizhu ok\n");
		send_result(fd, 0, 64);
	}
	else
	{
		printf("do yizhu fail:%s\n", sql2);
		send_result(fd, 1, 64);
	}
	sprintf(sql3, "update nures set YZGS = YZGS - 1 where name = '%s';",yz.nures);

	ret = mysql_query(&db, sql3);
	if(ret == 0)
	{
		printf("do yizhu ok\n");
		send_result(fd, 0, 64);
	}
	else
	{
		printf("do yizhu fail:%s\n", sql3);
		send_result(fd, 1, 64);
	}
	mysql_close(&db);

	return;

}

void myLogin_nures(int fd, char * buff)
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
	sprintf(sql2, "select passwd from nures where ID ='%s';", nur.ID);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 54);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	MYSQL_ROW result;
	rows = mysql_num_rows(res);
	if(rows > 0)
	{
		result = mysql_fetch_row(res);
		if(strcmp(result[0], nur.passwd) == 0)
		{
			send_result(fd, 0, 54);
		}
		else
		{
			send_result(fd, 2, 54);
		}
	}
	else
	{
		send_result(fd, 1, 54);//4表示登录的返回消息类型, 1表示用户名输错
	}

	mysql_close(&db);

	return;

}

void mySelect_nures(int fd, char* buf)//护士查看自己信息
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
		send_result(fd, 1, 56);
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
	sprintf(sql2, "select * from nures where ID = '%s';",nur.ID);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 56);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 56);
	}

	int column;
	column = mysql_num_fields(res);
	char buff[1000] = {0};
	memset(buff,0,sizeof(buff));
	int i;
	int j;
	MYSQL_ROW result;
	for(j = 0; j < rows; j++)
	{
		result = mysql_fetch_row(res);
		for(i = 0;i < column; i++)
		{
			sprintf(buff, "%s%10.10s",buff, result[i]);
		}
		sprintf(buff,"%s%s",buff,"\n");
	}
	send_select_result(fd, 56, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;
}

void mySelect_all_nures(int fd,char *buf)//护士查看所有病人信息
{
	SICKER_T sick;
	memset(&sick,0,sizeof(sick));

	memcpy(&sick,buf + sizeof(MSGHEAD_T), sizeof(sick));
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 60);
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
	sprintf(sql2, "select * from sicker where nures = '%s';",sick.nures);
	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 60);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd, 1, 60);
	}

	int column;
	column = mysql_num_fields(res);
	char buff[1000] = {0};
	memset(buff,0,sizeof(buff));
	int i;
	int j;
	char bu[100] = {0};
	memset(bu,0,sizeof(bu));
	sprintf(bu,"\t病人总数是 %d",rows);
//	printf("column = %d\n",column);
	MYSQL_ROW result;
	for(j = 0; j < rows; j++)
	{
		result = mysql_fetch_row(res);
		for(i = 0;i < column; i++)
		{
			sprintf(buff, "%s%10.10s",buff, result[i]);
		}
		sprintf(buff,"%s%s",buff,"\n");
	}
	sprintf(buff,"%s%s",buff,bu);
	send_select_result(fd, 60, buff);
	

	mysql_free_result(res);
	mysql_close(&db);
	return;

}

void mySelect_yizhu_nures(int fd,char *buf)//护士查看所有未执行医嘱
{
	YIZHU_T yz;
	memset(&yz,0,sizeof(yz));

	memcpy(&yz,buf + sizeof(MSGHEAD_T), sizeof(yz));
	MYSQL db;
	MYSQL *pdb;

	//1.init
	mysql_init(&db);

	//2.connect 

	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "class305", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect fail\n");
		send_result(fd, 1, 62);
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
	sprintf(sql2, "select * from yizhu where nures = '%s'and State = '0';",yz.nures);

	ret = mysql_query(&db, sql2);
	if(ret == 0)
	{
		printf("select ok:%s\n",sql2);
	}
	else
	{
		printf("select fail:%s\n", sql2);
		send_result(fd, 1, 62);
	}

	MYSQL_RES *res;
	res = mysql_store_result(&db);
	int rows;

	rows = mysql_num_rows(res);
	if(rows == 0)
	{
		send_result(fd,2, 62);
	}

	char bu[100] = {0};
	memset(bu,0,sizeof(bu));
	sprintf(bu,"\t工作量总数是 %d个病人",rows);
	int column;
	column = mysql_num_fields(res);
	char buff[1000] = {0};
	int i;
	int j;
//	printf("rows = %d\n",rows);
//	printf("column = %d\n",column);
	MYSQL_ROW result;
	for(j = 0; j < rows; j++)
	{
		result = mysql_fetch_row(res);
		for(i = 0;i < column; i++)
		{
			sprintf(buff, "%s%10.10s",buff, result[i]);
		}
		sprintf(buff,"%s%s",buff,"\n");
	}
	sprintf(buff,"%s%s",buff,bu);
	send_select_result(fd, 62, buff);

	mysql_free_result(res);
	mysql_close(&db);
	return;

}

void myTurn_nures(int fd,char *buff)
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
		send_result(fd, 1, 58);
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
	sprintf(sql2, "update nures set passwd = '%s' where name = '%s';",nur.passwd,nur.name);

	ret = mysql_query(&db, sql2);
//	printf ("ret = %d\n",ret);
	if(ret == 0)
	{
		printf("update ok\n");
		send_result(fd, 0, 58);
	}
	else
	{
		printf("update fail%s\n", sql2);
		send_result(fd, 1, 58);
	}

	mysql_close(&db);

	return;
}
