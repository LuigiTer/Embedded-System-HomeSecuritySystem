/*
 * This module contains methods to handle with the console, representing it with a singleton.
 * To instantiate a console, just call consoleInit() with the UART interface you want to use to communicate.
 * To print messages, just call the proper methods, without specifying again the UART interface.
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "bool.h"
#include "utils.h"

#define ESCAPE_SEQUENCE_CLEAR_CONSOLE ("\033[2J")
#define ESCAPE_SEQUENCE_TOPLEFT_CURSOR ("\033[0;0H")

/*
 * @brief	This struct represents the console singleton,
 * 			encapsulating the UART interface used for communication.
 * @param	huart	pointer to the UART_HandleTypeDef structure
 * 					representing the UART interface used for communication
 * @param	ready	a boolean value set to TRUE if the console is ready for use, FALSE otherwise
 */
typedef struct {
	UART_HandleTypeDef *huart;
	bool ready;
} TConsole;

/*
 * @fn		void console_init(UART_HandleTypeDef *huart)
 * @brief	Creates the console singleton.
 * 			This function must be called before calling every other function in this module.
 * @param	huart	pointer to the UART_HandleTypeDef structure
 * 					representing the UART interface that will be used for communication
 */
void console_init(UART_HandleTypeDef *huart);

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
TConsole* get_console(UART_HandleTypeDef *huart);

/*
 * @fn		void clear_console()
 * @brief	Clears the console
 */
void clear_console();

/*
 * @fn		void free_console()
 * @brief	Waits unitl the console is ready to be used
 */
void free_console();

/*
 * @fn		static void print_message(const char *message)
 * @brief	Prints a string on the console.
 * 			It wraps the particular mode for transmission.
 * @param	message		string to print
 */
static void print_message(const char *message) {
	HAL_UART_Transmit_DMA(get_console(NULL)->huart, (uint8_t*) message, strlen(message));
}

/*
 * @fn		void print_on_console(const char *message)
 * @brief	Prints a string on the console, waiting if it is not ready for use
 * @param	message		string to print
 */
void print_on_console(const char *message);

/*
 * @fn		void print_int_on_console(const uint16_t n)
 * @brief	Prints an integer on the console, waiting if it is not ready for use
 * @param	n	number to print
 */
void print_int_on_console(const uint16_t n);

/*
 * @fn		void transmit(uint8_t *data, uint8_t n)
 * @brief	Trasmits the content of a data buffer on the console
 * @param	data	buffer containing the data to transmit
 * @param	n		length of the data to transmit
 */
void transmit(uint8_t *data, uint8_t n);

/*
 * @fn		void receive(uint8_t *data, uint8_t n)
 * @brief	Receives data from the console and stores them in a buffer
 * @param	data	buffer the received data will be stored in
 * @param	n		length of the data to receive
 */
void receive(uint8_t *data, uint8_t n);

/*
 * @fn		void echo(const uint8_t n, char *str)
 * @brief	Echoes a message on the console, storing it in a string.
 * @param	n		number of characters to trasmit
 * @param	str		string the data will be stored in
 */
void echo(const uint8_t n, char *str);

#endif /* INC_CONSOLE_H_ */
