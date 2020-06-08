#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "console.h"
#include "pir_sensor.h"
#include "photoresistor.h"
#include "datetime.h"
#include "rtc_ds1307.h"
#include "bool.h"

typedef struct {
	UART_HandleTypeDef *huart;
	char *message;
} TLogger;

TLogger *logger_init(UART_HandleTypeDef *huart);

/**
 * @brief Prints the current date time followed by a message on the console
 * @param datetime the datetime to be used to print the date
 * @param message the message to print
 */
static void write_message_with_date_time(TDatetime *datetime, const char *message) {
	char msg[512] = { '\0' };
	sprintf(msg, (char*) "[%02u-%02u-%02u %02u:%02u:%02u] %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, message);

	print_message(msg);

	// HAL_UART_Transmit(get_console(NULL)->huart, (uint8_t*) msg, 512, HAL_MAX_DELAY);
}

/**
 * @brief This function will be called by the isr of the timer and will print on the console the periodic log, ie
 * the current datetime and the sensors status
 * @param datetime the datetime to be used to print the date
 */
static void show_periodic_log(TDatetime *datetime) {
	char msg[512] = { '\0' };
	char area_state[10] = { '\0' };
	char barrier_state[10] = { '\0' };

	PIR_get_string_state(&PIR_4, area_state);
	photoresistor_get_string_state(&photoresistor1, barrier_state);

	sprintf(msg, (uint8_t*) "[%02u-%02u-%02u %02u:%02u:%02u] Area %s - Barrier %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, area_state, barrier_state);

	print_message(msg);

	// HAL_UART_Transmit(get_console(NULL)->huart, (uint8_t*) msg, 512, HAL_MAX_DELAY);
}

void logger_callback(TLogger *logger);

void logger_print(TLogger *logger, const char *message);

#endif /* INC_LOGGER_H_ */

