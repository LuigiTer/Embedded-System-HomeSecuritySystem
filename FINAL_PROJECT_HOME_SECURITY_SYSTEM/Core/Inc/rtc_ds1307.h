#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "i2c.h"
#include "main.h"
#include "datetime.h"
#include "configuration.h"
#include "bool.h"

/**
 * Define constrains
 */

#define RTC_DS1307_OK       	 	(0)
#define RTC_DS1307_ERR      	 	(-1)
#define RTC_DS1307_I2C_ERR  	 	(-2)

#define MAX_RETRY 			 	 	(3)
#define ADDRESS_SIZE 				(1)
#define MIN_BUFFER_SIZE				(1)
#define MAX_BUFFER_SIZE				(7)

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

/*
 * @fn 			static uint8_t bcd2Dec(uint8_t val)
 * @brief  		Convert BCD to Decimal
 * @param[in]	value: value to convert from BCD to Decimal
 * @return 		converted value
 */
uint8_t bcd2Dec (uint8_t value);

/*
 * @fn 			static uint8_t dec2Bcd(uint8_t val)
 * @brief  		Convert Decimal to BCD
 * @param[in]	value: value to convert from Decimal to BCD
 * @return 		converted value
 */
uint8_t dec2Bcd (uint8_t value);

/*
 * @fn 			init_struct(date_time_t* datetime)
 * @brief  		initialize the datetime struct
 * @param[in]	datetime: the datetime variable to initialize
 */
void init_struct(TDatetime* datetime);

/*
 * @fn         int rtc_ds1307_init(date_time_t* datetime)
 * @brief      initialization of rtc_ds1307 and the datetime variable
 * @param[in]  datetime: the datetime variable to initialize
 * @return     RTC_DS1307_ERR if the device is not ready
 * @return     RTC_DS1307_OK if the device is ready
 */
int rtc_ds1307_init(TDatetime* datetime);

/*
 * @fn         int rtc_ds1307_set_datetime(const datetime_t* datetime)
 * @brief      set all the datetime attributes into the respective register of the rtc
 * @param[in]  datetime: the variable that contains date and time to set into the rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_datetime (const TDatetime* datetime);

/*
 * @fn 			uint8_t rtc_ds1307_get_datetime()
 * @brief  		get the datetime from the register of the rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_datetime ();

#endif
