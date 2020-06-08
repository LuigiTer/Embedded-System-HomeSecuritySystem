/*
 * This module contains methods to handle with date and time data.
 */

#include "datetime.h"

/*
 * @fn		int days_of_month(uint8_t month);
 * @brief	Determines the number of days for a month.
 * @param	month	month number from 1 to 12 (e.g. 1 for January, 12 for December)
 * @retval	number of days in month (e.g. days_of_month(11)=30, days_of_month(12)=31)
 */
int days_of_month(uint8_t month) {
	static uint8_t m[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return m[month];
}

/*
 * @fn		int get_month(char *month)
 * @brief	Determines the month number from 1 to 12 (e.g. 1 for January, 12 for December)
 * @param	month	pointer to the string with the name of month (e.g. Jan or Dec)
 * @retval 	month number (e.g. get_month("Jan")=1, get_month("Dec")=12)
 */
int get_month(char *month) {
	if (strcmp(month, "Jan") == 0)
		return 1;
	else if (strcmp(month, "Feb") == 0)
		return 2;
	else if (strcmp(month, "Mar") == 0)
		return 3;
	else if (strcmp(month, "Apr") == 0)
		return 4;
	else if (strcmp(month, "May") == 0)
		return 5;
	else if (strcmp(month, "Jun") == 0)
		return 6;
	else if (strcmp(month, "Jul") == 0)
		return 7;
	else if (strcmp(month, "Aug") == 0)
		return 8;
	else if (strcmp(month, "Sep") == 0)
		return 9;
	else if (strcmp(month, "Oct") == 0)
		return 10;
	else if (strcmp(month, "Nov") == 0)
		return 11;
	else if (strcmp(month, "Dec") == 0)
		return 12;
	return 0;
}

/*
 * @fn		void retrieve_time(int* time_buffer)
 * @brief	Reads the current time from the macro __TIME__ and splits it in hour, minute and second
 * @param	timeBuffer	pointer to the vector that will store the current time
 * 						time_buffer[0] will store the current hour
 * 						time_buffer[1] will store the current minute
 * 						time_buffer[2] will store the current second
 */
void retrieve_time(int *time_buffer) {
	char str[] = __TIME__;
	char delim[] = ":";

	char *hour = strtok(str, delim);
	char *min = strtok(NULL, delim);
	char *sec = strtok(NULL, delim);

	time_buffer[0] = atoi(hour);
	time_buffer[1] = atoi(min);
	time_buffer[2] = atoi(sec);
}

/*
 * @fn		void retrieve_date(int* date_buffer)
 * @brief	Reads the current date from the macro __DATE__ and splits it in day, month and year.
 * 			Note that the year is represented in two digits (e.g. 12 for both 1912 and 2012)
 * @param	dateBuffer	pointer to the vector that will store the current date
 * 						date_buffer[0] will store the current day
 * 						date_buffer[1] will store the current month
 * 						date_buffer[2] will store the current year
 */
void retrieve_date(int *date_buffer) {
	char str[] = __DATE__;
	char delim[] = " ";

	char *month = strtok(str, delim);
	char *day = strtok(NULL, delim);
	char *year = strtok(NULL, delim);

	date_buffer[0] = atoi(day);
	date_buffer[1] = get_month(month);
	date_buffer[2] = atoi(year) % 100;
}

/*
 * @fn		void retrieve_current_date_time(TDatetime *datetime)
 * @brief	Reads the current date and time from the macros __DATE__ and __TIME__
 * 			and stores it in a TDatetime structure
 * @param	datetime	pointer to the TDatetime structure that will store the current date and time
 */
void retrieve_current_date_time(TDatetime *datetime) {
	int current_date[3] = { 0 };
	retrieve_date(current_date);
	int current_time[3] = { 0 };
	retrieve_time(current_time);
	datetime->year = current_date[2];
	datetime->month = current_date[1];
	datetime->date = current_date[0];
	datetime->day = 0;
	datetime->hour = current_time[0];
	datetime->minute = current_time[1];
	datetime->second = current_time[2];
}
