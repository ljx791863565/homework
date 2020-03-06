#include <stdio.h>
#include "lunardata.h"

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE 
#define TRUE 1
#endif

char *DataName[30] = {
	"初一", "初二","初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十", 
	"十一", "十二","十三", "十四", "十五", "十六", "十七", "十八", "十九", "廿十", 
	"廿一", "廿二","廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十" 
};

char *MonthName[12] = {
	"正月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "冬月", "腊月"
};

char *TianGan[10] = {
	"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"
};

char *DiZhi[12] = {
	"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戍", "亥"
};

char *ShengXiao[12] = {
	"鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"
};

char *JieQi[24] = {
	"立春", "雨水", "惊蛰", "春风", "谷雨", "清明", "立夏", "小满", "芒种", "夏至", "小暑", "大暑",
	"立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至", "小寒", "大寒"
};

char *Week[7] = {
	"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"
};

char *SanJiu[9] = {
	"一九", "二九", "三九", "四九", "五九", "六九", "七九", "八九", "九九"
};

char *MeiYu[2] = {
	"入梅", "出梅"
};

char *SanFu[3] = {
	"初伏", "中伏", "末伏"
};

unsigned short DayOrdinal[13] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 204, 334, 365
};

unsigned short DayOrdinalLeap[13] = {
	0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 205, 335, 366
};

//菜单
void ShowMenu()
{

}

//公历转农历
void GregorianToLunar()
{

}

//农历转公历
void LunarToGregorian()
{

}

//显示每个月日期
void ShowMonthCalendar()
{

}

//查询节气信息
void SeleteJieQi()
{

}

//获得农历月份信息
//参数 农历年份
BOOL GetMonthInfo(int Year, unsigned int *Data)
{

}

