#include <stdio.h>
#define NOTBLANK 'a'

/*
	ch 用于记录当前输入的字符的ASCII值
   */
int main()
{
	int ch;
	int lasic = NOTBLANK; //记录前一个字符的值
	while ((ch = getchar()) != EOF){
		if (ch != ' '){
			putchar(ch);
		}else if (lasic != ' '){
			putchar(ch);//当ch为空格且前一个字符不为空格时输出ch 即一个空格
		}
		lasic = ch;
	}
	return 0;
}
