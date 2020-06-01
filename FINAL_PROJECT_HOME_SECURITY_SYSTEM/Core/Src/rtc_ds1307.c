#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "i2c.h"
#include "rtc_ds1307.h"
#include "main.h"
#include "utils.h"

extern date_time_t datetime;
uint8_t temp_buffer[MAX_READ_SIZE];

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
	int date_buffer[3];
	int time_buffer[3];

	retrive_date(&date_buffer);
	retrive_time(&time_buffer);

	datetime->second = time_buffer[2];
	datetime->minute = time_buffer[1];
	datetime->hour = time_buffer[0];
	datetime->day = 1;
	datetime->date =  date_buffer[0];
	datetime->month = date_buffer[1];
	datetime->year = date_buffer[2];
}

/*
 * @fn         int rtc_ds1307_init(date_time_t* datetime)
 * @brief      initialization of rtc_ds1307 and the datetime variable
 * @param[in]  datetime: the datetime variable to initialize
 * @return     RTC_DS1307_ERR if the device is not ready
 * @return     RTC_DS1307_OK if the device is ready
 */
int rtc_ds1307_init(date_time_t* datetime) {

    HAL_StatusTypeDef is_ds1307_ready = HAL_I2C_IsDeviceReady(&hi2c1, DS1307_ADDRESS,
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

	return_value = HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDRESS, DS1307_SECOND,
			ADDRESS_SIZE, &(buffer), MAX_WRITE_SIZE, HAL_MAX_DELAY);
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
int rtc_ds1307_get_datetime (date_time_t* datetime){
	HAL_StatusTypeDef return_value;

	return_value = HAL_I2C_Mem_Read_DMA(&hi2c1, DS1307_ADDRESS, DS1307_SECOND,
			ADDRESS_SIZE, &(temp_buffer), MAX_READ_SIZE);
	if (return_value != HAL_OK)
		return RTC_DS1307_I2C_ERR;

	return RTC_DS1307_OK;
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if (hi2c->Instance == I2C1)
	{
		datetime.second = bcd2Dec(temp_buffer[0]);
		datetime.minute = bcd2Dec(temp_buffer[1]);
		datetime.hour = bcd2Dec(temp_buffer[2]);
		datetime.day = bcd2Dec(temp_buffer[3]);
		datetime.date = bcd2Dec(temp_buffer[4]);
		datetime.month = bcd2Dec(temp_buffer[5]);
		datetime.year = bcd2Dec(temp_buffer[6]);
	}
}
