#include "logger.h"

char *message_to_log;

extern bool periodic_call;

void write_message_with_date_time(TDatetime *datetime, const char *message) {
	periodic_call = FALSE;

	/*
	rtc_ds1307_get_datetime();
	TDatetime *datetime = get_configuration()->datetime;
	*/

	char msg[512] = { '\0' };
	sprintf(msg, (char*) "[%02u-%02u-%02u %02u:%02u:%02u] %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, message);
	periodic_call = TRUE;
	//print_on_console(msg);
	HAL_UART_Transmit(get_console(NULL)->huart, (uint8_t*) msg, 512, HAL_MAX_DELAY);
}

void show_date_time_callback(TDatetime *datetime) {
	char msg[512] = { '\0' };
	char area_state[10] = { '\0' };
	char barrier_state[10] = { '\0' };

	PIR_get_string_state(&PIR_4, area_state);
	photoresistor_get_string_state(&photoresistor1, barrier_state);

	sprintf(msg, (uint8_t*) "[%02u-%02u-%02u %02u:%02u:%02u] Area %s - Barrier %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, area_state, barrier_state);
	// print_on_console(msg);
	HAL_UART_Transmit(get_console(NULL)->huart, (uint8_t*) msg, 512, HAL_MAX_DELAY);
}

void show_log_general() {
	TDatetime *datetime = get_configuration()->datetime;

	if (strlen(message_to_log) == 0) {
		show_date_time_callback(datetime);
	} else {
		write_message_with_date_time(datetime, message_to_log);
	}
}
