/*
 *	This module contains methods to handle with a rtc-ds1307
 */

#include "rtc_ds1307.h"

/*vector needed to store the value in bcd read from the rtc*/
uint8_t rtc_read_buffer[MAX_BUFFER_SIZE];

/*
 * @fn 			static uint8_t bcd2Dec(uint8_t val)
 * @brief  		Convert BCD to Decimal
 * @param		value: value to convert from BCD to Decimal
 * @retval 		converted value
 */
uint8_t bcd2Dec(uint8_t value) {
	return ((value / 16 * 10) + (value % 16));
}

/*
 * @fn 			static uint8_t dec2Bcd(uint8_t val)
 * @brief  		Convert Decimal to BCD
 * @param   	value: value to convert from Decimal to BCD
 * @retval 		converted value
 */
uint8_t dec2Bcd(uint8_t value) {
	return ((value / 10 * 16) + (value % 10));
}

/*
 * @fn 			init_struct(date_time_t* datetime)
 * @brief  		initialize the datetime struct
 * @param   	datetime: the datetime variable to initialize
 */
void init_struct(TDatetime *datetime) {
	retrieve_current_date_time(datetime);
}

/*
 * @fn         int rtc_ds1307_init(date_time_t* datetime)
 * @brief      initialization of rtc_ds1307 and the datetime parameter
 * @param      datetime: the datetime variable to initialize
 * @retval     RTC_DS1307_ERR if the device is not ready
 * @retval     RTC_DS1307_OK if the device is ready
 */
int rtc_ds1307_init(TDatetime *datetime) {

	HAL_StatusTypeDef is_ds1307_ready = HAL_I2C_IsDeviceReady(&hi2c1,
			DS1307_ADDRESS, MAX_RETRY, HAL_MAX_DELAY);

	if (is_ds1307_ready != HAL_OK)
		return RTC_DS1307_ERR;

	init_struct(datetime);

	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_datetime(const datetime_t* datetime)
 * @brief      set all the datetime attributes into the respective register of the rtc
 * @param	   datetime: the variable that contains date and time to set into the rtc
 * @retval     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @retval     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_datetime(const TDatetime *datetime) {

	HAL_StatusTypeDef return_value;
	uint8_t buffer[MAX_BUFFER_SIZE];

	buffer[0] = dec2Bcd(datetime->second);
	buffer[1] = dec2Bcd(datetime->minute);
	buffer[2] = dec2Bcd(datetime->hour);
	buffer[3] = dec2Bcd(datetime->day);
	buffer[4] = dec2Bcd(datetime->date);
	buffer[5] = dec2Bcd(datetime->month);
	buffer[6] = dec2Bcd(datetime->year);

	return_value = HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDRESS, DS1307_SECOND_REGISTER,
	ADDRESS_SIZE, (uint8_t *)&buffer, MAX_BUFFER_SIZE, HAL_MAX_DELAY);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	return RTC_DS1307_OK;
}

/*
 * @fn 		   uint8_t rtc_ds1307_get_datetime()
 * @brief  	   get the datetime from the registers of the rtc
 * @retval     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @retval     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_datetime() {
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_SECOND_REGISTER,
	ADDRESS_SIZE, (uint8_t *)&rtc_read_buffer, MAX_BUFFER_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	return RTC_DS1307_OK;
}
