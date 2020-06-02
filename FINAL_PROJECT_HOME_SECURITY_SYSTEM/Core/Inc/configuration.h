/*
 * This module contains methods to setup the initial configuration of the Home Security System.
 * The configuration parameters are stored in a singleton structure.
 * To instantiate the configuration, just call configurationInit() and systemBoot() to setup.
 * Note that you need to instiate a console before starting the configuration setup.
 */

#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "tim.h"

#include "console.h"
#include "datetime.h"
#include "utils.h"


#define USER_PIN_LENGTH		(4)
#define MAX_ALARM_DELAY		(30)
#define MAX_ALARM_DURATION	(60)

#define CONFIG_MESSAGE_WELCOME_MESSAGE ("Hello, SMT32. I am your home security system, HAL9000.\n\r\
You have 30 seconds to setup all the configuration parameters.\n\r")

#define CONFIG_REQUEST_PIN ("Please insert your 4-digits PIN")
#define CONFIG_REQUEST_DIGITS_ONLY ("Please insert only digits in [0-9]")
#define CONFIG_MESSAGE_CONFIRM_PIN ("Please confirm your PIN")
#define CONFIG_MESSAGE_SHOW_PIN ("Your PIN is: ")

#define CONFIG_REQUEST_AREA_ALARM_DELAY ("Please insert alarm delay for AREA sensor (in seconds) [Max 30 seconds]")
#define CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DELAY ("Alarm delay must be less than 30 seconds")
#define CONFIG_MESSAGE_SHOW_AREA_ALARM_DELAY ("Your alarm delay for AREA sensor is: ")

#define CONFIG_REQUEST_BARRIER_ALARM_DELAY ("Please insert alarm delay for BARRIER sensor (in seconds) [Max 30 seconds]")
#define CONFIG_MESSAGE_SHOW_BARRIER_ALARM_DELAY ("Your alarm delay for BARRIER sensor is: ")

#define CONFIG_REQUEST_ALARM_DURATION ("Please insert alarm duration (in seconds) [Max 60 seconds]")
#define CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DURATION ("Alarm duration must be less than 60 seconds")
#define CONFIG_MESSAGE_SHOW_ALARM_DURATION ("Your alarm duration is: ")

#define CONFIG_REQUEST_DATE_TIME ("Please insert current datetime:")
#define CONFIG_MESSAGE_SHOW_DATETIME ("Current datetime: ")

#define CONFIG_MESSAGE_SHOW_CONFIGURATION ("Your configuration:")

#define CONFIG_MESSAGE_ERROR ("Fatal Error!")
#define CONFIG_NEWLINE ("\n\r")
#define CONFIG_PROMPT ("> ")
#define CONFIG_SEPARATOR ("==================================================")

/*
 * @brief	This struct represents the system configuration, storing all the useful parameters.
 * @param	userPIN				pointer to the array containing the characters of the user PIN
 * @param	areaAlarmDelay		number of seconds of the delay of the alarm for the AREA Sensor
 * @param	barrierAlarmDelay	number of seconds of the delay of the alarm for the BARRIER Sensor
 * @param	alarmDuration		number of seconds of the duration of the alarm
 * @param	datetime			datetime of the first use of the system
 * @param	done				boolean value representing if the configuration is set or not
 */
typedef struct {
	uint8_t userPIN[USER_PIN_LENGTH];
	uint8_t areaAlarmDelay;
	uint8_t barrierAlarmDelay;
	uint8_t alarmDuration;
	TDatetime *datetime;
	bool done;
} TConfiguration;

/*
 * @fn		void configurationInit()
 * @brief	Creates the configuration singleton.
 * 			This function must be called before calling every other function in this module.
 * 			Note that after calling this function for the first time, the done paramter will be FALSE.
 */
void configurationInit();

/*
 * @fn		TConfiguration* getConfiguration()
 * @brief	Returns the singleton configuration instance.
 * 			If the instance has not been initialized yet,
 * 				then it will be initialized with the basic configuration.
 * 			If the instance has already been initialized,
 * 				the configuration structure itself will be returned.
 * @retval	pointer to the TConfiguration structure representing the configuration
 */
TConfiguration* getConfiguration();

/*
 *	@fn		void systemBoot()
 *	@brief	Performs all the steps needed to setup all the configuration parameters, asking them to the user.
 *			If the user does not provide these parameters in 30 seconds,
 *				the basic configuration will be set.
 */
void systemBoot();

