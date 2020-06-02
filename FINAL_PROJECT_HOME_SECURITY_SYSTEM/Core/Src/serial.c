#include "serial.h"

/*
 * Prints the message on the UART interface.
 */
void printMessage(UART_HandleTypeDef *huart, const char *message) {
	HAL_UART_Transmit(huart, (uint8_t*) message, strlen(message), HAL_MAX_DELAY);
}

/*
 * Clears the UART interface terminal.
 */
void clearTerminal(UART_HandleTypeDef *huart) {
	printMessage(huart, CLEAR_CONSOLE_ESCAPE_SEQUENCE);
	printMessage(huart, TOPLEFT_CURSOR_ESCAPE_SEQUENCE);
}

/*
 * Echoes a n-byte-long message on the UART interface, assigning the result to str.
 */
void echo(UART_HandleTypeDef *huart, const uint8_t n, char *str) {
	char buf[1];
	for (uint8_t i = 0; i < n; i++) {
		HAL_UART_Receive(huart, (uint8_t*) buf, 1, HAL_MAX_DELAY);
		str[i] = buf[0];
		HAL_UART_Transmit(huart, (uint8_t*) buf, 1, HAL_MAX_DELAY);
	}
}
