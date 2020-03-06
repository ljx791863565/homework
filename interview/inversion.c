#include <stdio.h>
#include <string.h>
#include <ctype.h>

//hello world my friend
//inversion...
//friend my world hello

int Inversion(char *str, char *newStr)
{
	char *p = str;
	if (str == NULL || newStr == NULL){
		printf("memory error\n");
		return -1;
	}
	int ch, prev,  flag = 0;
	char buf[20] = {0};
	while (*p != '\0'){
		prev = *p;
		p++;
		if (isalpha(*p) && !isalpha(prev)){
			
		}
	}

}
int main()
{
	char *p = "hello world my friend";
	char buf[100] = {0};
	int ret = Inversion(p, buf);
	if (ret < 0){
		return -1;
	}
	printf("%s\n", buf);

	return 0;
}
