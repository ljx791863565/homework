#include "Calendar.h"
unsigned int temp_total_day;
unsigned char gc_solar_calendar_year;
unsigned char gc_solar_calendar_month;
unsigned char gc_solar_calendar_date;
unsigned char gc_lunar_calendar_year;
unsigned char gc_lunar_calendar_month;
unsigned char gc_lunar_calendar_date;
unsigned char start_day_of_week;

void get_solar_day_date()
{
	unsigned char temp;
	start_day_of_week = 2;
	temp_total_day = 0;
	int calculate_temp = 1;
	if (gc_solar_calendar_year > 99){
		start_day_of_week = 6;
		calculate_temp = 100;
	}
	for (temp = calculate_temp; temp < gc_solar_calendar_year; temp++){
		if (temp % 4 == 0){
			start_day_of_week += 2;
			temp_total_day += 366;
		}else {
			start_day_of_week += 1;
			temp_total_day += 365;
		}
	}
	for (temp = 1; temp < gc_solar_calendar_month; temp++){
		switch (temp)
		{
			case 1, 3, 5, 7, 8, 10, 12:
				start_day_of_week += 3;
				temp_total_day += 31;
				break;
			case 2:
				if (((gc_solar_calendar_year %4) == 0) && (gc_solar_calendar_year != 200)){
					start_day_of_week += 1;
					temp_total_day += 29;
				}else {
					start_day_of_week += 0;
					temp_total_day += 28;
				}
				break;
			case 4, 6, 9, 11:
				start_day_of_week += 2; 
				temp_total_day += 30;
				break;
		}
		start_day_of_week %= 7;
	}
}


void get_lunar_day()
{
	unsigned char temp_leap_month;
	unsigned char temp_flag;
	unsigned char calculate_temp;
	unsigned char mc_tpumenus_temp_loop;
	unsigned char mc_tpumenus_temp;
	temp_leap_month = 0;
	temp_flag = 1;
	if (gc_solar_calendar_year > 99){
		gc_lunar_calendar_year = 99;
		gc_lunar_calendar_month = 11;
		gc_lunar_calendar_date = 25;
		temp_total_day += 25;
		calculate_temp = 100;
	}else {
		gc_lunar_calendar_year = 0;
		gc_lunar_calendar_month = 11;
		gc_lunar_calendar_date = 11;
		temp_total_day += 11;
		calculate_temp = 1;
	}

	if (gc_solar_calendar_year > calculate_temp || gc_solar_calendar_month > 1){
		for (mc_tpumenus_temp_loop = 1; mc_tpumenus_temp_loop > 0;){
			temp_total_day -= calendar_calculate_lunar_month_total_day();
			temp_leap_month = tpumenus_lunar_calendar_month_table[2*gc_lunar_calendar_year + 1];
			temp_leap_month = (temp_leap_month>>4)&0x0F;
			if(gc_lunar_calendar_month == temp_leap_month)
			{switch(gc_lunar_calendar_year)
				{case 6,14,19,25,33,36,38,41,44,52,55,79,117,
				136,147,150,155,158,185,193:
				if(temp_total_day<31){gc_lunar_calendar_date = temp_total_day;
				mc_tpumenus_temp_loop = 0;temp_flag = 0;}
				else temp_total_day -= 30;
				break; //current month:temp_leap_month
					default:
				if(temp_total_day < 30)
				{gc_lunar_calendar_date = temp_total_day;mc_tpumenus_temp_loop = 0;
				temp_flag = 0; /*current month:temp_leap_month*/ }
				else temp_total_day -= 29; break;}} 
					if(temp_flag){gc_lunar_calendar_month++;
					if(gc_lunar_calendar_month == 13){gc_lunar_calendar_month = 1;
					gc_lunar_calendar_year++;}
					If(temp_total_day < 61) //if temp_total_day>60,ignore compare 
					{mc_tpumenus_temp_01 = calendar_calculate_lunar_month_total_day();
					if(temp_total_day < (mc_tpumenus_temp_01 + 1))
					{mc_tpumenus_temp_loop = 0;gc_lunar_calendar_date = temp_total_day;
					} }}
		}
	}
}

unsigned char calendar_calculate_lunar_month_total_day()
{
	unsigned char mc_tpumenus_temp_01;
	unsigned char mc_tpumenus_temp_02;
	if(gc_lunar_calendar_month < 9){
		mc_tpumenus_temp_01 = lunar_calendar_month_table[2*gc_lunar_calendar_year];
		mc_tpumenus_temp_02 = gc_lunar_calendar_month - 1;
	}else{
		mc_tpumenus_temp_01 = lunar_calendar_month_table[2*gc_lunar_calendar_year + 1];
		mc_tpumenus_temp_02 = gc_lunar_calendar_month - 9;
	}
	if((mc_tpumenus_temp_01>> mc_tpumenus_temp_02)&0x01)
		return(29);
	else 
		return(30);
}
