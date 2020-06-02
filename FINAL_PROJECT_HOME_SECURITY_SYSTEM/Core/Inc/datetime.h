/*
 * This module contains methods to handle with date and time data.
 */

#ifndef INC_DATETIME_H_
#define INC_DATETIME_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * @brief	This struct represents date and time data.
 * 			It stores year, month, date, day, hour, minute and second.
 * @param	year	year in two digits (e.g. 1820, 1920, 2020 will be represented as 20)
 * @param	month	month number from 1 to 12 (e.g. 1 for January, 12 for December)
 * @param	date	date number from 1 to 31
 * @param	day		day of the week from 1 to 7 (e.g. 1 for Monday, 7 for Sunday) TODO check this
 * @param	hour	hour from 0 to 23
 * @param	minute	minute from 0 to 59
 * @param	second	second from 0 to 59
 */
typedef struct {
	uint8_t year;       	// year (0 - 99)
	uint8_t month;      	// month (1 - 12)
	uint8_t date;      		// date (1 - 31)
	uint8_t day;        	// day (1 - 07)
	uint8_t hour;       	// hour (0 - 23)
	uint8_t minute;    		// minute (0 - 59)
	uint8_t second;    		// second (0 - 59)
} TDatetime;

/*
 * @fn		int daysOfMonth(uint8_t month);
 * @brief	Determines the number of days for a month.
 * @param	month	month number from 1 to 12 (e.g. 1 for January, 12 for December)
 * @retval	number of days in month (e.g. daysOfMonth(11)=30, daysOfMonth(12)=31)
 */
int daysOfMonth(uint8_t month);

/*
 * @fn		int getMonth(char *month)
 * @brief	Determines the month number from 1 to 12 (e.g. 1 for January, 12 for December)
 * @param	month	pointer to the string with the name of month (e.g. Jan or Dec)
 * @retval 	month number (e.g. getMonth("Jan")=1, getMonth("Dec")=12)
 */
int getMonth(char *month);

/*
 * @fn		void retriveTime(int* time_buffer)
 * @brief	Reads the current time from the macro __TIME__ and splits it in hour, minute and second
 * @param	timeBuffer	pointer to the vector that will store the current time
 * 						timeBuffer[0] will store the current hour
 * 						timeBuffer[1] will store the current minute
 * 						timeBuffer[2] will store the current second
 */
void retriveTime(int *timeBuffer);

/*
 * @fn		void retriveDate(int* dateBuffer)
 * @brief	Reads the current date from the macro __DATE__ and splits it in day, month and year.
 * 			Note that the year is represented in two digits (e.g. 12 for both 1912 and 2012)
 * @param	dateBuffer	pointer to the vector that will store the current date
 * 						dateBuffer[0] will store the current day
 * 						dateBuffer[1] will store the current month
 * 						dateBuffer[2] will store the current year
 */
void retriveDate(int *dateBuffer);

/*
 * @fn		void retriveCurrentDateTime(TDatetime *datetime)
 * @brief	Reads the current date and time from the macros __DATE__ and __TIME__
 * 			and stores it in a TDatetime structure
 * @param	datetime	pointer to the TDatetime structure that will store the current date and time
 */
void retriveCurrentDateTime(TDatetime *datetime);

#endif /* INC_DATETIME_H_ */
