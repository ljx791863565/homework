#include <stdio.h>

/*删除字符串s中的字符ch*/
void squeeze(char *s, char ch)
{
	int i, j;
	for (i = j = 0; s[i] != '\0'; i++){
		if (s[i] != ch)
			s[j++] = s[i];
		s[j] = '\0';	//最后补上'\0'
	}
}

//将s1中任何和s2匹配的字符串都删除
void squeeze2(char *s1, char *s2)
{
	int i, j, k;
	for (i = k =0; s1[i] != '\0'; ++i){
		for (j = 0; s2[j] != '\0' && s1[i] != s2[j]; ++j){
			;
		}
		if (s2[j] == '\0'){
			s1[k++] = s1[i];
		}
	}
	s1[k] = '\0';
}

int main()
{
	char s1[20] = "hello world";
	char s2[20] = "hld";

	squeeze2(s1, s2);
	printf("after: s1 = %s\n", s1);

	return 0;
}
