#include <configuration.h>

static bool systemOn = FALSE;

/*
 * TODO documentation
 * Creates the console singleton.
 * This function must be called before calling every other function in this module.
 */
void configurationInit() {
	getConfiguration();
}

/*
 * TODO change documentation
 * Returns the singleton console instance.
 * If the instance has not been initialized yet and huart is not NULL,
 * then it will be initialized with huart and so the instance itself will be returned.
 * If the instance has not been initialized yet and huart is NULL, then the function will raise an error.
 * If the instance has already been initialized,
 * the parameter huart will be ineffective and the previous instance will be returned instead.
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
 * Prints a welcome message on the UART interface.
 */
void printWelcomeMessage() {
	printOnConsole(CONSOLE_MESSAGE_WELCOME_MESSAGE);
	printOnConsole(CONSOLE_NEWLINE);
}

void askForPIN(TConfiguration *configuration) {
	uint8_t userPIN2[USER_PIN_LENGTH];

	printOnConsole(CONSOLE_REQUEST_PIN);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	getUserPIN(configuration->userPIN);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole(CONSOLE_MESSAGE_CONFIRM_PIN);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	getUserPIN(userPIN2);

	if (!areEqual(configuration->userPIN, userPIN2, USER_PIN_LENGTH, USER_PIN_LENGTH)) {
		printOnConsole(CONSOLE_MESSAGE_ERROR);
		exit(1);
	}

	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_PIN);
	transmit(configuration->userPIN, USER_PIN_LENGTH);
	printOnConsole(CONSOLE_NEWLINE);
}

void askForAreaAlarmDelay(TConfiguration *configuration) {
	printOnConsole(CONSOLE_REQUEST_AREA_ALARM_DELAY);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	uint8_t alarmDelay = getIntLessThan(MAX_ALARM_DELAY, CONSOLE_REQUEST_LESS_THAN_MAX_ALARM_DELAY);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_AREA_ALARM_DELAY);
	printIntOnConsole(alarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONSOLE_NEWLINE);
	configuration->areaAlarmDelay = alarmDelay;
}

void askForBarrierAlarmDelay(TConfiguration *configuration) {
	printOnConsole(CONSOLE_REQUEST_BARRIER_ALARM_DELAY);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	uint8_t alarmDelay = getIntLessThan(MAX_ALARM_DELAY, CONSOLE_REQUEST_LESS_THAN_MAX_ALARM_DELAY);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_BARRIER_ALARM_DELAY);
	printIntOnConsole(alarmDelay);
	printOnConsole(" seconds");
	printOnConsole(CONSOLE_NEWLINE);
	configuration->barrierAlarmDelay = alarmDelay;
}

void askForAlarmDuration(TConfiguration *configuration) {
	printOnConsole(CONSOLE_REQUEST_ALARM_DURATION);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_PROMPT);
	uint8_t alarmDuration = getIntLessThan(MAX_ALARM_DURATION, CONSOLE_REQUEST_LESS_THAN_MAX_ALARM_DURATION);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_ALARM_DURATION);
	printIntOnConsole(alarmDuration);
	printOnConsole(" seconds");
	printOnConsole(CONSOLE_NEWLINE);
	configuration->alarmDuration = alarmDuration;
}

void showDatetime(TDatetime *datetime) {
	printOnConsole(CONSOLE_MESSAGE_SHOW_DATETIME);
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
	printOnConsole(CONSOLE_NEWLINE);
}

void askForDateTime(TConfiguration *configuration) {
	printOnConsole(CONSOLE_REQUEST_DATE_TIME);
	printOnConsole(CONSOLE_NEWLINE);

	TDatetime *datetime = configuration->datetime;

	printOnConsole("year (4 digits): ");
	datetime->year = getIntLessThan(3000, "Please insert a valid year") % 100;
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole("month [01-12]: ");
	datetime->month = getIntBetween(1, 12, "Month number must be in [01-12]");
	printOnConsole(CONSOLE_NEWLINE);

	char msg[32];
	uint8_t monthNumber = datetime->month - 1;
	sprintf(msg, "date [01-%d]: ", daysOfMonth(monthNumber));
	printOnConsole(msg);
	char msg2[32];
	sprintf(msg2, "Date number must be in [01-%d]", daysOfMonth(monthNumber));
	datetime->date = getIntBetween(1, daysOfMonth(monthNumber), msg2);
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
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_SEPARATOR);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_MESSAGE_SHOW_CONFIGURATION);
	printOnConsole(CONSOLE_NEWLINE);
	printOnConsole(CONSOLE_NEWLINE);

	printOnConsole(CONSOLE_MESSAGE_SHOW_PIN);
	transmit(configuration->userPIN, USER_PIN_LENGTH);
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
	printOnConsole(CONSOLE_NEWLINE);
}

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
	performNextStep(askForDateTime, tempConfiguration);

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
		printOnConsole(CONSOLE_NEWLINE);
		printOnConsole("Timeout! Basic configuration will be used...");
		printOnConsole(CONSOLE_NEWLINE);
	}

	configurationRecap(configuration);
}

void performNextStep(void (*nextStep)(TConfiguration*), TConfiguration *configuration) {
	TConfiguration *currentConfiguration = getConfiguration();
	if (!currentConfiguration->done) {
		(nextStep)(configuration);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1 && systemOn) {
		TConsole *console = getConsole(NULL);

		TConfiguration *configuration = getConfiguration();
		configuration->done = TRUE;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	TConsole *console = getConsole(NULL);

	if (huart == console->huart) {
		console->ready = TRUE;
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	TConsole *console = getConsole(NULL);

	if (huart == console->huart) {
		console->ready = TRUE;
	}
}


/*
periodelapsed() {
	// Get basic configuration
	configuration->userPIN = "0000";
	// ....
	configuration->datetime = rtc.getdatetime();
}
*/
