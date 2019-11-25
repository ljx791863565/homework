#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("万年历\n");
	printf("请输入年月 \n");
	int year, month, day;
	scanf("%d %d %d", &year, &month, &day);

/*	
	if (isLeapYear(year) < 0){
		printf("%d not a leappyear\n", year);
	}else{
		printf("%d is a  leapyear\n", year);
	}
*/	
//	int week = getWeekDay(year, month, day);
//	printf("week = %d\n", week);
	showMonth(year, month, day);
	return 0;;
}
