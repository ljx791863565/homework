#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

typedef struct StudentInfo {
	char name[20];
	int age;
} ST_INFO;

int main(int argc, const char *argv[])
{
	ST_INFO st[10] = {
		{"name1", 20},
		{"name2", 20},
		{"name3", 20},
		{"name4", 20},
		{"name5", 20},
		{"name6", 20},
		{"name7", 20},
		{"name8", 20},
		{"name9", 20},
		{"name10", 20}
	};

	FILE *fp = fopen("/home/ljx/working/interview/studentInfo.txt", "w+");
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}
	int i;
	int ret;
	ret = fwrite(st, sizeof(ST_INFO), 10, fp);
	if (ret < 0) {
		perror("fwrite");
		return -1;
	}
	printf("write %d info\n", ret);

	//此时文件指针指向文件结尾，将其拨到文件开头以便从开始读取文件
	fseek(fp, 0, SEEK_SET);
	ST_INFO temp;
	for (i = 0; i < 10; i++) {
		ret = fread(&temp, sizeof(ST_INFO), 1, fp);
		if (ret < 0 ) {
			perror("fread");
			return -1;
		}
		printf("%d %s %d\n", i, temp.name, temp.age);
	}

	fclose(fp);
	return 0;
}
