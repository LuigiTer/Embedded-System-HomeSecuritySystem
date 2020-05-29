/*
 * keypad.c
 *
 *  Created on: May 28, 2020
 *      Author: gioam
 */

#include "keypad.h"
#include "gpio.h"

static volatile KEYPAD_Button_t last_pressed_key = KEYPAD_Button_NOT_PRESSED;

static KEYPAD_Button_t t;
static const uint16_t COLUMNS_PINS[COLUMNS_N] = { COLUMN_1_PIN, COLUMN_2_PIN,
COLUMN_3_PIN, COLUMN_4_PIN, };
static const uint16_t ROWS_PINS[ROWS_N] = { ROW_1_PIN, ROW_2_PIN, ROW_3_PIN,
ROW_4_PIN, };

//todo add ports

/* Functions definitions */

/**
 * @brief  Initializes keypad functionality
 * @param  None
 * @retval None
 */
void KEYPAD_Init() {
	HAL_GPIO_WritePin(COLUMN_1_PORT, COLUMN_1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COLUMN_2_PORT, COLUMN_2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COLUMN_3_PORT, COLUMN_3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COLUMN_4_PORT, COLUMN_4_PIN, GPIO_PIN_SET);
	return;

}

/**
 * @brief  Read the pressed key on the keypad
 * @param  None
 * @retval The read value, if present
 */
KEYPAD_Button_t KEYPAD_Read(void) {
	t = last_pressed_key;
	last_pressed_key = KEYPAD_Button_NOT_PRESSED;
	return t;

}



void KEYPAD_Update(uint16_t pin) {



	GPIO_PinState state;
	uint8_t col = 0;
	uint8_t row;

	//finding the row that generated the interrupt
	if (pin == ROWS_PINS[0]) {
		row = 0;
	} else if (pin == ROWS_PINS[1]) {
		row = 1;
	} else if (pin == ROWS_PINS[2]) {
		row = 2;
	} else if (pin == ROWS_PINS[3]) {
		row = 3;
	} else {
		// no row has been pressed, go back to isr
		return;
	}

	// finding the column which the button is connected to
	for (; col < COLUMNS_N; col++) {
		HAL_GPIO_WritePin(COLUMN_1_PORT, COLUMNS_PINS[col], GPIO_PIN_RESET);
		state = HAL_GPIO_ReadPin(ROW_1_PORT, ROWS_PINS[row]);
		HAL_GPIO_WritePin(COLUMN_1_PORT, COLUMNS_PINS[col], GPIO_PIN_SET);
		if (state == GPIO_PIN_RESET) {
			__HAL_GPIO_EXTI_CLEAR_IT(pin);
			break;
		}
	}

	// safety check
	if (col >= COLUMNS_N) {
		return;
	}

	// save the pressed key for the next reading.
	last_pressed_key = KEYS[row][col];
	return;
}
