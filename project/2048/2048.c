#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define N 4

int a[N][N] = {0};
int empty;

void draw_one(int x, int y)
{
	int i, j, k, m;
	char ch[5] = {0x00};
	i = a[y][x];
	m = 0;
	
	while (i > 0) {
		j = i % 10;
		ch[m+1] = j + '0';
		i /= 10;
	}

	m = 0;
	k = (x + 1) * 5 - 1;
	while (ch[m] != 0x00) {
		move(x*y+1, k);
		addch(ch[m++]);
		k--;
	}
}

void draw(void)
{
	int m, n, x, y;
//	char ch[N] = {'0', '0', '0', '0'};
	clear();

	for (n = 0; n < 9; n += 2) {
		for (m = 0; m < 21; m++) {
			move(n, m);			//将光标移到m n位置
			addch('-');			//在当前位置画字符;
			refresh();			//刷新物理屏幕
		}
	}
	for (m = 0; m < 22; m += 5) {
		for (n = 1; n < 8; n++) {
			move(n, m);
			addch('|');
			refresh();
		}
	}

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			draw_one(x, y);
		}
	}
}

void init(void)
{
	int x, y;
	
	initscr();		//初始化curses库和tty
	cbreak();		//开启cbreak模式，除 DELETE 或 CTRL 等仍被视为特殊控制字元外一切输入的字元将立刻被一一读取并在输入时就被使用
	noecho();		//禁止输入的字符出现在屏幕上
	curs_set(0);	//设制光标是否可见。它的参数可以是：0（不可见），1（可见），2（完全可见）
	
	empty = 15;
	srand((unsigned)time(NULL));
	x = rand() % 4;
	y = rand() % 4;
	a[y][x] = 2;
	
	draw();
}

void play(void)
{

}
int main(int argc, const char *argv[])
{
	init();

	play();

	endwin();
	return 0;
}
