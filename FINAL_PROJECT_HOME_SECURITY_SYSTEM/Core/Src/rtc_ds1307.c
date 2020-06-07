#include "rtc_ds1307.h"

extern TDatetime datetime;
uint8_t temp_buffer[MAX_BUFFER_SIZE];

/*
 * @fn 			static uint8_t bcd2Dec(uint8_t val)
 * @brief  		Convert BCD to Decimal
 * @param[in]	value: value to convert from BCD to Decimal
 * @return 		converted value
 */
uint8_t bcd2Dec(uint8_t value) {
	return ((value / 16 * 10) + (value % 16));
}

/*
 * @fn 			static uint8_t dec2Bcd(uint8_t val)
 * @brief  		Convert Decimal to BCD
 * @param[in]	value: value to convert from Decimal to BCD
 * @return 		converted value
 */
uint8_t dec2Bcd(uint8_t value) {
	return ((value / 10 * 16) + (value % 10));
}

/*
 * @fn 			init_struct(date_time_t* datetime)
 * @brief  		initialize the datetime struct
 * @param[in]	datetime: the datetime variable to initialize
 */
void init_struct(TDatetime *datetime) {
	retrieve_current_date_time(datetime);
}

/*
 * @fn         int rtc_ds1307_init(date_time_t* datetime)
 * @brief      initialization of rtc_ds1307 and the datetime variable
 * @param[in]  datetime: the datetime variable to initialize
 * @return     RTC_DS1307_ERR if the device is not ready
 * @return     RTC_DS1307_OK if the device is ready
 */
int rtc_ds1307_init(TDatetime *datetime) {

	HAL_StatusTypeDef is_ds1307_ready = HAL_I2C_IsDeviceReady(&hi2c1,
			DS1307_ADDRESS,
			MAX_RETRY, HAL_MAX_DELAY);

	if (is_ds1307_ready != HAL_OK)
		return RTC_DS1307_ERR;

	init_struct(datetime);

	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_datetime(const datetime_t* datetime)
 * @brief      set all the datetime attributes into the respective register of the rtc
 * @param[in]  datetime: the variable that contains date and time to set into the rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
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

	return_value = HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDRESS, DS1307_SECOND,
	ADDRESS_SIZE, &(buffer), MAX_BUFFER_SIZE, HAL_MAX_DELAY);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	return RTC_DS1307_OK;
}

/*
 * @fn 			uint8_t rtc_ds1307_get_datetime(date_time_t* datetime)
 * @brief  		get the datetime from the register of the rtc
 * @param[in]	datetime: variable to store the date and time
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_datetime(TDatetime *datetime) {
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_SECOND,
	ADDRESS_SIZE, &(temp_buffer), MAX_BUFFER_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	return RTC_DS1307_OK;
}
