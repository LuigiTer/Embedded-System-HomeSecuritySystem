#include "logger.h"



TLogger *logger_init(UART_HandleTypeDef *huart) {
	TLogger *logger = NULL;
	logger = malloc(sizeof(*logger));
	logger->huart = huart;
	logger->message = "\0";
	return logger;
}

/**
 * @brief ISR of the timer. This function will be called every time that a request of the current datetime is done to the rtc
 */
void logger_callback(TLogger *logger) {
	TDatetime *datetime = get_configuration()->datetime;

	if (strlen(logger->message) == 0) {
		show_periodic_log(datetime);
	} else {
		write_message_with_date_time(datetime, logger->message);
	}
}

void logger_print(TLogger *logger, const char *message) {
	logger->message = message;
	rtc_ds1307_get_datetime();
}
