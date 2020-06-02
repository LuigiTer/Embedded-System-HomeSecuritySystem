/*
 * This module contains methods to handle with date and time data.
 */

#include "datetime.h"

/*
 * @fn		int daysOfMonth(uint8_t month);
 * @brief	Determines the number of days for a month.
 * @param	month	month number from 1 to 12 (e.g. 1 for January, 12 for December)
 * @retval	number of days in month (e.g. daysOfMonth(11)=30, daysOfMonth(12)=31)
 */
int daysOfMonth(uint8_t month) {
	static uint8_t m[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return m[month];
}

/*
 * @fn		int getMonth(char *month)
 * @brief	Determines the month number from 1 to 12 (e.g. 1 for January, 12 for December)
 * @param	month	pointer to the string with the name of month (e.g. Jan or Dec)
 * @retval 	month number (e.g. getMonth("Jan")=1, getMonth("Dec")=12)
 */
int getMonth(char *month) {
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
}

/*
 * @fn		void retriveTime(int* time_buffer)
 * @brief	Reads the current time from the macro __TIME__ and splits it in hour, minute and second
 * @param	timeBuffer	pointer to the vector that will store the current time
 * 						timeBuffer[0] will store the current hour
 * 						timeBuffer[1] will store the current minute
 * 						timeBuffer[2] will store the current second
 */
void retriveTime(int *timeBuffer) {
	char str[] = __TIME__;
	char delim[] = ":";

	char *hour = strtok(str, delim);
	char *min = strtok(NULL, delim);
	char *sec = strtok(NULL, delim);

	timeBuffer[0] = atoi(hour);
	timeBuffer[1] = atoi(min);
	timeBuffer[2] = atoi(sec);
}

/*
 * @fn		void retriveDate(int* dateBuffer)
 * @brief	Reads the current date from the macro __DATE__ and splits it in day, month and year.
 * 			Note that the year is represented in two digits (e.g. 12 for both 1912 and 2012)
 * @param	dateBuffer	pointer to the vector that will store the current date
 * 						dateBuffer[0] will store the current day
 * 						dateBuffer[1] will store the current month
 * 						dateBuffer[2] will store the current year
 */
void retriveDate(int *dateBuffer) {
	char str[] = __DATE__;
	char delim[] = " ";

	char *month = strtok(str, delim);
	char *day = strtok(NULL, delim);
	char *year = strtok(NULL, delim);

	dateBuffer[0] = atoi(day);
	dateBuffer[1] = getMonth(month);
	dateBuffer[2] = atoi(year) % 100;
}

/*
 * @fn		void retriveCurrentDateTime(TDatetime *datetime)
 * @brief	Reads the current date and time from the macros __DATE__ and __TIME__
 * 			and stores it in a TDatetime structure
 * @param	datetime	pointer to the TDatetime structure that will store the current date and time
 */
void retriveCurrentDateTime(TDatetime *datetime) {
	int currentDate[3] = { 0 };
	retriveDate(currentDate);
	int currentTime[3] = { 0 };
	retriveTime(currentTime);
	datetime->year = currentDate[2];
	datetime->month = currentDate[1];
	datetime->date = currentDate[0];
	datetime->hour = currentTime[0];
	datetime->minute = currentTime[1];
	datetime->second = currentTime[2];
}
