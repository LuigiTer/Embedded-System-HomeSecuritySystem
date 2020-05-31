#include <stdint.h>
#include "i2c.h"
#include "rtc_ds1307.h"

/*
 * @fn 			static uint8_t bcd2Dec(uint8_t val)
 * @brief  		Convert BCD to Decimal
 * @param[in]	value: value to convert from BCD to Decimal
 * @return 		converted value
 */
uint8_t bcd2Dec(uint8_t value){
	return ((value/16*10) + (value % 16));
}

/*
 * @fn 			static uint8_t dec2Bcd(uint8_t val)
 * @brief  		Convert Decimal to BCD
 * @param[in]	value: value to convert from Decimal to BCD
 * @return 		converted value
 */
uint8_t dec2Bcd(uint8_t value){
	return ((value/10*16) + (value % 10));
}

/*
 * @fn 			init_struct(date_time_t* datetime)
 * @brief  		initialize the datetime struct
 * @param[in]	datetime: the datetime variable to initialize
 */
void init_struct(date_time_t* datetime){
	datetime->second = 0;
	datetime->minute = 0;
	datetime->hour = 0;
	datetime->day = 0;
	datetime->date = 0;
	datetime->month = 0;
	datetime->year = 0;
}

/*
 * @fn         int rtc_ds1307_init(date_time_t* datetime)
 * @brief      initialization of rtc_ds1307 and the datetime variable
 * @param[in]  datetime: the datetime variable to initialize
 * @return     RTC_DS1307_ERR if the device is not ready
 * @return     RTC_DS1307_OK if the device is ready
 */
