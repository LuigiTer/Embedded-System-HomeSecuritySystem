/*
 * keypad.c
 *
 *  Created on: May 28, 2020
 *      Author: gioam
 */

#include "keypad.h"
#include "gpio.h"
#include "tim.h"

/* Private variable definition*/
static volatile KEYPAD_Button_t last_pressed_key = KEYPAD_Button_NOT_PRESSED;
static volatile uint8_t last_row;

static KEYPAD_Button_t t;
static const uint16_t COLUMNS_PINS[COLUMNS_N] = { COLUMN_1_PIN, COLUMN_2_PIN,
COLUMN_3_PIN, COLUMN_4_PIN, };
static const uint16_t ROWS_PINS[ROWS_N] = { ROW_1_PIN, ROW_2_PIN, ROW_3_PIN,
ROW_4_PIN, };

/* Functions definitions */

/**
 * @fn	   void KEYPAD_Init()
 * @brief  Initializes keypad functionality
 * @param  None
 * @retval None
 */
void KEYPAD_Init(void) {
	HAL_GPIO_WritePin(COLUMN_1_PORT, COLUMN_1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COLUMN_2_PORT, COLUMN_2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COLUMN_3_PORT, COLUMN_3_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COLUMN_4_PORT, COLUMN_4_PIN, GPIO_PIN_SET);

	last_pressed_key = KEYPAD_Button_NOT_PRESSED;
	t = KEYPAD_Button_NOT_PRESSED;
	last_row = ROWS_N;

	KEYPAD_TIMER.Init.Prescaler = KEYPAD_PRESCALER;
	KEYPAD_TIMER.Init.Period = DELAY_PERIOD;
	if (HAL_TIM_Base_Init(&KEYPAD_TIMER) != HAL_OK) {
		Error_Handler();
	}

	return;
}

/**
 * @fn     KEYPAD_Button_t KEYPAD_Read(void)
 * @brief  Returns the last valid pressed key on the keypad, if any.
 * @param  None
 * @retval The button pressed, if present #KEYPAD_Button_NOT_PRESSED
 */
KEYPAD_Button_t KEYPAD_Read(void) {
	t = last_pressed_key;
	last_pressed_key = KEYPAD_Button_NOT_PRESSED;
	return t;
}

/**
 * @fn     KEYPAD_Update(uint16_t pin)
 * @brief  Starts a timer, which will generate an interrupt and the pressed row will be checked to prevent bouncing. should be called only
 * by the interrupt
 * @param  the pin that generated the interrupt
 * @retval None
 */
void KEYPAD_Update(uint16_t pin) {

	//finding the row that generated the interrupt
	if (pin == ROWS_PINS[0]) {
		last_row = 0;
	} else if (pin == ROWS_PINS[1]) {
		last_row = 1;
	} else if (pin == ROWS_PINS[2]) {
		last_row = 2;
	} else if (pin == ROWS_PINS[3]) {
		last_row = 3;
	} else {
		// no row has been pressed, go back to isr
		last_row = ROWS_N;
		return;
	}

	//initializing the timer
	HAL_TIM_Base_Start_IT(&KEYPAD_TIMER);


}

/**
 * @fn     KEYPAD_time_elapsed(void)
 * @brief  ISR of the timer. This function will check on which column the button was pressed to prevent false detection.
 * @param  None
 * @retval None
 */
void KEYPAD_time_elapsed(void) {
	//stop the timer
	HAL_TIM_Base_Stop_IT(&KEYPAD_TIMER);

	// check that the last pressed row is valid
	if (last_row == ROWS_N) {
		return;
	}

	// finding the column which the button is connected to
	uint8_t col = 0;
	GPIO_PinState state;
	GPIO_PinState prev_state;
	for (; col < COLUMNS_N; col++) {
		prev_state = HAL_GPIO_ReadPin(ROW_1_PORT, ROWS_PINS[last_row]);
		HAL_GPIO_WritePin(COLUMN_1_PORT, COLUMNS_PINS[col], GPIO_PIN_RESET);
		state = HAL_GPIO_ReadPin(ROW_1_PORT, ROWS_PINS[last_row]);
		HAL_GPIO_WritePin(COLUMN_1_PORT, COLUMNS_PINS[col], GPIO_PIN_SET);
		if (state != prev_state) {
			break;
		}
	}

	// safety check
	if (col == COLUMNS_N) {
		return;
	}

	//reset the pending bit that has been generated while scanning. this instruction should not be executed right
	// after the possible interrupt or right before the return.
	__HAL_GPIO_EXTI_CLEAR_IT(ROWS_PINS[last_row]);

	// save the pressed key for the next reading and stop the timer
	last_pressed_key = KEYS[last_row][col];
	return;
}
