#include <configuration.h>

static bool systemOn = FALSE;

/*
 * @fn		void configuration_init()
 * @brief	Creates the configuration singleton.
 * 			This function must be called before calling every other function in this module.
 * 			Note that after calling this function for the first time, the done paramter will be FALSE.
 */
void configuration_init() {
	get_configuration();
}

/*
 * @fn		TConfiguration* get_configuration()
 * @brief	Returns the singleton configuration instance.
 * 			If the instance has not been initialized yet,
 * 				then it will be initialized with the basic configuration.
 * 			If the instance has already been initialized,
 * 				the configuration structure itself will be returned.
 * @retval	pointer to the TConfiguration structure representing the configuration
 */
TConfiguration* get_configuration() {
	static TConfiguration *configuration = NULL;

	if (configuration == NULL) {
		configuration = malloc(sizeof(*configuration));
		configuration->datetime = malloc(sizeof(configuration->datetime));

		strcpy(configuration->user_PIN, "0000");
		configuration->area_alarm_delay = MAX_ALARM_DELAY;
		configuration->barrier_alarm_delay = MAX_ALARM_DELAY;
		configuration->alarm_duration = MAX_ALARM_DURATION;
		retrive_current_date_time(configuration->datetime);
		configuration->done = FALSE;
	}

	return configuration;
}

/*
 *	@fn		void system_boot()
 *	@brief	Performs all the steps needed to setup all the configuration parameters, asking them to the user.
 *			If the user does not provide these parameters in 30 seconds,
 *				the basic configuration will be set.
 */
void system_boot() {
	// Initial phase
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	clear_console();
	print_welcome_message();
	HAL_TIM_Base_Start_IT(&htim1);
	systemOn = TRUE;

	// tempConfiguration will store the information asked to the user
	TConfiguration *tempConfiguration = NULL;
	tempConfiguration = malloc(sizeof(*tempConfiguration));
	tempConfiguration->datetime = malloc(sizeof(tempConfiguration->datetime));

	// the actual configuration requests are here
	perform_next_step(ask_for_PIN, tempConfiguration);
	perform_next_step(ask_for_area_alarm_delay, tempConfiguration);
	perform_next_step(ask_for_barrier_alarm_delay, tempConfiguration);
	perform_next_step(ask_for_alarm_duration, tempConfiguration);
	perform_next_step(ask_for_datetime, tempConfiguration);

	// If the timer has not fired yet (getConfiguration()->done is FALSE), set the user configuration
	// If the timer has already fired (getConfiguration()->done is TRUE), the basic configuration is set
	TConfiguration *configuration = get_configuration();
	if (!configuration->done) {
		strcpy(configuration->user_PIN, tempConfiguration->user_PIN);
		configuration->area_alarm_delay = tempConfiguration->area_alarm_delay;
		configuration->barrier_alarm_delay = tempConfiguration->barrier_alarm_delay;
		configuration->alarm_duration = tempConfiguration->alarm_duration;
		configuration->datetime = tempConfiguration->datetime;
		configuration->done = TRUE;
		HAL_TIM_Base_Stop_IT(&htim1);
	} else {
		print_on_console(CONFIG_NEWLINE);
		print_on_console(CONFIG_TIMEOUT);
		print_on_console(CONFIG_NEWLINE);
	}

	// Prints all the configuration parameters in a compact way
	configuration_recap(configuration);

	// Inform the user the system is ready for use
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	print_on_console(CONFIG_MESSAGE_READY);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		void configuration_recap(TConfiguration *configuration)
 * @brief	Prints all the configuration parameters in a compact way
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void configuration_recap(TConfiguration *configuration) {
	// Recap start
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_SEPARATOR);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_MESSAGE_SHOW_CONFIGURATION);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_NEWLINE);

	// Print user PIN
	print_on_console(CONFIG_MESSAGE_SHOW_PIN);
	transmit(configuration->user_PIN, USER_PIN_LENGTH);
	print_on_console(CONFIG_NEWLINE);

	// Print number of seconds of the delay of the alarm for the AREA Sensor
	print_on_console(CONFIG_MESSAGE_SHOW_AREA_ALARM_DELAY);
	print_int_on_console(configuration->area_alarm_delay);
	print_on_console(" seconds");
	print_on_console(CONFIG_NEWLINE);

	// Print number of seconds of the delay of the alarm for the BARRIER Sensor
	print_on_console(CONFIG_MESSAGE_SHOW_BARRIER_ALARM_DELAY);
	print_int_on_console(configuration->barrier_alarm_delay);
	print_on_console(" seconds");
	print_on_console(CONFIG_NEWLINE);

	// Print number of seconds of the duration of the alarm
	print_on_console(CONFIG_MESSAGE_SHOW_ALARM_DURATION);
	print_int_on_console(configuration->alarm_duration);
	print_on_console(" seconds");
	print_on_console(CONFIG_NEWLINE);

	// Print datetime of the first use of the system
	show_date_time(configuration->datetime);
	print_on_console(CONFIG_NEWLINE);

	// Recap end
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_SEPARATOR);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		void print_welcome_message()
 * @brief	Prints a welcome message on the console.
 */
