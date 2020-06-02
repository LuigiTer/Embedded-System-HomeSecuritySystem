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



#endif /* INC_SERIAL_H_ */
