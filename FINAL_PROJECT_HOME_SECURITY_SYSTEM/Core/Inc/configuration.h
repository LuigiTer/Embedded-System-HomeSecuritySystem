/*
 * This module contains methods to setup the initial configuration of the Home Security System.
 * The configuration parameters are stored in a singleton structure.
 * To instantiate the configuration, just call configuration_init() and system_boot() to setup.
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

#define CONFIG_DEFAULT_USER_PIN 		("0000")
#define CONFIG_DEFAULT_ALARM_DELAY 		(0)
#define CONFIG_DEFAULT_ALARM_DURATION	(5)

#define CONFIG_MESSAGE_WELCOME_MESSAGE ("Hello, SMT32. I am your Home Security System, HAL9000.\n\r\
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
#define CONFIG_MESSAGE_READY ("Configuration completed. Your Home Security System is ready for use.")

#define CONFIG_TIMEOUT ("Timeout! Basic configuration will be used...")
#define CONFIG_MESSAGE_ERROR ("Fatal Error!")
#define CONFIG_MESSAGE_ERROR_WRONG_PIN ("Wrong PIN!")

#define CONFIG_NEWLINE ("\n\r")
#define CONFIG_PROMPT ("> ")
#define CONFIG_SEPARATOR ("==================================================")

/*
 * @brief	This struct represents the system configuration, storing all the useful parameters.
 * @param	user_PIN				pointer to the array containing the characters of the user PIN
 * @param	area_alarm_delay		number of seconds of the delay of the alarm for the AREA Sensor
 * @param	barrier_alarm_delay		number of seconds of the delay of the alarm for the BARRIER Sensor
 * @param	alarm_duration			number of seconds of the duration of the alarm
 * @param	datetime				datetime of the first use of the system
 * @param	done					boolean value representing if the configuration is set or not
 */
typedef struct {
	uint8_t user_PIN[USER_PIN_LENGTH];
	uint8_t area_alarm_delay;
	uint8_t barrier_alarm_delay;
	uint8_t alarm_duration;
	TDatetime *datetime;
	bool done;
} TConfiguration;

/*
 * @fn		void configuration_init()
 * @brief	Creates the configuration singleton.
 * 			This function must be called before calling every other function in this module.
 * 			Note that after calling this function for the first time, the done parameter will be FALSE.
 */
void configuration_init();

/*
 * @fn		TConfiguration* get_configuration()
 * @brief	Returns the singleton configuration instance.
 * 			If the instance has not been initialized yet,
 * 			then it will be initialized with the basic configuration.
 * 			If the instance has already been initialized,
 *			the configuration structure itself will be returned.
 * @retval	pointer to the TConfiguration structure representing the configuration
 */
TConfiguration* get_configuration();

/*
 *	@fn		void system_boot()
 *	@brief	Performs all the steps needed to setup all the configuration parameters, asking them to the user.
 *			If the user does not provide these parameters in 30 seconds,
 *			the default configuration will be set.
 */
void system_boot();

/*
 * @fn		void configuration_recap()
 * @brief	Prints all the configuration parameters in a compact way
 */
void configuration_recap();

/*
 * @fn		void show_date_time(TDatetime *datetime)
 * @brief	Prints a TDatetime structure in a compact and readble way (e.g. [08-11-1997 23:00:00])
 * @param	datetime	pointer to the TDatetime structure to print
 */
void show_date_time(TDatetime *datetime);

/*
 * @fn		void print_welcome_message()
 * @brief	Prints a welcome message on the console.
 */
