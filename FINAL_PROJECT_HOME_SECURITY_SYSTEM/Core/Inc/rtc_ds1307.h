#ifndef RTC_DS1307_H
#define RTC_DS1307_H

/**
 * Define constrains
 */

#define RTC_DS1307_OK       	 	(0)
#define RTC_DS1307_ERR      	 	(-1)
#define RTC_DS1307_I2C_ERR  	 	(-2)

#define MAX_RETRY 			 	 	(3)
#define ADDRESS_SIZE 				(1)
#define MIN_WRITE_SIZE				(1)
#define MAX_WRITE_SIZE				(7)
#define MIN_READ_SIZE				(1)
#define MAX_READ_SIZE				(7)

#define DS1307_ADDRESS      	 	(0xD0)

#define DS1307_SECOND     	 		(0x00)
#define DS1307_MINUTE     	 		(0x01)
#define DS1307_HOUR       	 		(0x02)
#define DS1307_DAY        	 		(0x03)
#define DS1307_DATE       	 		(0x04)
#define DS1307_MONTH      	 		(0x05)
#define DS1307_YEAR            		(0x06)
#define DS1307_CONTROL         		(0x07)

/* Bits in control register */
#define RTC_DS1307_CONTROL_OUT   	(7)
#define RTC_DS1307_CONTROL_SQWE  	(4)
#define RTC_DS1307_CONTROL_RS1   	(1)
#define RTC_DS1307_CONTROL_RS0   	(0)

enum month {Jan = 1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

enum day {Mon = 1, Tue, Wed, Thu, Fri, Sat, Sun};

enum day_per_month {January = 31, February = 28, March = 31, April = 30, Mayy = 31, June = 30 , July = 31,
    August = 31, September = 30, October = 31 , November = 30, December = 31};

/**
 * Define date_time struct
 */
struct date_time_s {
    uint8_t year;       // year (0 - 99)
    uint8_t month;      // month (01 - 12)
    uint8_t date;       // date (01 - 31)
    uint8_t day;        // day (01 - 07)
    uint8_t hour;       // hour (00 - 23)
    uint8_t minute;    // minutes (00 - 59)
    uint8_t second;    // seconds (00 - 59)
};

typedef struct date_time_s date_time_t;

uint8_t bcd2Dec (uint8_t value);
uint8_t dec2Bcd (uint8_t value);
void init_struct(date_time_t* datetime);
int rtc_ds1307_init(date_time_t* datetime);
int rtc_ds1307_set_second (uint8_t value);
int rtc_ds1307_set_minute(uint8_t value);
int rtc_ds1307_set_hour(uint8_t value);
int rtc_ds1307_set_day(uint8_t value);
int rtc_ds1307_set_date(uint8_t value);
int rtc_ds1307_set_month(uint8_t value);
int rtc_ds1307_set_year(uint8_t value);
int rtc_ds1307_set_datetime (const date_time_t* datetime);
int rtc_ds1307_get_datetime (date_time_t* datetime);
int rtc_ds1307_get_second(date_time_t* datetime);
int rtc_ds1307_get_minute(date_time_t* datetime);
int rtc_ds1307_get_hour(date_time_t* datetime);
int rtc_ds1307_get_day(date_time_t* datetime);
int rtc_ds1307_get_date(date_time_t* datetime);
int rtc_ds1307_get_month(date_time_t* datetime);
int rtc_ds1307_get_year(date_time_t* datetime);
int rtc_ds1307_get_datetime (date_time_t* datetime);

#endif
