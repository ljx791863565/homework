#include "Calendar.h"

int isYear(int year)
{
	if (year < 0 || year > 9999){
		return -1;
	}
	return 1;
}

int isLeapYear(int year)
{
	if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0)){
		return 1;
	}
	return -1;
}

int isMonth(int month)
{
	if (month < 0 || month > 12){
		return -1;
	}
	return 1;
}

int getDayOfMonth(int year, int month)
{
	if (isLeapYear(year) < 0){
		return dayOfMonth[0][month -1];
	}else{
		return dayOfMonth[1][month -1];
	}
}

int isDay(int year, int month, int day)
{
	if (day < 0 || day > getDayOfMonth(year, month)){
		return -1;
	}
	return 1;
}

int getDayOfYear(int year)
{
	if (isLeapYear(year) < 0){
		return 365;
	}else {
		return 366;
	}
}

int getWeekDay(int year, int month, int day)
{
	int i = 0;
	int sumDay = 0;
	for (i=1; i < month; i++){
		sumDay += getDayOfMonth(year, i);
//		printf("sum = %d\n", sumDay);
	}
	sumDay += day;
//	printf("sumDay = %d\n", sumDay);
	int week = ((year -1) * 365)  + ((year-1)/4) - ((year-1)/100) + ((year-1)/400) + sumDay;
//	printf("week = %d\n", week);
	int weekday = (int)(week % 7);
//	printf("week %% 7 = %d\n", weekday);

	//0 sunday 1 
	return weekday;
}

void showMonth(int year, int month)
{
	int week = getWeekDay(year, month, 1);
	int i, j;

	printf("%d年%d月\n\n",year, month);
	printf("日\t一\t二\t三\t四\t五\t六\t\n");
	printf("==================================================\n");
	for (j = 0; j < week; j++){
		printf(" \t");
	}
	for (j = 1; j <= getDayOfMonth(year, month); j++){
		printf("%d\t", j);
		if (getWeekDay(year, month, j) == 6){
			printf("\n\n\n");
		}
	}
	printf("\n");

}

/*
void showYear(year, month, day)
{
	int i, j;
	for (i = 0; i < 3; i++){
		
	}
}
*/