int rtc_ds1307_init(date_time_t* datetime) {

    HAL_StatusTypeDef is_ds1307_ready = HAL_I2C_IsDeviceReady(&hi2c1, DS1307_ADDRESS, MAX_RETRY, HAL_MAX_DELAY);

    if (is_ds1307_ready != HAL_OK)
    	return RTC_DS1307_ERR;

    init_struct(datetime);

    return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_second(uint8_t value)
 * @brief      set the @param[in] in the second register of rtc_ds1307
 * @param[in]  value: it is the seconds to set into the register of rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_second(uint8_t value) {
	 HAL_StatusTypeDef return_value;
	 uint8_t buffer = dec2Bcd(value);

	 return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_SECOND, ADDRESS_SIZE, buffer, MIN_WRITE_SIZE);
	 if (return_value != HAL_OK)
		 return RTC_DS1307_I2C_ERR;
	 return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_miunte(uint8_t value)
 * @brief      set the @param[in] in the minute register of rtc_ds1307
 * @param[in]  value: it is the minutes to set into the register of rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_minute(uint8_t value) {
	HAL_StatusTypeDef return_value;
	uint8_t buffer = dec2Bcd(value);

	return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_MINUTE, ADDRESS_SIZE, buffer, MIN_WRITE_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;
    return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_hour(uint8_t value)
 * @brief      set the @param[in] in the hour register of rtc_ds1307
 * @param[in]  value: it is the hour to set into the register of rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_hour(uint8_t value) {
	HAL_StatusTypeDef return_value;
	uint8_t buffer = dec2Bcd(value);

	return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_HOUR, ADDRESS_SIZE, buffer, MIN_WRITE_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_day(uint8_t value)
 * @brief      set the @param[in] in the day register of rtc_ds1307
 * @param[in]  value: it is the day to set into the register of rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_day(uint8_t value) {
	HAL_StatusTypeDef return_value;
	uint8_t buffer = dec2Bcd(value);

	return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_DAY, ADDRESS_SIZE, buffer, MIN_WRITE_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_date(uint8_t value)
 * @brief      set the @param[in] in the date register of rtc_ds1307
 * @param[in]  value: it is the date to set into the register of rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_date(uint8_t value) {
	HAL_StatusTypeDef return_value;
	uint8_t buffer = dec2Bcd(value);

	return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_DATE, ADDRESS_SIZE, buffer, MIN_WRITE_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_month(uint8_t value)
 * @brief      set the @param[in] in the month register of rtc_ds1307
 * @param[in]  value: it is the month to set into the register of rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_month(uint8_t value) {
	HAL_StatusTypeDef return_value;
	uint8_t buffer = dec2Bcd(value);

	return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_MONTH, ADDRESS_SIZE, buffer, MIN_WRITE_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_year(uint8_t value)
 * @brief      set the @param[in] in the year register of rtc_ds1307
 * @param[in]  value: it is the year to set into the register of rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_year(uint8_t value) {
	HAL_StatusTypeDef return_value;
	uint8_t buffer = dec2Bcd(value);

	return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_YEAR, ADDRESS_SIZE, buffer, MIN_WRITE_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_set_datetime(const datetime_t* datetime)
 * @brief      set all the datetime attributes into the respective register of the rtc
 * @param[in]  datetime: the variable that contains date and time to set into the rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the setting fails
 * @return     RTC_DS1307_OK if the transmit for the setting was successful
 */
int rtc_ds1307_set_datetime (const date_time_t* datetime) {

	HAL_StatusTypeDef return_value;
	uint8_t buffer[MAX_WRITE_SIZE];

	buffer[0] = dec2Bcd(datetime->second);
	buffer[1] = dec2Bcd(datetime->minute);
	buffer[2] = dec2Bcd(datetime->hour);
	buffer[3] = dec2Bcd(datetime->day);
	buffer[4] = dec2Bcd(datetime->date);
	buffer[5] = dec2Bcd(datetime->month);
	buffer[6] = dec2Bcd(datetime->year);

	return_value = HAL_I2C_Mem_Write_DMA(&hi2c1, DS1307_ADDRESS, DS1307_SECOND, ADDRESS_SIZE, buffer, MAX_WRITE_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_get_second(datetime_t* datetime)
 * @brief      get the seconds from the register in the rtc
 * @param[in]  datetime: the variable that contains date and time and used to store the seconds retrieved from rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_second(date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_SECOND, ADDRESS_SIZE, &(datetime->second), MIN_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->second = bcd2Dec(datetime->second);
	return RTC_DS1307_OK;

}

/*
 * @fn         int rtc_ds1307_get_minute(datetime_t* datetime)
 * @brief      get the minutes from the register in the rtc
 * @param[in]  datetime: the variable that contains date and time and used to store the minutes retrieved from rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_minute(date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_MINUTE, ADDRESS_SIZE, &(datetime->minute), MIN_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->minute = bcd2Dec(datetime->minute);
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_get_hour(datetime_t* datetime)
 * @brief      get the hour from the register in the rtc
 * @param[in]  datetime: the variable that contains date and time and used to store the hour retrieved from rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_hour(date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_HOUR, ADDRESS_SIZE, &(datetime->hour), MIN_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->hour = bcd2Dec(datetime->hour);
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_get_day(datetime_t* datetime)
 * @brief      get the day from the register in the rtc
 * @param[in]  datetime: the variable that contains date and time and used to store the day retrieved from rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_day(date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_DAY, ADDRESS_SIZE, &(datetime->day), MIN_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->day = bcd2Dec(datetime->day);
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_get_date(datetime_t* datetime)
 * @brief      get the date from the register in the rtc
 * @param[in]  datetime: the variable that contains date and time and used to store the date retrieved from rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_date(date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_DATE, ADDRESS_SIZE, &(datetime->date), MIN_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->date = bcd2Dec(datetime->date);
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_get_month(const datetime_t* datetime)
 * @brief      get the month from the register in the rtc
 * @param[in]  datetime: the variable that contains date and time and used to store the month retrieved from rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_month(date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_MONTH, ADDRESS_SIZE, &(datetime->month), MIN_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->month = bcd2Dec(datetime->month);
	return RTC_DS1307_OK;
}

/*
 * @fn         int rtc_ds1307_get_year(datetime_t* datetime)
 * @brief      get the year from the register in the rtc
 * @param[in]  datetime: the variable that contains date and time and used to store the year retrieved from rtc
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_year(date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_YEAR, ADDRESS_SIZE, &(datetime->year), MIN_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->year = bcd2Dec(datetime->year);
	return RTC_DS1307_OK;
}

/*
 * @fn 			uint8_t rtc_ds1307_get_datetime(date_time_t* datetime)
 * @brief  		get the datetime from the register of the rtc
 * @param[in]	datetime: variable to store the date and time
 * @return     RTC_DS1307_I2C_ERR if the transmit for the getting fails
 * @return     RTC_DS1307_OK if the transmit for the getting was successful
 */
int rtc_ds1307_get_datetime (date_time_t* datetime){
	HAL_StatusTypeDef return_value;
	uint8_t buffer[7];

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_SECOND, ADDRESS_SIZE, &buffer, MAX_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	datetime->second = bcd2Dec(buffer[0]);
	datetime->minute = bcd2Dec(buffer[1]);
	datetime->hour = bcd2Dec(buffer[2]);
	datetime->day = bcd2Dec(buffer[3]);
	datetime->date = bcd2Dec(buffer[4]);
	datetime->month = bcd2Dec(buffer[5]);
	datetime->year = bcd2Dec(buffer[6]);

	return RTC_DS1307_OK;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
	if (hi2c->Instance == I2C1)
		return RTC_DS1307_I2C_ERR;
}

