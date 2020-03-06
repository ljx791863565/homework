#include <stdio.h>

int main()
{
	int i = 1;	//1在内存中表示为 0x00000001
	char *p = (char *)&i;	//i的地址强制转换成char*类型

	if (*p == 1){
		printf("little endian\n");	//如果第一个字节是1，为小端
	}else{
		printf("big endian\n");
	}
	return 0;
}
