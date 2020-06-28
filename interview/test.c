#include <stdio.h>

#define ROW 21
#define COL 18
static gameArea[ROW][COL];
int main(int argc, const char *argv[])
{
	printf("hello world\n");
	printf("\033[2J");
	system("stty -icanon");
	system("stty -echo");
	fprintf(stdout, "\033[?251");

	int i;
	for (i = 0; i < COL; i++){
		 gameArea[0][i] = 8;
		gameArea[0][5] = 8;
		gameArea[ROW-1][i] = 8;
	}

	for (i = 0; i < ROW; i++) {
		gameArea[i][0]	= 8;
		gameArea[i][COL-1] = 8;
	}
	for (i = 0; i < 5; i++) {
		gameArea[i][6] = 8;
	}
	return 0;
}
