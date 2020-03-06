/*
   书本第四个程序，单词计数
   单词定义为任何不包含空格、制表符、换行符的字符序列
   */

#include <stdio.h>

#define IN 1	//在单词里
#define OUT 0	//在单词外

int main()
{
	int ch, nRow, nWord, nCh, state;
	state = OUT;
	nRow = nWord = nCh = 0;	//由右至左 <==>	nRow = (nWord = (nCh = 0));
	while ((ch = getchar()) != EOF){
		++nCh;	//字符数+1
		if (ch == '\n'){
			++nRow;	//行数+1
		}
		if (ch == ' ' || ch == '\t' || ch == '\n'){
			state = OUT;	
		}else if (state == OUT){
			state = IN;	//此时状态需要改成在单词内，因为字符已经不再是空格制表符和换行
			nWord++;	//单词数+1
		}
		printf("Rows = %d Words = %d Chs = %d\n", nRow, nWord, nCh);
	}

	return 0;
}
