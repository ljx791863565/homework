#include <stdio.h>
union endian
{	
	int i;
	char b;
};
int main()
{
	int i = 1;	//1在内存中表示为 0x00000001
	char *p = (char *)&i;	//i的地址强制转换成char*类型

	if (*p == 1){
		printf("little endian\n");	//如果第一个字节是1，为小端
	}else{
		printf("big endian\n");
	}

	union endian e;
	e.i = 0x12345678;
	if (e.b == 120) {
		printf("little endian\n");
	}else if ( e.b == 18) {
		printf("big endian\n");
	}
	printf("b = %x\n", e.b);
	return 0;
}
