#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include <string.h>
#include "stm32f4xx_hal.h"

#define CLEAR_CONSOLE_ESCAPE_SEQUENCE ("\033[2J")
#define TOPLEFT_CURSOR_ESCAPE_SEQUENCE ("\033[0;0H")

/*
 * Prints the message on the UART interface.
 */
void printMessage(UART_HandleTypeDef *huart, const char *message);

/*
 * Clears the UART interface terminal.
 */
void clearTerminal(UART_HandleTypeDef *huart);

/*
 * Echoes a n-byte-long message on the UART interface, assigning the result to str.
 */
void echo(UART_HandleTypeDef *huart, const uint8_t n, char *str);

#endif /* INC_SERIAL_H_ */