/*
 * @fn		void configurationRecap(TConfiguration *configuration)
 * @brief	Prints all the configuration parameters in a compact way
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void configurationRecap(TConfiguration *configuration);

/*
 * @fn		void printWelcomeMessage()
 * @brief	Prints a welcome message on the console.
 */
void printWelcomeMessage();

/*
 * @fn		void askForPIN(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console, reading the user PIN
 * 				and setting it in the configuration strucutre.
 * 			For confirmation purposes, it will be asked twice.
 * 				If two different sequences are inserted, it will raise an error.
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForPIN(TConfiguration *configuration);

/*
 * @fn		void askForAreaAlarmDelay(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the delay of the alarm for the AREA Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForAreaAlarmDelay(TConfiguration *configuration);

/*
 * @fn		void askForBarrierAlarmDelay(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the delay of the alarm for the BARRIER Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForBarrierAlarmDelay(TConfiguration *configuration);

/*
 * @fn		void askForAlarmDuration(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the duration of the alarm
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForAlarmDuration(TConfiguration *configuration);

/*
 * @fn		void askForDatetime(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the datetime set by the user
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForDatetime(TConfiguration *configuration);

/*
 * @fn		static void getUserPIN(uint8_t *buf)
 * @brief	Receives the user PIN from the console and stored it in a buffer
 * @param	buf		buffer the received user PIN will be stored in
 */
static void getUserPIN(uint8_t *buf) {
	receive(buf, USER_PIN_LENGTH);
	while (!isOnlyDigit(buf, USER_PIN_LENGTH)) {
		printOnConsole(CONFIG_NEWLINE);
		printOnConsole(CONFIG_REQUEST_DIGITS_ONLY);
		printOnConsole(CONFIG_NEWLINE);
		printOnConsole(CONFIG_PROMPT);
		receive(buf, USER_PIN_LENGTH);
	}
}

/*
 * @fn		static uint16_t getIntBetween(const uint8_t min, const uint16_t max, const char *error)
 * @brief	Receives an integer from the console in a specified range,
 *			printing an error message if the constraints are not respected.
 * @param	min		lower bound of the constraint range
 * @param	max		upper bound of the constraint range
 * @param	error	string printed if the received number is not in [min, max]
 * @retval	number in [min, max] read from the console
 */
static uint16_t getIntBetween(const uint8_t min, const uint16_t max,
		const char *error) {
	uint8_t n = digitsOf(max);
	char str[n];

	echo(n, str);

	while (!isOnlyDigit((uint8_t*) str, n)) {
		printOnConsole(CONFIG_NEWLINE);
		printOnConsole(CONFIG_REQUEST_DIGITS_ONLY);
		printOnConsole(CONFIG_NEWLINE);
		printOnConsole(CONFIG_PROMPT);
		echo(n, str);
	}

	uint16_t retVal = atoi(str);

	if (!(min <= retVal && retVal <= max)) {
		printOnConsole(CONFIG_NEWLINE);
		printOnConsole(error);
		printOnConsole(CONFIG_NEWLINE);
		printOnConsole(CONFIG_PROMPT);
		return getIntBetween(min, max, error);
	} else {
		return retVal;
	}
}

/*
 * @fn		static uint16_t getIntLessThan(const uint16_t max, const char *error)
 * @brief	Receives an integer from the console less than a specified limit,
 *			printing an error message if the constraint is not respected.
 * @param	max		upper bound of the constraint
 * @param	error	string printed if the received number is not in [0, max]
 * @retval	number in [0, max] read from the console
 */
static uint16_t getIntLessThan(const uint16_t max, const char *error) {
	return getIntBetween(0, max, error);
}

/*
 * @fn		void showDatetime(TDatetime *datetime)
 * @brief	Prints a TDatetime structure in a compact and readble way (e.g. 08-11-1997 @ 23:00:00)
 * @param	datetime	pointer to the TDatetime structure to print
 */
void showDatetime(TDatetime *datetime);

/*
 * @fn		Performs the next step needed to setup the next configuration parameter, asking it to the user.
 * 			If the configuration is done, nothing will be performed at all.
 * @param	nextStep		a void(TConfiguration*) function containing the next step to perform
 * 								if the configuration is not done yet
 * @pram	configuration	pointer to the TConfiguration structure that will store the next parameter.
 */
static void performNextStep(void (*nextStep)(TConfiguration*),
		TConfiguration *configuration) {
	TConfiguration *currentConfiguration = getConfiguration();
	if (!currentConfiguration->done) {
		(nextStep)(configuration);
	}
}

#endif /* INC_CONFIGURATION_H_ */
