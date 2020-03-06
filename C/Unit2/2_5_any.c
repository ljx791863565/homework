
/*
   将字符串s1中第一次出现任意一个s2中的字符，返回在s1中的位置，如果没有，返回-1
   */
#include <stdio.h>

int any(char *s1, char *s2)
{
	int i, j;
	for (i = 0; s1[i] != '\0'; i++){
		for (j = 0; s2[j] != '\0'; j++){
			if (s1[i] == s2[j]){
				return i;
			}
		}
	}
	return -1;
}
int main()
{
	char s1[20] = "hello world";
	char s2[20] = "nw ome";
	int num = any(s1, s2);
	if (num != -1){
		printf("num = %d\n", num);
	}else{
		printf("s1 has no s2\n");
	}
	return 0;	
}
