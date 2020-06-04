/*
 * This module contains methods to handle with the console, representing it with a singleton.
 * To instantiate a console, just call consoleInit() with the UART interface you want to use to communicate.
 * To print messages, just call the proper methods, without specifying again the UART interface.
 */

#include "console.h"

/*
 * @fn		void console_init(UART_HandleTypeDef *huart)
 * @brief	Creates the console singleton.
 * 			This function must be called before calling every other function in this module.
 * @param	huart	pointer to the UART_HandleTypeDef structure
 * 					representing the UART interface that will be used for communication
 */
void console_init(UART_HandleTypeDef *huart) {
	get_console(huart);
}

/*
 * @fn		TConsole* get_console(UART_HandleTypeDef *huart)
 * @brief	Returns the singleton console instance.
 * 			If the instance has not been initialized yet and huart is not NULL,
 * 				then it will be initialized with huart itself.
 * 			If the instance has not been initialized yet and huart is NULL,
 * 				then the function will raise an error.
 * 			If the instance has already been initialized, than the parameter huart will be uneffective
 * 				and the previous instance will be returned instead.
 * @param	huart	pointer to the UART_HandleTypeDef structure
 * @retval	pointer to the TConsole structure representing the console
 */
TConsole* get_console(UART_HandleTypeDef *huart) {
	static TConsole *console = NULL;

	if (huart == NULL && console == NULL) {
		exit(EXIT_FAILURE);
	}

	if (console == NULL) {
		console = malloc(sizeof(*console));
		console->huart = huart;
		console->ready = TRUE;
	}

	return console;
}

/*
 * @fn		void free_console()
 * @brief	Waits unitl the console is ready to be used
 */
void free_console() {
	while (!get_console(NULL)->ready) {
		HAL_Delay(1);
	}
}

/*
 * @fn		void print_on_console(const char *message)
 * @brief	Prints a string on the console, waiting if it is not ready for use
 * @param	message		string to print
 */
void print_on_console(const char *message) {
	free_console();
	get_console(NULL)->ready = FALSE;
	print_message(message);
}

/*
 * @fn		void clear_console()
 * @brief	Clears the console
 */
void clear_console() {
	print_on_console(ESCAPE_SEQUENCE_CLEAR_CONSOLE);
	print_on_console(ESCAPE_SEQUENCE_TOPLEFT_CURSOR);
}

/*
 * @fn		void print_int_on_console(const uint16_t n)
 * @brief	Prints an integer on the console, waiting if it is not ready for use
 * @param	n	number to print
 */
void print_int_on_console(const uint16_t n) {
	char str[digits_of(n)];
	sprintf(str, "%d", n);
	print_on_console(str);
}

/*
 * @fn		void transmit(uint8_t *data, uint8_t n)
 * @brief	Trasmits the content of a data buffer on the console
 * @param	data	buffer containing the data to transmit
 * @param	n		length of the data to transmit
 */
void transmit(uint8_t *data, uint8_t n) {
	TConsole *console = get_console(NULL);
	UART_HandleTypeDef *huart = console->huart;
	free_console();
	console->ready = FALSE;
	HAL_UART_Transmit_DMA(huart, data, n);
}

/*
 * @fn		void receive(uint8_t *data, uint8_t n)
 * @brief	Receives data from the console and stores them in a buffer
 * @param	data	buffer the received data will be stored in
 * @param	n		length of the data to receive
 */
void receive(uint8_t *data, uint8_t n) {
	TConsole *console = get_console(NULL);
	UART_HandleTypeDef *huart = console->huart;
	free_console();
	console->ready = FALSE;
	HAL_UART_Receive_DMA(huart, data, n);
	free_console();
}

/*
 * @fn		void echo(const uint8_t n, char *str)
 * @brief	Echoes a message on the console, storing it in a string.
 * @param	n		number of characters to trasmit
 * @param	str		string the data will be stored in
 */
void echo(const uint8_t n, char *str) {
	uint8_t buf[1];
	for (uint8_t i = 0; i < n; i++) {
		receive(buf, 1);
		str[i] = buf[0];
		transmit(buf, 1);
	}
}
