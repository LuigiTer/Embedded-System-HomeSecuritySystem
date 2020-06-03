/*
 * keyboard.h
 *
 *  Created on: May 28, 2020
 *      Author: gioam
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "keypad_configuration.h"
#include "tim.h"
#include "stdbool.h"
#include "pir_sensor.h"
#include "keypad.h"

/**
 * @brief  Keypad Keys enumeration
 */
typedef enum {
	KEYPAD_Button_0 = '0', /*!< Button 0 code */
	KEYPAD_Button_1 = '1', /*!< Button 1 code */
	KEYPAD_Button_2 = '2', /*!< Button 2 code */
	KEYPAD_Button_3 = '3', /*!< Button 3 code */
	KEYPAD_Button_4 = '4', /*!< Button 4 code */
	KEYPAD_Button_5 = '5', /*!< Button 5 code */
	KEYPAD_Button_6 = '6', /*!< Button 6 code */
	KEYPAD_Button_7 = '7', /*!< Button 7 code */
	KEYPAD_Button_8 = '8', /*!< Button 8 code */
	KEYPAD_Button_9 = '9', /*!< Button 9 code */
	KEYPAD_Button_A = 'A', /*!< Button A code */
	KEYPAD_Button_B = 'B', /*!< Button B code */
	KEYPAD_Button_C = 'C', /*!< Button C code */
	KEYPAD_Button_D = 'D', /*!< Button D code */
	KEYPAD_Button_STAR = '*', /*!< Button STAR code */
	KEYPAD_Button_HASH = '#', /*!< Button HASH code */
	KEYPAD_Button_NOT_PRESSED = '\0' /*!< No button pressed */
} KEYPAD_Button_t;

typedef struct Keypad {
	KEYPAD_Button_t buffer[DEFAULT_BUFFER_SIZE];
	KEYPAD_Button_t last_pressed_key;
	uint8_t index;
	TIM_HandleTypeDef *timer;
	uint32_t last_pressed_time;
	uint16_t rows_pins[ROWS_N];
	uint16_t cols_pins[COLUMNS_N];

} Keypad_t;

const static KEYPAD_Button_t KEYS[ROWS_N][COLUMNS_N] = { { KEYPAD_Button_1,
		KEYPAD_Button_2, KEYPAD_Button_3, KEYPAD_Button_A }, { KEYPAD_Button_4,
		KEYPAD_Button_5, KEYPAD_Button_6, KEYPAD_Button_B }, { KEYPAD_Button_7,
		KEYPAD_Button_8, KEYPAD_Button_9, KEYPAD_Button_C },
		{ KEYPAD_Button_STAR, KEYPAD_Button_0, KEYPAD_Button_HASH,
				KEYPAD_Button_D } };

Keypad_t KEYPAD_1;

 void KEYPAD_init_columns(Keypad_t *keypad);

void KEYPAD_Init_default(Keypad_t *keypad);
bool KEYPAD_buffer_read(Keypad_t *keypad, KEYPAD_Button_t *buffer);
void KEYPAD_clear_buffer(Keypad_t *keypad);
void KEYPAD_key_pressed(Keypad_t *keypad, uint16_t pin);
void KEYPAD_time_elapsed(Keypad_t *keypad);

#endif /* INC_KEYPAD_H_ */
