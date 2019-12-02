#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("万年历\n");
	printf("请输入年月 \n");
	int year, month, day;
	scanf("%d %d %d", &year, &month, &day);
	if (isYear(year) < 0){
		printf("输入年错误\n");
		return -1;
	}
	if (isMonth(month) < 0){
		printf("输入月错误\n");
		return -1;
	}
	if (isDay(year, month, day) < 0){
		printf("输入日错误\n");
		return -1;
	}

	
	showMonth(year, month);
	showDayInMonth(year, month, day);
	showYear(year, month, day);
	return 0;;
}
