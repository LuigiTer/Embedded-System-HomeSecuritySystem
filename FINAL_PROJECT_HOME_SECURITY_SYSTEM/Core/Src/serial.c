#include "serial.h"

/*
 * Prints the message on the UART interface.
 */
void printMessage(UART_HandleTypeDef *huart, const char *message) {
	HAL_UART_Transmit_DMA(huart, (uint8_t*) message, strlen(message));
	// HAL_UART_Transmit(huart, (uint8_t*) message, strlen(message), HAL_MAX_DELAY);
}

/*
 * Clears the UART interface terminal.
 */
void clearTerminal(UART_HandleTypeDef *huart) {
	printMessage(huart, CLEAR_CONSOLE_ESCAPE_SEQUENCE);
	printMessage(huart, TOPLEFT_CURSOR_ESCAPE_SEQUENCE);
}