static void print_welcome_message() {
	print_on_console(CONFIG_MESSAGE_WELCOME_MESSAGE);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		static void get_user_PIN(uint8_t *buf)
 * @brief	Receives the user PIN from the console and stores it in a buffer
 * @param	buf		buffer the received user PIN will be stored in
 */
static void get_user_PIN(uint8_t *buf) {
	receive(buf, USER_PIN_LENGTH);
	while (!is_only_digit(buf, USER_PIN_LENGTH)) {
		print_on_console(CONFIG_NEWLINE);
		print_on_console(CONFIG_REQUEST_DIGITS_ONLY);
		print_on_console(CONFIG_NEWLINE);
		print_on_console(CONFIG_PROMPT);
		receive(buf, USER_PIN_LENGTH);
	}
}

/*
 * @fn		static uint16_t get_int_between(const uint8_t min, const uint16_t max, const char *error)
 * @brief	Receives an integer from the console in a specified range,
 *			printing an error message if the constraints are not respected.
 * @param	min		lower bound of the constraint range
 * @param	max		upper bound of the constraint range
 * @param	error	string printed if the received number is not in [min, max]
 * @retval	number in [min, max] read from the console
 */
static uint16_t get_int_between(const uint16_t min, const uint16_t max, const char *error) {
	uint8_t n = digits_of(max);
	char str[n];

	echo(n, str);

	while (!is_only_digit((uint8_t*) str, n)) {
		print_on_console(CONFIG_NEWLINE);
		print_on_console(CONFIG_REQUEST_DIGITS_ONLY);
		print_on_console(CONFIG_NEWLINE);
		print_on_console(CONFIG_PROMPT);
		echo(n, str);
	}

	uint16_t retVal = atoi(str);

	if (!(min <= retVal && retVal <= max)) {
		print_on_console(CONFIG_NEWLINE);
		print_on_console(error);
		print_on_console(CONFIG_NEWLINE);
		print_on_console(CONFIG_PROMPT);
		return get_int_between(min, max, error);
	} else {
		return retVal;
	}
}

/*
 * @fn		static uint16_t get_int_less_than(const uint16_t max, const char *error)
 * @brief	Receives an integer from the console less than a specified limit,
 *			printing an error message if the constraint is not respected.
 * @param	max		upper bound of the constraint
 * @param	error	string printed if the received number is not in [0, max]
 * @retval	number in [1, max] read from the console
 */
static uint16_t get_int_less_than(const uint16_t max, const char *error) {
	return get_int_between(0, max, error);
}

/*
 * @fn		static void ask_for_PIN(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console, reading the user PIN
 * 			and setting it in the configuration strucutre.
 * 			For confirmation purposes, it will be asked twice.
 * 			If two different sequences are inserted, it will raise an error.
 * @param	configuration	pointer to the TConfiguration structure
 * 			containing the system configuration parameters
 */
static void ask_for_PIN(TConfiguration *configuration) {
	uint8_t userPIN2[USER_PIN_LENGTH];

	// Ask PIN for the first time
	print_on_console(CONFIG_REQUEST_PIN);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_PROMPT);
	get_user_PIN(configuration->user_PIN);
	print_on_console(CONFIG_NEWLINE);

	// Ask PIN for the second time
	print_on_console(CONFIG_MESSAGE_CONFIRM_PIN);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_PROMPT);
	get_user_PIN(userPIN2);

	// If the two sequences are not the same, an error message will be printed and the program ends
	if (!are_equal(configuration->user_PIN, userPIN2, USER_PIN_LENGTH, USER_PIN_LENGTH)) {
		print_on_console(CONFIG_MESSAGE_ERROR);
		exit(1);
	}

	// Print user PIN
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_MESSAGE_SHOW_PIN);
	transmit(configuration->user_PIN, USER_PIN_LENGTH);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		void ask_for_area_alarm_delay(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 			reading the number of seconds of the delay of the alarm for the AREA Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 			containing the system configuration parameters
 */
static void ask_for_area_alarm_delay(TConfiguration *configuration) {
	// Ask number of seconds of the delay of the alarm for the AREA Sensor
	print_on_console(CONFIG_REQUEST_AREA_ALARM_DELAY);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_PROMPT);
	uint8_t alarmDelay = get_int_less_than(MAX_ALARM_DELAY, CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DELAY);

	// Print number of seconds of the delay of the alarm for the AREA Sensor
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_MESSAGE_SHOW_AREA_ALARM_DELAY);
	print_int_on_console(alarmDelay);
	print_on_console(" seconds");
	print_on_console(CONFIG_NEWLINE);
	configuration->area_alarm_delay = alarmDelay;
}

/*
 * @fn		static void ask_for_barrier_alarm_delay(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 			reading the number of seconds of the delay of the alarm for the BARRIER Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 			containing the system configuration parameters
 */
