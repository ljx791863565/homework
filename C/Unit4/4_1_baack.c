#include <stdio.h>
#include <string.h>
//返回字符串 temp 在 字符串str中最后一次出现位置的索引 如果字符串中不包含temp 返回-1 
int str_right_index(char *str, char *temp)
{
	int i, j, k;
	for (i = strlen(str) - strlen(temp); i >= 0; --i) {
		for (j = i, k = 0; temp[k] != '\0' && str[j] == temp[k];) {
			j++;
			k++;
		}
		if (k > 0 && temp[k] == '\0') {
			return i;
		}
	}
	return -1;
}


int main(int argc, const char *argv[])
{
	char str[40] = "abcdehijbcdefghijkabcd";
	char t[4] = "hij";

	int num = str_right_index(str, t);
	printf("%d\n", num);
	return 0;
}
