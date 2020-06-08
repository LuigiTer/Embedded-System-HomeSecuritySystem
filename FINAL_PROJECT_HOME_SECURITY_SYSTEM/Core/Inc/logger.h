/*
 * This module contains methods to handle with the logger, represented with a structure holding:
 * 		a pointer to the UART_HandleTypeDef structure representing the UART interface used to print
 * 			the log messages
 * 		a pointer to the PIR sensor to print the status of
 * 		a pointer to the photoresistor to print the status of
 * 		a message to print
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "console.h"
#include "pir_sensor.h"
#include "photoresistor.h"
#include "datetime.h"
#include "rtc_ds1307.h"
#include "bool.h"

/*
 * @brief	This struct represents the logger,
 * 			encapsulating the UART interface used to print the log messages.
 * @param	huart			pointer to the UART_HandleTypeDef structure
 *							representing the UART interface used to print the log messages
 * @param	pir				pointer to the PIR sensor to print the status of
 * @param	duty_cycle		pointer to the photoresistor to print the status of
 * @param	message			message to print
 */
typedef struct {
	UART_HandleTypeDef *huart;
	TPIR_sensor *pir;
	TPhotoresistor *photoresistor;
	char *message;
} TLogger;

/*
 *	@fn		void logger_init(TLogger *logger, UART_HandleTypeDef *huart, TPIR_sensor *pir, TPhotoresistor *photoresistor)
 *	@brief	Instantiates the logger
 *	@param	logger			pointer to the TLogger structure to store the parameters in
 *	@param	huart			pointer to the UART_HandleTypeDef structure
 *							representing the UART interface used to print the log messages
 *	@param	pir				pointer to the PIR sensor to print the status of
 *	@param	photoresistor	pointer to the photoresistor to print the status of
 */
void logger_init(TLogger *logger, UART_HandleTypeDef *huart, TPIR_sensor *pir, TPhotoresistor *photoresistor);

/**
 * @fn	static void logger_show_event_message(TDatetime *datetime, const char *event_message)
 * @brief	Logs the current datetime followed by a specific message on the console
 * @param	datetime		the datetime used to print the date
 * @param	event_message	the message to print
 */
static void logger_show_event_message(TDatetime *datetime, const char *event_message) {
	char msg[512] = { '\0' };
	sprintf(msg, (char*) "[%02u-%02u-%02u %02u:%02u:%02u] %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, event_message);

	print_message(msg);
}

/**
 * @fn	static void logger_show_periodic_message(TLogger *logger, TDatetime *datetime)
 * @brief	Logs the current datetime followed by a periodic message showing the status of the sensors
 * @param	logger		pointer to the TLogger structure
 * @param	datetime	the datetime used to print the date
 */
static void logger_show_periodic_message(TLogger *logger, TDatetime *datetime) {
	char msg[512] = { '\0' };
	char area_state[10] = { '\0' };
	char barrier_state[10] = { '\0' };

	PIR_get_string_state(logger->pir, area_state);
	photoresistor_get_string_state(logger->photoresistor, barrier_state);

	sprintf(msg, (uint8_t*) "[%02u-%02u-%02u %02u:%02u:%02u] Area %s - Barrier %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, area_state, barrier_state);

	print_message(msg);
}

/*
 * @fn	void logger_callback(TLogger *logger)
 * @brief	This function is called every time the RTC is asked to get the datetime.
 * 			Automatically shows either the periodic log message or an aperiodic event message
 * @param logger	pointer to the TLogger structure
 */
void logger_callback(TLogger *logger);

/*
 * @fn	void logger_print(TLogger *logger, char *event_message)
 * @brief	Sets a message to print in a TLogger structure, and shows the specific message
 * @param	logger			pointer to the TLogger structure
 * @param	event_message	the message to print
 */
void logger_print(TLogger *logger, char *event_message);

#endif /* INC_LOGGER_H_ */