//判断年月日是否合法
BOOL DateCheck(int Year, int Month, int Day)
{
	if (Year < START_YEAR || year >= END_YEAR || Month > 12 || Month < 1 || Day > 31 || Day < 0) {
		return FALSE;
	}
	if (Month == 4 || Month == 6 || Month == 9 || Month == 11) {
		if (Day == 31) {
			return FALSE;
		}
	}else if (Month == 2) {
		if (IsLeapYear(Year) == 0) {
			if (Day > 28) {
				return FALSE;
			}
		}else if (IsLeapYear(Year) == 1){
			if (Day > 29) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

//判断是否闰年 
//参数 年份
//返回值 TRUE：是闰年 FALSE：不是闰年
BOOL IsLeapYear(int year)
{
	if (year <= 1600 || year > 7000) {
		return -1;
	}
	if (Year % 4 == 0 && (Year % 400 == 0 || Year % 100 != 0)) {
		return TRUE;
	} else{
		return FALSE;
	}
}

//计算日期在年内的序数
//参数 年 月 日 结果保存年内序数参数
//返回值 FALSE： 失败 TRUE： 成功
BOOL GetDayOrdinal(int year, int month, int day, int *ordinal)
{
	int ret = IsLeapYear(Year);
	if (DateCheck(year, Month, Day) == 0) {
		return FALSE;
	}
	if (ret == -1) {
		return FALSE;
	}
	if (ret == 1) {
		*ordinal = DayOrdinalLeap[Month-1] +Day -1;
	}else {
		*ordinal = DayOrdinal[Month-1] + Day - 1;
	}
	return TRUE;
}

//由年内序数计算在该年几月几号
//参数 年  年内序数 结果保存在month和day中
//返回值 FALSE: 失败 TRUE：成功
BOOL GetFromOrdinal(int year, int ordinal, int *month, int *day)
{
	int ret = 0, i = 0;
	//在上一年
	if (ordinal < 0) {
		return FALSE;
	}
	ret = IsLeapYear(year);
	if (ret == 1) {
		if (ordinal > 366) {
			return FALSE;
		}
	}else {
		if (ordinal > 365) {
			return FALSE;
		}
	}
	if (ret == -1) {
		*month = 0;
		*day = 0;
		return FALSE;
	}
	for (i = 0; i < 12; i++) {
		if (ret == 1) {
			if (ordinal >= DayOrdinalLeap[i] && ordinal < DayOrdinalLeap[i+1]) {
				*month = i+1;
				*day = ordinal - DayOrdinalLeap[i] +1;
				break;
			}
		}else {
			if (ordinal >= DayOrdinal[i] && ordinal < DayOrdinal[i+1]) {
				*month = i+1;
				*day = ordinal - DayOrdinal[i] + 1;
				break;
			}
		}
	}
	return TRUE;
}

//获得农历新年当天的公历年内序数
//参数 农历新年
//返回值 年内序数
int LunarGetNewYearOrdinal(int LunarYear)
{
	unsigned int Data = 0;
	if (GetMonthInfo(LunarYear, &Data) == FALSE) {
		return FALSE;
	}
	return ((Data >> 17)&0x3F);
}

//获取农历月天数 
//参数 农历年份 农历月份 是否闰月
//返回值 该农历月的天数 不是闰月返回0 
int LunarGetDayOfMonth(int LunarYear, int LunarMonth, BOOL LeapMonth)
{
	int i = 0;
	unsigned int BS_Data;	//这一年农历大小月
	unsigned int Data;
	int Acc_leapMonth = 0;
	if (LunarYear == START_YEAR - 1) {
		if (cPreYearIndex == -1) {
			if (LeapMonth == TRUE) {
				return FALSE;
			}
			return cPreMonth[LunarMonth - 9]
		}
	}
}

//展开农历每一年之前一年11月到该年12月每月天数
//参数  农历年份  结果保存在数组中
//返回值 成功：TRUE 失败：FALSE
BOOL LunarExpandBigOrSmallMonth(int LunarYear, DayOfMonth[15])
{

}

//获取某个农历年的闰月情况
//参数 农历年
//返回值 闰月的月份  0表示该年无闰月
int LunarGetLeapMonth(int LunarYear)
{
	
}

//公历年月日转换为农历年月日
//参数 公历年 月 日 农历年 月 日 该公历年是否有农历闰月
BOOL Gregorian2Lunar(int year, int month, int day, int *LunarYEar, *int LunarMonth, int *LunarDay, BOOL LeapMonth)
{

}

//农历年月日转换为公历年月日
//参数 农历年 月 日 公历年 月 日 该农历年是否有闰月
BOOL Lunar2Gregorian(int LunarYear, int LunarMonth, int LunarDay, int *Year, int *Month, int *Day, BOOL LeapMonth)
{

}

//获得指定年份的节气信息
//参数 公历年月 结果保存在数组中
BOOL GetJieQi(int year, int Month, int JieQi[2])
{

}

//输入年月日返回当天星期几
//失败返回-1 表示参数有误或超出范围
int GetDayOfWeek(int Year, int Month, int Day)
{

}

//输入公历年月返回该月天数
//失败返回-1 表示输入数据有误
int GetDayOfMonth(int Year, int Month)
{

}

//公历节日和节气信息显示
int G_ShowHoliday(int Year, int Month, int Day)
{

}

//农历节日和其他信息显示
//是否农历闰月 0：不是 1-12：是
BOOL L_ShowHoliday(int LunarYear, int LunarMonth, int LunarDay, int LeapMonth)
{

}

//显示一年的数九 三伏信息
//参数 公历年份 结果保存在后三个参数中
BOOL GetExtermeSeason(int Year, int *YiJiu, int *ChuFu, int *MoFu)
{

}

//显示一年的入梅和出梅信息
//参数 公历年份 结果保存在后两个参数中
BOOL GetMeiYu(int Year, int *RuMeiOrd, int *ChuMeiOrd)
{

}


int main(int argc, const char *argv[])
{
	int choise = 0;

	while (1){
		ShowMenu();
		scanf("%d", &choise);
		printf("\n\n");
		switch (choise)
		{
			case 1:
				ShowMonthCalendar();
				break;
			case 2:
				GregorianToLunar();
				break;
			case 3:
				LunarToGregorian();
				break;
			case 4:
				SeleteJieQi();
				break;
		}
	}
	return 0;
}
