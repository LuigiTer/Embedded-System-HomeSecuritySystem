#include "datetime.h"


int daysOfMonth(uint8_t month) {
	static uint8_t m[] = {
			31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};
	return m[month];
}

/*
 * @fn 		   int get_month(char *month)
 * @brief  	   get the month in number
 * @param[in]  *month: pointer to the string with the name of month
 * @return     integer of the month
 */
int get_month(char *month){
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
 * @fn 		   void retrive_time(int* time_buffer)
 * @brief  	   read the time from the macro __TIME__ and split in hh mm ss
 * @param[in]  time_buffer: pointer to the vector that will store the current time
 */
void retrive_time(int* time_buffer){
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
 * @fn 		   void retrive_date(int* date_buffer)
 * @brief  	   read the date from the macro __DATE__ and split in dd mm yy. Note if the year is 2021,
 * 			   the function return the year in the format 21.
 * @param[in]  date_buffer: pointer to the vector that will store the current date
 */
void retrive_date(int* date_buffer){
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
 * @fn 		   	void retriveCurrentDateTime(TDatetime* datetime)
 * @brief  	   	reads the current date and time from the macros __DATE__ and __TIME__ and stores it
 * 				datetime
 * @param[in]	datetime: pointer to the datetime structure that will store the current datetime
 */
void retriveCurrentDateTime(TDatetime *datetime) {
	int currentDate[3] = {0};
	retrive_date(currentDate);
	int currentTime[3] = {0};
	retrive_time(currentTime);
	datetime->year = currentDate[2];
	datetime->month = currentDate[1];
	datetime->date = currentDate[0];
	datetime->hour = currentTime[0];
	datetime->minute = currentTime[1];
	datetime->second = currentTime[2];
}