void print_welcome_message() {
	print_on_console(CONFIG_MESSAGE_WELCOME_MESSAGE);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		void ask_for_PIN(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console, reading the user PIN
 * 				and setting it in the configuration strucutre.
 * 			For confirmation purposes, it will be asked twice.
 * 				If two different sequences are inserted, it will raise an error.
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void ask_for_PIN(TConfiguration *configuration) {
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
 * 				reading the number of seconds of the delay of the alarm for the AREA Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void ask_for_area_alarm_delay(TConfiguration *configuration) {
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
 * @fn		void ask_for_barrier_alarm_delay(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the delay of the alarm for the BARRIER Sensor
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void ask_for_barrier_alarm_delay(TConfiguration *configuration) {
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
 * @fn		void ask_for_alarm_duration(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the number of seconds of the duration of the alarm
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void ask_for_alarm_duration(TConfiguration *configuration) {
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
 * @fn		void ask_for_datetime(TConfiguration *configuration)
 * @brief	Prints a series of messages on the console,
 * 				reading the datetime set by the user
 * @param	configuration	pointer to the TConfiguration structure
 * 				containing the system configuration parameters
 */
void ask_for_datetime(TConfiguration *configuration) {
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
	show_date_time(datetime);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		void show_date_time(TDatetime *datetime)
 * @brief	Prints a TDatetime structure in a compact and readble way (e.g. 08-11-1997 @ 23:00:00)
 * @param	datetime	pointer to the TDatetime structure to print
 */
void show_date_time(TDatetime *datetime) {
	print_on_console(CONFIG_MESSAGE_SHOW_DATETIME);
	print_on_console("[");
	print_int_on_console(datetime->date);
	print_on_console("-");
	print_int_on_console(datetime->month);
	print_on_console("-");
	print_int_on_console(datetime->year);
	print_on_console(" ");
	print_int_on_console(datetime->hour);
	print_on_console(":");
	print_int_on_console(datetime->minute);
	print_on_console(":");
	print_int_on_console(datetime->second);
	print_on_console("]");
}

/*
 * TIM1 lasts 10 seconds in One-Pulse Mode.
 * When this time has passed, the basic configuration will be set, and it's considered done.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1 && systemOn) {
		TConfiguration *configuration = get_configuration();
		configuration->done = TRUE;
	}
}

/*
 * When the UART interface has fully received the data, the console will be set to be ready to use
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	TConsole *console = get_console(NULL);

	if (huart == console->huart) {
		console->ready = TRUE;
	}
}

/*
 * When the UART interface has fully trasmitted the data, the console will be set to be ready to use
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	TConsole *console = get_console(NULL);

	if (huart == console->huart) {
		console->ready = TRUE;
	}
}
