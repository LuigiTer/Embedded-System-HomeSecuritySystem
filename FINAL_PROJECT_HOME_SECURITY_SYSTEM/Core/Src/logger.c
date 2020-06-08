/*
 * This module contains methods to handle with the logger, represented with a structure holding:
 * 		a pointer to the UART_HandleTypeDef structure representing the UART interface used to print
 * 			the log messages
 * 		a pointer to the PIR sensor to print the status of
 * 		a pointer to the photoresistor to print the status of
 * 		a message to print
 */

#include "logger.h"

/*
 *	@fn		void logger_init(TLogger *logger, UART_HandleTypeDef *huart, TPIR_sensor *pir, TPhotoresistor *photoresistor)
 *	@brief	Instantiates the logger
 *	@param	logger			pointer to the TLogger structure to store the parameters in
 *	@param	huart			pointer to the UART_HandleTypeDef structure
 *							representing the UART interface used to print the log messages
 *	@param	pir				pointer to the PIR sensor to print the status of
 *	@param	photoresistor	pointer to the photoresistor to print the status of
 */
void logger_init(TLogger *logger, UART_HandleTypeDef *huart, TPIR_sensor *pir, TPhotoresistor *photoresistor) {
	logger->huart = huart;
	logger->pir = pir;
	logger->photoresistor = photoresistor;
	logger->message[0] = '\0';
}

/*
 * @fn	void logger_callback(TLogger *logger)
 * @brief	This function is called every time the RTC is asked to get the datetime.
 * 			Automatically shows either the periodic log message or an aperiodic event message
 * @param logger	pointer to the TLogger structure
 */
void logger_callback(TLogger *logger) {
	TDatetime *datetime = get_configuration()->datetime;

	if (strlen(logger->message) == 0) {
		logger_show_periodic_message(logger, datetime);
	} else {
		logger_show_event_message(datetime, logger->message);
	}
}

/*
 * @fn	void logger_print(TLogger *logger, char *event_message)
 * @brief	Sets a message to print in a TLogger structure, and shows the specific message
 * @param	logger			pointer to the TLogger structure
 * @param	event_message	the message to print
 */
void logger_print(TLogger *logger, char *message) {
	logger->message = message;
	rtc_ds1307_get_datetime();
}
