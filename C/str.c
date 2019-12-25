#include <stdio.h>

//在字符串str1中,寻找字串str2,若找到返回找到的位置,否则返回NULL
char *strstr(const char *str1,const char *str2)
{
	char *cp = (char *)strl;
	char *s1, *s2;

	if (!*str2)
		return (char*)strl;

	while (*cp){
		s1 = cp;
		s2 = (char *)str2;	

		while (*s1 && *s2 && (!*s1 - *s2)){
			s1++;
			s2++;
		}
		if (!*s2){
			return cp;
		}
		cp++;
	}
	return NULL;

}
