#include <configuration.h>

static bool systemOn = FALSE;

/*
 * @fn		void configurationInit()
 * @brief	Creates the configuration singleton.
 * 			This function must be called before calling every other function in this module.
 * 			Note that after calling this function for the first time, the done paramter will be FALSE.
 */
void configurationInit() {
	getConfiguration();
}

/*
 * @fn		TConfiguration* getConfiguration()
 * @brief	Returns the singleton configuration instance.
 * 			If the instance has not been initialized yet,
 * 				then it will be initialized with the basic configuration.
 * 			If the instance has already been initialized,
 * 				the configuration structure itself will be returned.
 * @retval	pointer to the TConfiguration structure representing the configuration
 */
TConfiguration* getConfiguration() {
	static TConfiguration *configuration = NULL;

	if (configuration == NULL) {
		configuration = malloc(sizeof(*configuration));
		configuration->datetime = malloc(sizeof(configuration->datetime));

		strcpy(configuration->userPIN, "0000");
		configuration->areaAlarmDelay = MAX_ALARM_DELAY;
		configuration->barrierAlarmDelay = MAX_ALARM_DELAY;
		configuration->alarmDuration = MAX_ALARM_DURATION;
		retriveCurrentDateTime(configuration->datetime);
		configuration->done = FALSE;
	}

	return configuration;
}

/*
 *	@fn		void systemBoot()
 *	@brief	Performs all the steps needed to setup all the configuration parameters, asking them to the user.
 *			If the user does not provide these parameters in 30 seconds,
 *				the basic configuration will be set.
 */
void systemBoot() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	clearConsole();
	printWelcomeMessage();
	HAL_TIM_Base_Start_IT(&htim1);
	systemOn = TRUE;

	TConfiguration *tempConfiguration = NULL;
	tempConfiguration = malloc(sizeof(*tempConfiguration));
	tempConfiguration->datetime = malloc(sizeof(tempConfiguration->datetime));

	performNextStep(askForPIN, tempConfiguration);
	performNextStep(askForAreaAlarmDelay, tempConfiguration);
	performNextStep(askForBarrierAlarmDelay, tempConfiguration);
	performNextStep(askForAlarmDuration, tempConfiguration);
	performNextStep(askForDatetime, tempConfiguration);

	TConfiguration *configuration = getConfiguration();
	if (!configuration->done) {
		strcpy(configuration->userPIN, tempConfiguration->userPIN);
		configuration->areaAlarmDelay = tempConfiguration->areaAlarmDelay;
		configuration->barrierAlarmDelay = tempConfiguration->barrierAlarmDelay;
		configuration->alarmDuration = tempConfiguration->alarmDuration;
		configuration->datetime = tempConfiguration->datetime;
		configuration->done = TRUE;
		HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	} else {
		printOnConsole(CONFIG_NEWLINE);
		printOnConsole("Timeout! Basic configuration will be used...");
		printOnConsole(CONFIG_NEWLINE);
	}

	configurationRecap(configuration);
}

/*
 * @fn		void configurationRecap(TConfiguration *configuration)
 * @brief	Prints all the configuration parameters in a compact way
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void configurationRecap(TConfiguration *configuration) {
	// Recap start
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_SEPARATOR);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_MESSAGE_SHOW_CONFIGURATION);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_NEWLINE);

	// Print user PIN
	printOnConsole(CONFIG_MESSAGE_SHOW_PIN);
	transmit(configuration->userPIN, USER_PIN_LENGTH);
	printOnConsole(CONFIG_NEWLINE);

	// Print number of seconds of the delay of the alarm for the AREA Sensor
	printOnConsole(CONFIG_MESSAGE_SHOW_AREA_ALARM_DELAY);
	printIntOnConsole(configuration->areaAlarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONFIG_NEWLINE);

	// Print number of seconds of the delay of the alarm for the BARRIER Sensor
	printOnConsole(CONFIG_MESSAGE_SHOW_BARRIER_ALARM_DELAY);
	printIntOnConsole(configuration->barrierAlarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONFIG_NEWLINE);

	// Print number of seconds of the duration of the alarm
	printOnConsole(CONFIG_MESSAGE_SHOW_ALARM_DURATION);
	printIntOnConsole(configuration->alarmDuration);
	printOnConsole(" seconds");
	printOnConsole(CONFIG_NEWLINE);

	// Print datetime of the first use of the system
	showDatetime(configuration->datetime);

	// Recap end
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_SEPARATOR);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_NEWLINE);
}

/*
 * @fn		void printWelcomeMessage()
 * @brief	Prints a welcome message on the console.
 */
