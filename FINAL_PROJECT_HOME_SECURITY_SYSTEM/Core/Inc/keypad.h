/*
 * This module allows to connect a keypad to the system. This module will also manage the activation and deactivation of
 * the sensors. The user pin is automatically retrieved by the configuration.
 * */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "main.h"
#include "configuration.h"
#include "keypad_configuration.h"
#include "tim.h"
#include "bool.h"
#include "pir_sensor.h"
#include "photoresistor.h"
#include "logger.h"
#include "buzzer.h"

#define MESSAGE_WRONG_USER_PIN 		("Wrong user pin inserted")
#define MESSAGE_COMMAND_REJECTED	("Command rejected")
#define MESSAGE_COMMAND_ACCEPTED	("Command accepted")


/**
 * @brief  Keypad Keys enumeration
 */
typedef enum {
	KEYPAD_Button_0 = '0', /* Button 0 code */
	KEYPAD_Button_1 = '1', /* Button 1 code */
	KEYPAD_Button_2 = '2', /* Button 2 code */
	KEYPAD_Button_3 = '3', /* Button 3 code */
	KEYPAD_Button_4 = '4', /* Button 4 code */
	KEYPAD_Button_5 = '5', /* Button 5 code */
	KEYPAD_Button_6 = '6', /* Button 6 code */
	KEYPAD_Button_7 = '7', /* Button 7 code */
	KEYPAD_Button_8 = '8', /* Button 8 code */
	KEYPAD_Button_9 = '9', /* Button 9 code */
	KEYPAD_Button_A = 'A', /* Button A code */
	KEYPAD_Button_B = 'B', /* Button B code */
	KEYPAD_Button_C = 'C', /* Button C code */
	KEYPAD_Button_D = 'D', /* Button D code */
	KEYPAD_Button_STAR = '*', /* Button STAR code */
	KEYPAD_Button_HASH = '#', /* Button HASH code */
	KEYPAD_Button_NOT_PRESSED = '\0' /* No button pressed */
} TKEYPAD_Button;

/**
 * @brief This struct represents the keypad connected to the system.
 * @param buffer			keeps the pressed button in a short period of time
 * @param last_pressed_key 	keeps the last pressed key, useful for one byte reading
 * @param index				keeps track of the pressed buttons
 * @param timer			timer used for debouncing
 * @param last_pressed_time	used to check the time between different pressions
 * @param rows_pins			used to scan through the rows
 * @param cols_pins			used to scan through the columns
 */
typedef struct Keypad {
	TKEYPAD_Button buffer[KEYPAD_DEFAULT_BUFFER_SIZE];
	TKEYPAD_Button last_pressed_key;
	uint8_t index;
	TIM_HandleTypeDef *timer;
	uint32_t last_pressed_time;
	uint16_t rows_pins[ROWS_N];
	uint16_t cols_pins[COLUMNS_N];
} TKeypad;

/* Maps buttons to rows and columns number */
static const TKEYPAD_Button KEYS[ROWS_N][COLUMNS_N] = { { KEYPAD_Button_1,
		KEYPAD_Button_2, KEYPAD_Button_3, KEYPAD_Button_A }, { KEYPAD_Button_4,
		KEYPAD_Button_5, KEYPAD_Button_6, KEYPAD_Button_B }, { KEYPAD_Button_7,
		KEYPAD_Button_8, KEYPAD_Button_9, KEYPAD_Button_C },
		{ KEYPAD_Button_STAR, KEYPAD_Button_0, KEYPAD_Button_HASH,
				KEYPAD_Button_D } };

/**
 * @fn 		void KEYPAD_init_default(TKeypad *keypad)
 * @brief 	This function will initialize a keypad, using the default settings that are in the header file. Useful for single keypad.
 * 			More keypad could be added, but the configuration process is a bit different and needs another function.
 * @param 	keypad a pointer to the structure of the keyboard to initialize
 * @retval 	none
 */
void KEYPAD_init_default(TKeypad *keypad);

/**
 * @fn 				void KEYPAD_init_columns(TKeypad *keypad)
 * @brief 			This function will initialize all the columns, setting them to high state, in order to detect when a button is pressed
 * @param keypad 	a pointer to the structure of the keypad to initialize
 * @retval			None
 */
void KEYPAD_init_columns(TKeypad *keypad);

/**
 * @fn		void KEYPAD_key_pressed(TKeypad *keypad, uint16_t pin)
 * @brief 	ISR of the interrupt of the pins of the keypad. Should be called only by the irq.
 * 			This function will check that the last pression is in a short period of time,save the
 * 			corresponding row and starts a timer, in order to prevent bouncing.
 * @param 	keypad a pointer to the structure of the keyboard that has generated the interrupt
 * @param 	pin the pin that triggered the interrupt
 * @retval 	none
 */
void KEYPAD_key_pressed(TKeypad *keypad, uint16_t pin);

/**
 * @fn 		void KEYPAD_time_elapsed(TKeypad *keypad)
 * @brief 	ISR of the timer used to prevent bouncing (keypad is really bouncy:) ). Should be called only by the irq.
 * 			When the time is elapsed, the function will scan the columns and will read the last saved row. If it is valid,
 * 			the read button will be saved in a buffer. When the buffer is full, the function will restart the timer,
 * 			and the buffer will be checked later.
 * @param 	keypad a pointer to the structure of the keyboard that has started the timer
 * @retval	none
 */
void KEYPAD_time_elapsed(TKeypad *keypad);

/**
 * @fn 		void KEYPAD_check_buffer(uint8_t *buffer)
 * @brief 	ISR of the timer used to check the buffer. This is called only when the buffer size is full.
 * 			The buffer will be checked and, if the command is valid,it is executed. So it will enable and disable
 * 			the sensors and the system. This will also log on the console.
 * @param 	buffer a pointer to the buffer to be checked
 * @return none
 */
void KEYPAD_check_buffer(uint8_t *buffer);

/* Installed keypads should be declared here */
TKeypad KEYPAD_1;

#endif /* INC_KEYPAD_H_ */
