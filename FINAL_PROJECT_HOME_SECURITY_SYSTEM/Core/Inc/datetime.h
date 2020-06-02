#ifndef INC_DATETIME_H_
#define INC_DATETIME_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t year;       	// year (0 - 99)
    uint8_t month;      	// month (01 - 12)
    uint8_t date;      		// date (01 - 31)
    uint8_t day;        	// day (01 - 07)
    uint8_t hour;       	// hour (00 - 23)
    uint8_t minute;    		// minutes (00 - 59)
    uint8_t second;    		// seconds (00 - 59)
} TDatetime;


int daysOfMonth(uint8_t month);

/*
 * @fn 		   int get_month(char *month)
 * @brief  	   get the month in number
 * @param[in]  *month: pointer to the string with the name of month
 * @return     integer of the month
 */
int get_month(char *month);

/*
 * @fn 		   void retrive_time(int* time_buffer)
 * @brief  	   read the time from the macro __TIME__ and split in hh mm ss
 * @param[in]  time_buffer: pointer to the vector that will store the current time
 */
void retrive_time(int* time_buffer);

/*
 * @fn 		   void retrive_date(int* date_buffer)
 * @brief  	   read the date from the macro __DATE__ and split in dd mm yy. Note if the year is 2021,
 * 			   the function return the year in the format 21.
 * @param[in]  date_buffer: pointer to the vector that will store the current date
 */
void retrive_date(int* date_buffer);


/*
 * @fn 		   	void retriveCurrentDateTime(TDatetime* datetime)
 * @brief  	   	reads the current date and time from the macros __DATE__ and __TIME__ and stores it
 * 				datetime
 * @param[in]	datetime: pointer to the datetime structure that will store the current datetime
 */
void retriveCurrentDateTime(TDatetime *datetime);


#endif /* INC_DATETIME_H_ */
