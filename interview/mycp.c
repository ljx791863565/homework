#include <stdio.h>
#include <stdlib.h>

// ./a.out fromfile tofile
int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("输入参数有误\n");
		return -1;
	}
	FILE *fpFrom, *fpTo;
	if ((fpFrom = fopen(argv[1], "r")) == NULL){
		perror("fpFrom:fopen");
		return -1;
	}
	if ((fpTo = fopen(argv[2], "w")) == NULL){
		perror("fpTo:fopen");
		return -1;
	}
	int ch;

	while ((ch = fgetc(fpFrom))!= EOF){
		fputc(ch, fpTo);
	}

	fclose(fpFrom);
	fclose(fpTo);
	return 0;
}
