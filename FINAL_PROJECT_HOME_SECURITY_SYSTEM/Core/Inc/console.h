#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include <stdlib.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "serial.h"
#include "bool.h"
#include "utils.h"


typedef struct {
	UART_HandleTypeDef *huart;
	bool ready;
} TConsole;


/*
 * Creates the console singleton.
 * This function must be called before calling every other function in this module.
 */
void consoleInit(UART_HandleTypeDef *huart);


/*
 * Returns the singleton console instance.
 * If the instance has not been initialized yet and huart is not NULL,
 * then it will be initialized with huart and so huart itself will be returned.
 * If the instance has not been initialized yet and huart is NULL, then the function will raise an error.
 * If the instance has already been initialized,
 * the parameter huart will be uneffective and the previous instance will be returned instead.
 */
TConsole* getConsole(UART_HandleTypeDef *huart);

/*
 * Clears the console
 */
void clearConsole();

void freeConsole();

/*
 * Prints a char message on the console
 */
void printOnConsole(const char *message);

/*
 * Prints an integer on the console
 */
void printIntOnConsole(const uint16_t n);


void transmit(uint8_t *data, uint8_t n);

void receive(uint8_t *data, uint8_t n);

/*
 * Echoes a n-byte-long message on the UART interface, assigning the result to str.
 */
void echo(UART_HandleTypeDef *huart, const uint8_t n, char *str);

#endif /* INC_CONSOLE_H_ */
