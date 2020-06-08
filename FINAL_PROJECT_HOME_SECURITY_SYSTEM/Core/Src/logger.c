#include "logger.h"

char *message_to_log;

extern bool periodic_call;


/**
 * @brief Prints the current date time followed by a message on the console
 * @param datetime the datetime to be used to print the date
 * @param message the message to print
 */
void write_message_with_date_time(TDatetime *datetime, const char *message) {
	periodic_call = FALSE;

	char msg[512] = { '\0' };
	sprintf(msg, (char*) "[%02u-%02u-%02u %02u:%02u:%02u] %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, message);
	periodic_call = TRUE;

	HAL_UART_Transmit(get_console(NULL)->huart, (uint8_t*) msg, 512, HAL_MAX_DELAY);
}

/**
 * @brief This function will be called by the isr of the timer and will print on the console the periodic log, ie
 * the current datetime and the sensors status
 * @param datetime the datetime to be used to print the date
 */
void show_periodic_log(TDatetime *datetime) {
	char msg[512] = { '\0' };
	char area_state[10] = { '\0' };
	char barrier_state[10] = { '\0' };

	PIR_get_string_state(&PIR_4, area_state);
	photoresistor_get_string_state(&photoresistor1, barrier_state);

	sprintf(msg, (uint8_t*) "[%02u-%02u-%02u %02u:%02u:%02u] Area %s - Barrier %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, area_state, barrier_state);
	HAL_UART_Transmit(get_console(NULL)->huart, (uint8_t*) msg, 512, HAL_MAX_DELAY);
}


/**
 * @brief ISR of the timer. This function will be called every time that a request of the current datetime is done to the rtc
 */
void show_log_general_callback() {
	TDatetime *datetime = get_configuration()->datetime;

	if (strlen(message_to_log) == 0) {
		show_periodic_log(datetime);
	} else {
		write_message_with_date_time(datetime, message_to_log);
	}
}
