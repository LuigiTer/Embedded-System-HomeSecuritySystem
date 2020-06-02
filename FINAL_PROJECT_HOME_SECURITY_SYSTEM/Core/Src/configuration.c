#include <configuration.h>


/*
 * Prints a welcome message on the UART interface.
 */
void printWelcomeMessage() {
	printOnConsole(CONSOLE_MESSAGE_WELCOME_MESSAGE);
	printOnConsole(CONSOLE_NEWLINE);
}

void askForPIN(uint8_t *userPIN) {
	UART_HandleTypeDef *huart = getConsole(NULL)->huart;

	uint8_t userPIN2[USER_PIN_LENGTH];

	printOnConsole(CONSOLE_REQUEST_PIN);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	getUserPIN(userPIN);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole(CONSOLE_MESSAGE_CONFIRM_PIN);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	getUserPIN(userPIN2);

	if (!areEqual(userPIN, userPIN2, USER_PIN_LENGTH, USER_PIN_LENGTH)) {
		printOnConsole(CONSOLE_MESSAGE_ERROR);
		exit(1);
	}

	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_PIN);
	HAL_UART_Transmit(huart, userPIN, USER_PIN_LENGTH, HAL_MAX_DELAY);
	printOnConsole(CONSOLE_NEWLINE);
}

uint8_t askForAreaAlarmDelay() {
	printOnConsole(CONSOLE_REQUEST_AREA_ALARM_DELAY);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	uint8_t alarmDelay = getAlarmDelay();
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_AREA_ALARM_DELAY);
	printIntOnConsole(alarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONSOLE_NEWLINE);
	return alarmDelay;
}

uint8_t askForBarrierAlarmDelay() {
	printOnConsole(CONSOLE_REQUEST_BARRIER_ALARM_DELAY);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	uint8_t alarmDelay = getAlarmDelay();
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_BARRIER_ALARM_DELAY);
	printIntOnConsole(alarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONSOLE_NEWLINE);
	return alarmDelay;
}

uint8_t askForAlarmDuration() {
	printOnConsole(CONSOLE_REQUEST_ALARM_DURATION);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	uint8_t alarmDuration = getAlarmDuration();
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_ALARM_DURATION);
	printIntOnConsole(alarmDuration);
	printOnConsole(" seconds");
	printOnConsole(CONSOLE_NEWLINE);
	return alarmDuration;
}

void showDatetime(TDatetime *datetime) {
	printOnConsole(CONSOLE_MESSAGE_SHOW_DATETIME);
	printIntOnConsole(datetime->year);
	printOnConsole("-");
	printIntOnConsole(datetime->month);
	printOnConsole("-");
	printIntOnConsole(datetime->date);
	printOnConsole(" @ ");
	printIntOnConsole(datetime->hour);
	printOnConsole(":");
	printIntOnConsole(datetime->minute);
	printOnConsole(":");
	printIntOnConsole(datetime->second);
	printOnConsole(CONSOLE_NEWLINE);
}

void askForDateTime(TDatetime *datetime) {
	printOnConsole(CONSOLE_REQUEST_DATE_TIME);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole("year [00-99]: ");
	datetime->year = getIntLessThan(99, "Year must be in range [00-99]");
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole("month [01-12]: ");
	datetime->month = getIntBetween(1, 12, "Month number must be in [01-12]");
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole("date [01-31]: ");
	datetime->date = getIntBetween(1, 31, "Date number must be in [01-31]");
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole("hour [00-23]: ");
	datetime->hour = getIntLessThan(23, "Hour must be in [00-23]");
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole("minute [00-59]: ");
	datetime->minute = getIntLessThan(59, "Minute must be in [00-59]");
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole("second [00-59]: ");
	datetime->second = getIntLessThan(59, "Second must be in [00-59]");
	printOnConsole(CONSOLE_NEWLINE);

	showDatetime(datetime);
}

void configurationRecap(TConfiguration *configuration) {
	UART_HandleTypeDef *huart = getConsole(NULL)->huart;

	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_SEPARATOR);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_CONFIGURATION);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole(CONSOLE_MESSAGE_SHOW_PIN);
	HAL_UART_Transmit(huart, configuration->userPIN, USER_PIN_LENGTH, HAL_MAX_DELAY);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole(CONSOLE_MESSAGE_SHOW_AREA_ALARM_DELAY);
	printIntOnConsole(configuration->areaAlarmDelay);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole(CONSOLE_MESSAGE_SHOW_BARRIER_ALARM_DELAY);
	printIntOnConsole(configuration->barrierAlarmDelay);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole(CONSOLE_MESSAGE_SHOW_ALARM_DURATION);
	printIntOnConsole(configuration->alarmDuration);
	printOnConsole(CONSOLE_NEWLINE);

	showDatetime(configuration->datetime);

	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_SEPARATOR);
	printOnConsole(CONSOLE_NEWLINE);
}

void systemBoot() {
	TConfiguration *configuration = NULL;
	configuration = malloc(sizeof(*configuration));
	configuration->datetime = malloc(sizeof(configuration->datetime));

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	clearConsole();
	printWelcomeMessage();
	askForPIN(configuration->userPIN);
	configuration->areaAlarmDelay = askForAreaAlarmDelay();
	configuration->barrierAlarmDelay = askForBarrierAlarmDelay();
	configuration->alarmDuration = askForAlarmDuration();
	askForDateTime(configuration->datetime);
	configurationRecap(configuration);


	/*
	char *temp = __TIME__;
	printOnConsole(temp);
	temp = __DATE__;
	printOnConsole(temp);
	*/
}


/*
periodelapsed() {
	// Get basic configuration
	configuration->userPIN = "0000";
	// ....
	configuration->datetime = rtc.getdatetime();
}
*/
