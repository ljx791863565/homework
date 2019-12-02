#include <stdio.h>

void resetTime(int *y, int *m, int *d, int *h, int *min, int *sec)
{
//	if (*y < 0 || *m <1 || *m > 12 || *d < 1 || *d >31 || *h < 0 || *h > 24 || *min < 0 || *min > 60 || *sec < 0 || *sec > 60){
//		return ;
//	}
	int dayOfMonth[12] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	if (*y % 400 == 0 || *y % 100 != 0 || *y % 4 == 0){
		dayOfMonth[1] = 29;
	}
	*sec += 1;
	if (*sec >= 60){
		*sec = 0;
		*min += 1;
		if (*min >= 60){
			*min = 0;
			*h += 1;
			if (*h >= 24){
				*h = 0;
				*d += 1;
				if (*d > dayOfMonth[*m -1]){
					*d = 1;
					*m += 1;
					if (*m > 12){
						*m = 1;
						*y += 1;
					}
				}
			}
		}
	}
	return ;
}
int main()
{
	int year, month, day, hour, min, sec;
	year = 2004;
	month = 12;
	day = 31;
	hour = 23;
	min = 59;
	sec = 59;
	printf("%d年 %d月 %d日 %d时 %d分 %d秒\n", year, month, day, hour, min, sec);
	resetTime(&year, &month, &day, &hour, &min, &sec);
	printf("%d年 %d月 %d日 %d时 %d分 %d秒\n", year, month, day, hour, min, sec);

}
