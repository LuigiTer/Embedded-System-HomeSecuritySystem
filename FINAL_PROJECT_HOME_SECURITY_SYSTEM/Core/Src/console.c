#include "console.h"

/*
 * Creates the console singleton.
 * This function must be called before calling every other function in this module.
 */
void consoleInit(UART_HandleTypeDef *huart) {
	getConsole(huart);
}

/*
 * Returns the singleton console instance.
 * If the instance has not been initialized yet and huart is not NULL,
 * then it will be initialized with huart and so the instance itself will be returned.
 * If the instance has not been initialized yet and huart is NULL, then the function will raise an error.
 * If the instance has already been initialized,
 * the parameter huart will be ineffective and the previous instance will be returned instead.
 */
TConsole* getConsole(UART_HandleTypeDef *huart) {
	static TConsole *instance = NULL;

	if (huart == NULL && instance == NULL) {
		exit(EXIT_FAILURE);
	}

	if (instance == NULL) {
		instance = malloc(sizeof(*instance));
		instance->huart = huart;
		instance->ready = TRUE;
	}

	return instance;
}

/*
 * Clears the console
 */
void clearConsole() {
	UART_HandleTypeDef *huart = getConsole(NULL)->huart;
	clearTerminal(huart);
}

/*
 * Prints a char message on the console
 */
void printOnConsole(const char *message) {
	UART_HandleTypeDef *huart = getConsole(NULL)->huart;
	printMessage(huart, message);
}

/*
 * Prints an integer on the console
 */
void printIntOnConsole(const uint16_t n) {
	char str[digitsOf(n)];
	sprintf(str, "%d", n);
	printOnConsole(str);
}
