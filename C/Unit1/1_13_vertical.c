
/*
 *打印输入单词长度的水平直方图
 */



#include <stdio.h>

#define MAXHIST 15		//直方图的最大长度
#define MAXWORD 11		//单词的最大长度
#define IN 1
#define OUT 0


int main()
{
	int c, i, nc, state;
	int len;			//直方图中长条的高度
	int maxvalue;		//出现频率最多的单词长度的次数
	int overflow;
	int wl[MAXWORD];

	state = OUT;
	nc = 0;
	overflow = 0;
	for (i=0; i<MAXWORD; ++i){
		wl[i] = 0;
	}
	while ((c = getchar()) != EOF){
		if (c == ' ' || c == '\n' || c == '\t'){
			state = OUT;
			if (nc > 0){
				if (nc < MAXWORD){
					++wl[nc];
				}else{
					++overflow;
				}
			}
			nc = 0;
		}else if(state == OUT){
				state = IN;
				nc = 1;
		}else{
				++nc;
		}
	}
	maxvalue = 0;
	for (i=1; i<MAXWORD; ++i){
		if (wl[i] > maxvalue){
			maxvalue = wl[i];
		}
	}
	int j;
	for (i=MAXHIST; i>0; --i){
		for (j = 1; j < MAXWORD; ++j) {
			if ((wl[j] * MAXHIST / maxvalue) >= i) {
				printf("  *  ");
			}else {
				printf("     ");
			}	
		}
		putchar('\n');
	}
	for (i = 1; i < MAXWORD; i++) {
		printf("%4d ", i);
	}
	putchar('\n');
	for (i = 1; i < MAXWORD; i++) {
		printf("%4d ", wl[i]);
	}
	putchar('\n');
	if (overflow > 0){
		printf("There are %d worlds >= %d\n",overflow, MAXWORD);
	}
}
