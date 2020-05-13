#include <stdio.h>

//返回字符串 temp 在 字符串str中最后一次出现位置的索引 如果字符串中不包含temp 返回-1 
int str_right_index(char *str, char *temp)
{
	int i, j, k;
	int pos = -1;
	for (i = 0; str[i] != '\0'; i++) {
		for (j = i, k = 0; temp[k] != '\0' && str[j] == temp[k];) {
			j++;
			k++;
		}
		if (k > 0 && temp[k] == '\0') {
			pos = i;		//此时temp遍历完 i所在位置为temp字符串在str字符串最后出现的位置 如果出现多处temp字符串 pos用于更新i
		}
	}
	return pos;
}


int main(int argc, const char *argv[])
{
	char str[40] = "abcdehijbcdefghijkabcd";
	char t[4] = "hij";

	int num = str_right_index(str, t);
	printf("%d\n", num);
	return 0;
}
