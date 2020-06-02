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

void freeConsole() {
	while (!getConsole(NULL)->ready) {
		HAL_Delay(1);
	}
}


/*
 * Prints a char message on the console
 */
void printOnConsole(const char *message) {
	TConsole *console = getConsole(NULL);
	UART_HandleTypeDef *huart = console->huart;
	freeConsole();
	console->ready = FALSE;
	printMessage(huart, message);
}

/*
 * Clears the console
 */
void clearConsole() {
	printOnConsole(CLEAR_CONSOLE_ESCAPE_SEQUENCE);
	printOnConsole(TOPLEFT_CURSOR_ESCAPE_SEQUENCE);
}

/*
 * Prints an integer on the console
 */
void printIntOnConsole(const uint16_t n) {
	char str[digitsOf(n)];
	sprintf(str, "%d", n);
	printOnConsole(str);
}

void transmit(uint8_t *data, uint8_t n) {
	TConsole *console = getConsole(NULL);
	UART_HandleTypeDef *huart = console->huart;
	freeConsole();
	console->ready = FALSE;
	HAL_UART_Transmit_DMA(huart, data, n);
}

void receive(uint8_t *data, uint8_t n) {
	TConsole *console = getConsole(NULL);
	UART_HandleTypeDef *huart = console->huart;
	freeConsole();
	console->ready = FALSE;
	HAL_UART_Receive_DMA(huart, data, n);
	freeConsole();
}

/*
 * Echoes a n-byte-long message on the UART interface, assigning the result to str.
 */
void echo(UART_HandleTypeDef *huart, const uint8_t n, char *str) {
	uint8_t buf[1];
	for (uint8_t i = 0; i < n; i++) {
		// HAL_UART_Receive(huart, (uint8_t*) buf, 1, HAL_MAX_DELAY);
		// HAL_UART_Receive_DMA(huart, (uint8_t*) buf, 1);
		receive(buf, 1);
		str[i] = buf[0];
		// HAL_UART_Transmit(huart, (uint8_t*) buf, 1, HAL_MAX_DELAY);
		// HAL_UART_Transmit_DMA(huart, (uint8_t*) buf, 1);
		transmit(buf, 1);
	}
}
