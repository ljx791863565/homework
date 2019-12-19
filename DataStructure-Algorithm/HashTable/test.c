/*************************************************************************
  > File Name: test.c
  > Author: ljx
  > Mail: liaojia138797@163.com 
  > Created Time: Thu 19 Dec 2019 10:19:01 AM CST
 ************************************************************************/
#include "HashTable.h"

int main()
{
	table t;
	initHashTable(&t);

	insertEntry(&t , "电脑型号" , "华硕 X550JK 笔记本电脑");
	insertEntry(&t , "操作系统" , "Windows 8.1 64位 (DirectX 11)");
	insertEntry(&t , "处理器" , "英特尔 Core i7 - 4710HQ @ 2.50GHz 四核");
	insertEntry(&t , "主板" , "华硕 X550JK(英特尔 Haswell)");
	insertEntry(&t , "内存" , "4 GB(Hynix / Hyundai)");
	insertEntry(&t , "主硬盘" , "日立 HGST HTS541010A9E680(1 TB / 5400 转 / 分)");
	insertEntry(&t , "显卡" , "NVIDIA GeForce GTX 850M(2 GB / 华硕)");
	insertEntry(&t , "显示器" , "奇美 CMN15C4(15.3 英寸)");
	insertEntry(&t , "光驱" , "松下 DVD - RAM UJ8E2 S DVD刻录机");
	insertEntry(&t , "声卡" , "Conexant SmartAudio HD @ 英特尔 Lynx Point 高保真音频");
	insertEntry(&t , "网卡" , "瑞昱 RTL8168 / 8111 / 8112 Gigabit Ethernet Controller / 华硕");
	insertEntry(&t , "主板型号" , "华硕 X550JK");
	insertEntry(&t , "芯片组" , "英特尔 Haswell");
	insertEntry(&t , "BIOS" , "X550JK.301");
	insertEntry(&t , "制造日期" , "06 / 26 / 2014");
	insertEntry(&t , "主人" , "就是我");
	insertEntry(&t , "价格" , "六十张红色毛主席");
	insertEntry(&t , "主硬盘" , "换了个120G的固态");	//在这里 key=主硬盘，需要替换掉上面的机械硬盘的信息

//	printTable(&t);
	
	printf("删除 主板型号\n");
	entry* e = removeEntry(&t , "主板型号");
	if (e != NULL) {
		puts("找到了,要释放");
		free(e->key);
		free(e->value);
		free(e);
		e = NULL;
		puts("释放了 主板型号");
	}else{
		puts("没找到 主板型号");
	}
	printTable(&t);

	const char* keys[] = {
		"显示器" , "主人","没有" , "处理器" };

	int i;
	for (i = 0; i < 4; ++i) {
		const char* value = findValueByKey(&t , keys[i]);
		if (value != NULL) {
			printf("find %s\t=\t%s\n" ,keys[i], value);
		}
		else {
			printf("not found %s\n",keys[i]);
		}
	}

	freeHashTable(&t);
	getchar();
	return 0;
	
}