void printWelcomeMessage() {
	printOnConsole(CONFIG_MESSAGE_WELCOME_MESSAGE);
	printOnConsole(CONFIG_NEWLINE);
}

/*
 * @fn		void askForPIN(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console, reading the user PIN
 * 				and setting it in the configuration strucutre.
 * 			For confirmation purposes, it will be asked twice.
 * 				If two different sequences are inserted, it will raise an error.
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForPIN(TConfiguration *configuration) {
	uint8_t userPIN2[USER_PIN_LENGTH];

	// Ask PIN for the first time
	printOnConsole(CONFIG_REQUEST_PIN);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_PROMPT);
	getUserPIN(configuration->userPIN);
	printOnConsole(CONFIG_NEWLINE);

	// Ask PIN for the second time
	printOnConsole(CONFIG_MESSAGE_CONFIRM_PIN);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_PROMPT);
	getUserPIN(userPIN2);

	// If the two sequences are not the same, an error message will be printed and the program ends
	if (!areEqual(configuration->userPIN, userPIN2, USER_PIN_LENGTH,
			USER_PIN_LENGTH)) {
		printOnConsole(CONFIG_MESSAGE_ERROR);
		exit(1);
	}

	// Print user PIN
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_MESSAGE_SHOW_PIN);
	transmit(configuration->userPIN, USER_PIN_LENGTH);
	printOnConsole(CONFIG_NEWLINE);
}

/*
 * @fn		void askForAreaAlarmDelay(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the delay of the alarm for the AREA Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForAreaAlarmDelay(TConfiguration *configuration) {
	// Ask number of seconds of the delay of the alarm for the AREA Sensor
	printOnConsole(CONFIG_REQUEST_AREA_ALARM_DELAY);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_PROMPT);
	uint8_t alarmDelay = getIntLessThan(MAX_ALARM_DELAY,
			CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DELAY);

	// Print number of seconds of the delay of the alarm for the AREA Sensor
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_MESSAGE_SHOW_AREA_ALARM_DELAY);
	printIntOnConsole(alarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONFIG_NEWLINE);
	configuration->areaAlarmDelay = alarmDelay;
}

/*
 * @fn		void askForBarrierAlarmDelay(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the delay of the alarm for the BARRIER Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForBarrierAlarmDelay(TConfiguration *configuration) {
	// Ask number of seconds of the delay of the alarm for the BARRIER Sensor
	printOnConsole(CONFIG_REQUEST_BARRIER_ALARM_DELAY);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_PROMPT);
	uint8_t alarmDelay = getIntLessThan(MAX_ALARM_DELAY,
			CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DELAY);

	// Print number of seconds of the delay of the alarm for the BARRIER Sensor
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_MESSAGE_SHOW_BARRIER_ALARM_DELAY);
	printIntOnConsole(alarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONFIG_NEWLINE);
	configuration->barrierAlarmDelay = alarmDelay;
}

/*
 * @fn		void askForAlarmDuration(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the duration of the alarm
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForAlarmDuration(TConfiguration *configuration) {
	// Ask number of seconds of the duration of the alarm
	printOnConsole(CONFIG_REQUEST_ALARM_DURATION);
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_PROMPT);
	uint8_t alarmDuration = getIntLessThan(MAX_ALARM_DURATION,
			CONFIG_REQUEST_LESS_THAN_MAX_ALARM_DURATION);

	// Print number of seconds of the duration of the alarm
	printOnConsole(CONFIG_NEWLINE);
	printOnConsole(CONFIG_MESSAGE_SHOW_ALARM_DURATION);
	printIntOnConsole(alarmDuration);
	printOnConsole(" seconds");
	printOnConsole(CONFIG_NEWLINE);
	configuration->alarmDuration = alarmDuration;
}

/*
 * @fn		void askForDatetime(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the datetime set by the user
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void askForDatetime(TConfiguration *configuration) {
	printOnConsole(CONFIG_REQUEST_DATE_TIME);
	printOnConsole(CONFIG_NEWLINE);

	TDatetime *datetime = configuration->datetime;

	// Ask year
	printOnConsole("year (4 digits): ");
	datetime->year = getIntLessThan(3000, "Please insert a valid year") % 100;
	printOnConsole(CONFIG_NEWLINE);

	// Ask month
	printOnConsole("month [01-12]: ");
	datetime->month = getIntBetween(1, 12, "Month number must be in [01-12]");
	printOnConsole(CONFIG_NEWLINE);

	// Ask date
	char msg[32];
	char msg2[32];
	uint8_t maxDays = daysOfMonth(datetime->month - 1);
	sprintf(msg, "date [01-%d]: ", maxDays);
	printOnConsole(msg);
	sprintf(msg2, "Date number must be in [01-%d]", maxDays);
	datetime->date = getIntBetween(1, maxDays, msg2);
	printOnConsole(CONFIG_NEWLINE);

	// Ask hour
	printOnConsole("hour [00-23]: ");
	datetime->hour = getIntLessThan(23, "Hour must be in [00-23]");
	printOnConsole(CONFIG_NEWLINE);

	// Ask minute
	printOnConsole("minute [00-59]: ");
	datetime->minute = getIntLessThan(59, "Minute must be in [00-59]");
	printOnConsole(CONFIG_NEWLINE);

	// Ask second
	printOnConsole("second [00-59]: ");
	datetime->second = getIntLessThan(59, "Second must be in [00-59]");
	printOnConsole(CONFIG_NEWLINE);

	// Print datetime set by the user
	showDatetime(datetime);
}

/*
 * @fn		void showDatetime(TDatetime *datetime)
 * @brief	Prints a TDatetime structure in a compact and readble way (e.g. 08-11-1997 @ 23:00:00)
 * @param	datetime	pointer to the TDatetime structure to print
 */
void showDatetime(TDatetime *datetime) {
	printOnConsole(CONFIG_MESSAGE_SHOW_DATETIME);
	printIntOnConsole(datetime->date);
	printOnConsole("-");
	printIntOnConsole(datetime->month);
	printOnConsole("-");
	printIntOnConsole(datetime->year);
	printOnConsole(" @ ");
	printIntOnConsole(datetime->hour);
	printOnConsole(":");
	printIntOnConsole(datetime->minute);
	printOnConsole(":");
	printIntOnConsole(datetime->second);
	printOnConsole(CONFIG_NEWLINE);
}

/*
 * TIM1 lasts 10 seconds in One-Pulse Mode.
 * When this time has passed, the basic configuration will be set, and it's considered done.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1 && systemOn) {
		TConfiguration *configuration = getConfiguration();
		configuration->done = TRUE;
	}
}

/*
 * When the UART interface has fully received the data, the console will be set to be ready to use
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	TConsole *console = getConsole(NULL);

	if (huart == console->huart) {
		console->ready = TRUE;
	}
}

/*
 * When the UART interface has fully trasmitted the data, the console will be set to be ready to use
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	TConsole *console = getConsole(NULL);

	if (huart == console->huart) {
		console->ready = TRUE;
	}
}