static void ask_for_barrier_alarm_delay(TConfiguration *configuration) {
	// Ask number of seconds of the delay of the alarm for the BARRIER Sensor
	print_on_console(CONFIG_REQUEST_BARRIER_ALARM_DELAY);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_PROMPT);
	uint8_t alarmDelay = get_int_less_than(MAX_ALARM_DELAY, CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DELAY);

	// Print number of seconds of the delay of the alarm for the BARRIER Sensor
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_MESSAGE_SHOW_BARRIER_ALARM_DELAY);
	print_int_on_console(alarmDelay);
	print_on_console(" seconds");
	print_on_console(CONFIG_NEWLINE);
	configuration->barrier_alarm_delay = alarmDelay;
}

/*
 * @fn		static void ask_for_alarm_duration(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 			reading the number of seconds of the duration of the alarm
 * @param	configuration	pointer to the TConfiguration structure
 * 			containing the system configuration parameters
 */
static void ask_for_alarm_duration(TConfiguration *configuration) {
	// Ask number of seconds of the duration of the alarm
	print_on_console(CONFIG_REQUEST_ALARM_DURATION);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_PROMPT);
	uint8_t alarmDuration = get_int_less_than(MAX_ALARM_DURATION, CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DURATION);

	// Print number of seconds of the duration of the alarm
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_MESSAGE_SHOW_ALARM_DURATION);
	print_int_on_console(alarmDuration);
	print_on_console(" seconds");
	print_on_console(CONFIG_NEWLINE);
	configuration->alarm_duration = alarmDuration;
}

/*
 * @fn		static void ask_for_datetime(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 			reading the datetime set by the user
 * @param	configuration	pointer to the TConfiguration structure
 * 			containing the system configuration parameters
 */
static void ask_for_datetime(TConfiguration *configuration) {
	print_on_console(CONFIG_REQUEST_DATE_TIME);
	print_on_console(CONFIG_NEWLINE);

	TDatetime *datetime = configuration->datetime;

	// Ask year
	print_on_console("year [0-99]: ");
	datetime->year = get_int_less_than(99, "Please insert a valid year");
	print_on_console(CONFIG_NEWLINE);

	// Ask month
	print_on_console("month [01-12]: ");
	datetime->month = get_int_between(1, 12, "Month number must be in [01-12]");
	print_on_console(CONFIG_NEWLINE);

	// Ask date
	char msg[32];
	char msg2[32];
	uint8_t maxDays = days_of_month(datetime->month - 1);
	sprintf(msg, "date [01-%d]: ", maxDays);
	print_on_console(msg);
	sprintf(msg2, "Date number must be in [01-%d]", maxDays);
	datetime->date = get_int_between(1, maxDays, msg2);
	print_on_console(CONFIG_NEWLINE);

	// Ask hour
	print_on_console("hour [00-23]: ");
	datetime->hour = get_int_less_than(23, "Hour must be in [00-23]");
	print_on_console(CONFIG_NEWLINE);

	// Ask minute
	print_on_console("minute [00-59]: ");
	datetime->minute = get_int_less_than(59, "Minute must be in [00-59]");
	print_on_console(CONFIG_NEWLINE);

	// Ask second
	print_on_console("second [00-59]: ");
	datetime->second = get_int_less_than(59, "Second must be in [00-59]");
	print_on_console(CONFIG_NEWLINE);

	// Print datetime set by the user
	print_on_console(CONFIG_MESSAGE_SHOW_DATETIME);
	show_date_time(datetime);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		static void perform_next_step(void (*nextStep)(TConfiguration*), TConfiguration *configuration)
 * @brief	Performs the next step needed to setup the next configuration parameter, asking it to the user.
 * 			If the configuration is done, nothing will be performed at all.
 * @param	nextStep		a void(TConfiguration*) function pointer containing the next step to perform
 * 							if the configuration is not done yet
 * @pram	configuration	pointer to the TConfiguration structure that will store the next parameter.
 */
static void perform_next_step(void (*nextStep)(TConfiguration*), TConfiguration *configuration) {
	TConfiguration *currentConfiguration = get_configuration();
	if (!currentConfiguration->done) {
		(nextStep)(configuration);
	}
}

#endif /* INC_CONFIGURATION_H_ */
