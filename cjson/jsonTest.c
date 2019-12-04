#include <stdio.h>
#include "cJSON.h"
#include <string.h>

int main()
{
	FILE *fp = fopen("jsoninfo", "r");
	if (fp == NULL){
		printf("fopen error\n");
		return -1;
	}
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int ret = fread(buf, sizeof(buf), 1, fp);
	if (ret < 0){
		printf("fread error\n");
		return -1;
	}
	printf("%s\n", buf);

	cJSON *json = cJSON_Parse(buf);
	if (json == NULL){
		printf("cJSON_Parse error\n");
		return -1;
	}
	cJSON *json_cmd1 = cJSON_GetObjectItem(json, "cmd1");
	cJSON *json_com = cJSON_GetObjectItem(json_cmd1, "com");
	if (json_com->type == cJSON_String)
		printf("com : %s\n", json_com->valuestring);

	cJSON *json_op = cJSON_GetObjectItem(json_cmd1, "op");
	if (json_op->type = cJSON_String)
		printf("op : %s\n", json_op->valuestring);

	cJSON_Delete(json);
	
	char *p = "status";
	cJSON *jsonroot = 0;
	cJSON *jsonout = 0;
	jsonroot = cJSON_CreateObject();
	cJSON *jsoncmd1 = 0;
	jsoncmd1 = cJSON_CreateObject();
	cJSON_AddStringToObject(jsoncmd1, "com", "ifname/lte");
	cJSON_AddStringToObject(jsoncmd1, "op", p);
	cJSON_AddItemToObject(jsonroot, "cmd1", jsoncmd1);
	jsonout = cJSON_Print(jsonroot);
	printf("%s", jsonout);
	cJSON_Delete(jsonroot);
	fclose(fp);

}
