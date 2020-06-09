/*
 *	This module contains methods to handle with a rtc-ds1307
 */

#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#include "i2c.h"
#include "datetime.h"

#define RTC_DS1307_OK       	 			(0)
#define RTC_DS1307_ERR      	 			(-1)
#define RTC_DS1307_I2C_ERR  	 			(-2)

#define MAX_RETRY 			 	 			(3)
#define ADDRESS_SIZE 						(1)
#define MAX_BUFFER_SIZE						(7)

#define DS1307_ADDRESS      	 			(0xD0)

#define DS1307_SECOND_REGISTER     	 		(0x00)
#define DS1307_MINUTE_REGISTER     	 		(0x01)
#define DS1307_HOUR_REGISTER       	 		(0x02)
#define DS1307_DAY_REGISTER        	 		(0x03)
#define DS1307_DATE_REGISTER       	 		(0x04)
#define DS1307_MONTH_REGISTER      	 		(0x05)
#define DS1307_YEAR_REGISTER            	(0x06)

/*
 * @fn 			static uint8_t bcd2Dec(uint8_t val)
 * @brief  		Convert BCD to Decimal
 * @param		value: value to convert from BCD to Decimal
 * @retval 		converted value
 */
uint8_t bcd2Dec (uint8_t value);

/*
 * @fn 			static uint8_t dec2Bcd(uint8_t val)
 * @brief  		Convert Decimal to BCD
 * @param		value: value to convert from Decimal to BCD
 * @retval 		converted value
 */
uint8_t dec2Bcd (uint8_t value);

/*
 * @fn 			init_struct(date_time_t* datetime)
 * @brief  		initialize the datetime struct
 * @param		datetime: the datetime variable to initialize
 */
void init_struct(TDatetime* datetime);

/*
 * @fn         int rtc_ds1307_init(date_time_t* datetime)
 * @brief      initialization of rtc_ds1307 and the datetime variable
 * @param	   datetime: the datetime variable to initialize
 * @retval     RTC_DS1307_ERR if the device is not ready
 * @retval     RTC_DS1307_OK if the device is ready
 */
int rtc_ds1307_init(TDatetime* datetime);

/*
 * @fn         int rtc_ds1307_set_datetime(const datetime_t* datetime)
 * @brief      set all the datetime attributes into the respective register of the rtc
 * @param      datetime: the variable that contains date and time to set into the rtc
 * @retval     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @retval     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_datetime (const TDatetime* datetime);

/*
 * @fn 		   uint8_t rtc_ds1307_get_datetime()
 * @brief  	   get the datetime from the registers of the rtc
 * @retval     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @retval     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_datetime ();

#endif
