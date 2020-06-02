#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "console.h"
#include "utils.h"
#include "datetime.h"


#define USER_PIN_LENGTH		(4)
#define MAX_ALARM_DELAY		(30)
#define MAX_ALARM_DURATION	(60)

#define CONSOLE_MESSAGE_WELCOME_MESSAGE ("Hello SMT32, I am your home security system HAL9000.")

#define CONSOLE_REQUEST_PIN ("Please insert your 4-digits PIN")
#define CONSOLE_REQUEST_DIGITS_ONLY ("Please insert only digits in [0-9]")
#define CONSOLE_MESSAGE_CONFIRM_PIN ("Please confirm your PIN")
#define CONSOLE_MESSAGE_SHOW_PIN ("Your PIN is: ")

#define CONSOLE_REQUEST_AREA_ALARM_DELAY ("Please insert alarm delay for AREA sensor (in seconds) [Max 30 seconds]")
#define CONSOLE_REQUEST_LESS_THAN_MAX_ALARM_DELAY ("Alarm delay must be less than 30 seconds")
#define CONSOLE_MESSAGE_SHOW_AREA_ALARM_DELAY ("Your alarm delay for AREA sensor is: ")

#define CONSOLE_REQUEST_BARRIER_ALARM_DELAY ("Please insert alarm delay for BARRIER sensor (in seconds) [Max 30 seconds]")
#define CONSOLE_MESSAGE_SHOW_BARRIER_ALARM_DELAY ("Your alarm delay for BARRIER sensor is: ")

#define CONSOLE_REQUEST_ALARM_DURATION ("Please insert alarm duration (in seconds) [Max 60 seconds]")
#define CONSOLE_REQUEST_LESS_THAN_MAX_ALARM_DURATION ("Alarm duration must be less than 60 seconds")
#define CONSOLE_MESSAGE_SHOW_ALARM_DURATION ("Your alarm duration is: ")

#define CONSOLE_REQUEST_DATE_TIME ("Please insert current datetime:")
#define CONSOLE_MESSAGE_SHOW_DATETIME ("Current datetime: ")

#define CONSOLE_MESSAGE_SHOW_CONFIGURATION ("Your configuration:")

#define CONSOLE_MESSAGE_ERROR ("Fatal Error!")
#define CONSOLE_NEWLINE ("\n\r")
#define CONSOLE_PROMPT ("> ")
#define CONSOLE_SEPARATOR ("==================================================")


typedef struct {
	uint8_t 	userPIN[USER_PIN_LENGTH];
	uint8_t 	areaAlarmDelay;
	uint8_t 	barrierAlarmDelay;
	uint8_t 	alarmDuration;
	TDatetime 	*datetime;
	bool		done;
} TConfiguration;


void configurationInit();

/*
 * TODO change documentation
 * Returns the singleton console instance.
 * If the instance has not been initialized yet and huart is not NULL,
 * then it will be initialized with huart and so the instance itself will be returned.
 * If the instance has not been initialized yet and huart is NULL, then the function will raise an error.
 * If the instance has already been initialized,
 * the parameter huart will be ineffective and the previous instance will be returned instead.
 */
TConfiguration* getConfiguration();

/*
 * Prints a welcome message on the UART interface.
 */
void printWelcomeMessage();


static void getUserPIN(uint8_t *buf) {
	receive(buf, USER_PIN_LENGTH);
	while (!isOnlyDigit(buf, USER_PIN_LENGTH)) {
		printOnConsole(CONSOLE_NEWLINE);
		printOnConsole(CONSOLE_REQUEST_DIGITS_ONLY);
		printOnConsole(CONSOLE_NEWLINE);
		printOnConsole(CONSOLE_PROMPT);
		receive(buf, USER_PIN_LENGTH);
	}
}

static uint16_t getIntBetween(const uint8_t min, const uint16_t max, const char *error) {
	UART_HandleTypeDef *huart = getConsole(NULL)->huart;
	uint8_t n = digitsOf(max);
	char str[n];

	echo(huart, n, str);

	while (!isOnlyDigit((uint8_t*) str, n)) {
		printOnConsole(CONSOLE_NEWLINE);
		printOnConsole(CONSOLE_REQUEST_DIGITS_ONLY);
		printOnConsole(CONSOLE_NEWLINE);
		printOnConsole(CONSOLE_PROMPT);
		echo(huart, n, str);
	}

	uint16_t retVal = atoi(str);

	if (!(min <= retVal && retVal <= max)) {
		printOnConsole(CONSOLE_NEWLINE);
		printOnConsole(error);
		printOnConsole(CONSOLE_NEWLINE);
		printOnConsole(CONSOLE_PROMPT);
		return getIntBetween(min, max, error);
	} else {
		return retVal;
	}
}

static uint16_t getIntLessThan(const uint16_t max, const char *error) {
	return getIntBetween(0, max, error);
}

void askForPIN(TConfiguration *configuration);

void askForAreaAlarmDelay(TConfiguration *configuration);

void askForBarrierAlarmDelay(TConfiguration *configuration);

void askForAlarmDuration(TConfiguration *configuration);

void showDatetime(TDatetime *datetime);

void askForDatetime(TConfiguration *configuration);

void configurationRecap(TConfiguration *configuration);

void systemBoot();

void performNextStep(void (*nextStep)(TConfiguration*), TConfiguration *configuration);

#endif /* INC_CONFIGURATION_H_ */
