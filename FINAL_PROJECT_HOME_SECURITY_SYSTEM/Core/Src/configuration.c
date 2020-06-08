/*
 * This module contains methods to setup the initial configuration of the Home Security System.
 * The configuration parameters are stored in a singleton structure.
 * To instantiate the configuration, just call configuration_init() and system_boot() to setup.
 * Note that you need to instiate a console before starting the configuration setup.
 */

#include <configuration.h>

bool systemOn = FALSE;

extern TDatetime datetime;

/*
 * @fn		void configuration_init()
 * @brief	Creates the configuration singleton.
 * 			This function must be called before calling every other function in this module.
 * 			Note that after calling this function for the first time, the done parameter will be FALSE.
 */
void configuration_init() {
	get_configuration();
}

/*
 * @fn		TConfiguration* get_configuration()
 * @brief	Returns the singleton configuration instance.
 * 			If the instance has not been initialized yet,
 * 			then it will be initialized with the basic configuration.
 * 			If the instance has already been initialized,
 *			the configuration structure itself will be returned.
 * @retval	pointer to the TConfiguration structure representing the configuration
 */
TConfiguration* get_configuration() {
	static TConfiguration *configuration = NULL;

	if (configuration == NULL) {
		configuration = malloc(sizeof(*configuration));
		configuration->datetime = malloc(sizeof(configuration->datetime));

		strcpy(configuration->user_PIN, (uint8_t*) CONFIG_DEFAULT_USER_PIN);
		configuration->area_alarm_delay = CONFIG_DEFAULT_ALARM_DELAY;
		configuration->barrier_alarm_delay = CONFIG_DEFAULT_ALARM_DELAY;
		configuration->alarm_duration = CONFIG_DEFAULT_ALARM_DURATION;
		retrieve_current_date_time(configuration->datetime);
		configuration->done = FALSE;
	}

	return configuration;
}

/*
 *	@fn		void system_boot()
 *	@brief	Performs all the steps needed to setup all the configuration parameters, asking them to the user.
 *			If the user does not provide these parameters in 30 seconds,
 *			the default configuration will be set.
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
	rtc_ds1307_set_datetime(configuration->datetime);

	// Inform the user the system is ready for use
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	print_on_console(CONFIG_MESSAGE_READY);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_NEWLINE);
	while (!get_console(NULL)->ready) {
		HAL_Delay(1);
	}
}

/*
 * @fn		void configuration_recap()
 * @brief	Prints all the configuration parameters in a compact way
 */
void configuration_recap() {
	TConfiguration *configuration = get_configuration();

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
	print_on_console(CONFIG_MESSAGE_SHOW_DATETIME);
	show_date_time(configuration->datetime);
	print_on_console(CONFIG_NEWLINE);

	// Recap end
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_SEPARATOR);
	print_on_console(CONFIG_NEWLINE);
	print_on_console(CONFIG_NEWLINE);
}

/*
 * @fn		void show_date_time(TDatetime *datetime)
 * @brief	Prints a TDatetime structure in a compact and readble way (e.g. [08-11-1997 23:00:00])
 * @param	datetime	pointer to the TDatetime structure to print
 */
void show_date_time(TDatetime *datetime) {
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
