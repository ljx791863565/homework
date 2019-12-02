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

void showTheWeekDay(int week)
{
	if (week == 0){
		printf("星期日\n\n");
	}else if (week == 1){
		printf("星期一\n\n");
	}else if (week == 2){
		printf("星期二\n\n");
	}else if (week == 3){
		printf("星期三\n\n");
	}else if (week == 4){
		printf("星期四\n\n");
	}else if (week == 5){
		printf("星期五\n\n");
	}else if (week == 6){
		printf("星期六\n\n");
	}

}

void printMonthHead()
{
	printf("日\t一\t二\t三\t四\t五\t六\t\n");
	printf("==================================================\n");
}
void showMonth(int year, int month)
{
	int week = getWeekDay(year, month, 1);
	int i, j;

	printf("%d年%d月\n\n",year, month);
	printMonthHead();
	for (j = 0; j < week; j++){
		printf(" \t");
	}
	for (j = 1; j <= getDayOfMonth(year, month); j++){
		printf("%d\t", j);
		if (getWeekDay(year, month, j) == 6){
			printf("\n\n\n");
		}
	}
	printf("\n\n");

}

void showDayInMonth(int year, int month, int day)
{
	int week = getWeekDay(year, month, 1);
	int i, j;

	printf("%d年%d月%d日  ",year, month, day);
	showTheWeekDay(week);
	printMonthHead();
	for (j = 0; j < week; j++){
		printf(" \t");
	}
	for (j = 1; j <= getDayOfMonth(year, month); j++){
		if (j == day){
			printf("\33[47;31m%d\t\33[0m", day);	
		}else {
			printf("%d\t", j);
		}

		if (getWeekDay(year, month, j) == 6){
			printf("\n\n\n");
		}
	}
	printf("\n\n");

}

void printMenu()
{
	printf("\t\t\t万年历\n\n");
	printf("1.");
}

void showYear(year, month, day)
{
	int i, j, k;
	int m, n, o;
	printf("\t\t\t\t\t%d\n\n", year);		
	int week1, week2, week3;
	int count1, count2, count3;
	for (i = 1; i < 13; i+=3){
		printf("\t\t%d年%d月\t\t\t\t\t\t%d年%d月\t\t\t\t\t\t%d年%d月\n\n",year, i, year, i+1, year, i+2);
		printf("日\t一\t二\t三\t四\t五\t六\t");
		printf("日\t一\t二\t三\t四\t五\t六\t");
		printf("日\t一\t二\t三\t四\t五\t六\t\n\n");
		week1 = getWeekDay(year, i, 1);
		week2 = getWeekDay(year, i+1, 1);
		week3 = getWeekDay(year, i+2, 1);
		count1 = getDayOfMonth(year, i);
		count2 = getDayOfMonth(year, i+1);
		count3 = getDayOfMonth(year, i+2);
		for (j = 0; j < week1; j++){
			printf(" \t");
		}

//		printf("")
		
	}
}

