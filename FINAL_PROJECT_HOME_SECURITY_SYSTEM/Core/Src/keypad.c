/*
 * keypad.c
 *
 *  Created on: May 28, 2020
 *      Author: gioam
 */

#include "keypad.h"

/* Private variable definition*/
static volatile TKEYPAD_Button last_pressed_key = KEYPAD_Button_NOT_PRESSED;
static volatile uint8_t last_row;

/* Private function definition*/
/**
 * @brief This function will initialize all the columns, setting them to high, in order to detect pression
 * @param Keypad_t *keypad a pointer to the structure of the keypad to inizialize
 * @return none
 */
 void KEYPAD_init_columns(TKeypad *keypad) {
	for (uint8_t i = 0; i < COLUMNS_N; i++) {
		HAL_GPIO_WritePin(COLUMN_1_PORT, keypad->cols_pins[i], GPIO_PIN_SET);
	}
	return;
}

/* Functions definitions */
/**
 * @brief This function will initialize a keypad, using the settings that are in the header files. Useful for single keypad.
 * More keypad could be added, but the configuration process is a bit different.
 * @param Keypad_t *keypad a pointer to the structure of the keyboard to inizialize
 * @return none
 */
void KEYPAD_Init_default(TKeypad *keypad) {

	keypad->last_pressed_key = KEYPAD_Button_NOT_PRESSED;
	keypad->index = 0; //top of the buffer
	keypad->timer = &KEYPAD_TIMER; //pointer to the timer to use, defined in the header file
	keypad->last_pressed_time = 0;

	//setting up the structure f the pins. Please note that the porte used is the same for every pin.
	// if different ports are used, this library needs some changes
	keypad->rows_pins[0] = ROW_1_PIN;
	keypad->rows_pins[1] = ROW_2_PIN;
	keypad->rows_pins[2] = ROW_3_PIN;
	keypad->rows_pins[3] = ROW_4_PIN;

	keypad->cols_pins[0] = COLUMN_1_PIN;
	keypad->cols_pins[1] = COLUMN_2_PIN;
	keypad->cols_pins[2] = COLUMN_3_PIN;
	keypad->cols_pins[3] = COLUMN_4_PIN;

	KEYPAD_init_columns(keypad);

	//setting up the timer, so even if it is not configured via gui, it is done directly here.
	KEYPAD_TIMER.Init.Prescaler = KEYPAD_PRESCALER;
	KEYPAD_TIMER.Init.Period = DELAY_PERIOD;

	return;
}


/**
 * @brief This function will copy the buffer of the keypad into the destination buffer, without cleaning the keypad buffer.
 * @param Keypad_t *keypad a pointer to the structure of the keyboard to read from
 * @param[out]  KEYPAD_Button_t *buffer a pointer to the buffer where the keypad buffer will be copied
 * @return true if the buffer was modified, false otherwise
 */
bool KEYPAD_buffer_read(TKeypad *keypad, TKEYPAD_Button *buffer) {
	if (keypad->index == DEFAULT_BUFFER_SIZE) {
		for (uint8_t i = 0; i < DEFAULT_BUFFER_SIZE; i++) {
			buffer[i] = keypad->buffer[i];
		}
		return true;
	}
	return false;
}


/**
 * @brief This function will clear the buffer of the keypad. Note that the clear process consists in just resetting the index
 * @param Keypad_t *keypad a pointer to the structure of the keyboard which needs the buffer cleared.
 * @return none
 */
void KEYPAD_clear_buffer(TKeypad *keypad) {
	keypad->index = 0;
}

/**
 * @brief ISR of the interrupt of the pins of the keypad. Should be called only by the irq
 * @param Keypad_t *keypad a pointer to the structure of the keyboard that has generated the interrupt
 * @param uint16_t pin the pin that triggered the interruption
 * @return none
 */
void KEYPAD_key_pressed(TKeypad *keypad, uint16_t pin) {
	if ((HAL_GetTick() - keypad->last_pressed_time)
			> MAX_DELAY_BETWEEN_PRESSIONS) {
		//if the last pressed valid button was a long time ago, discard everything
		keypad->index = 0;
	}

	// Now that the buffer is validated, check if there is space.
	if (keypad->index >= DEFAULT_BUFFER_SIZE) {
		// if not, return. user must wait MAX_DELAY_BETWEEN_PRESSIONS to cancel everything typed
		return;
	}

	//finding the row that generated the interrupt
	if (pin == keypad->rows_pins[0]) {
		last_row = 0;
	} else if (pin == keypad->rows_pins[1]) {
		last_row = 1;
	} else if (pin == keypad->rows_pins[2]) {
		last_row = 2;
	} else if (pin == keypad->rows_pins[3]) {
		last_row = 3;
	} else {
		// no row has been pressed, go back to isr
		last_row = ROWS_N;
		return;
	}

	//initializing the timer
	HAL_TIM_Base_Stop_IT(keypad->timer);
	KEYPAD_TIMER.Instance->CNT = 0;
	HAL_TIM_Base_Start_IT(keypad->timer);
	return;
}

/**
 * @brief ISR of the timer used to prevent bouncing (keypad is really bouncy:) ). Should be called only by the irq
 * @param Keypad_t *keypad a pointer to the structure of the keyboard that has started the timer
 * @return none
 */
void KEYPAD_time_elapsed(TKeypad *keypad) {
	//stop the timer
	HAL_TIM_Base_Stop_IT(keypad->timer);

	// check that the last pressed row is valid
	if (last_row == ROWS_N) {
		return;
	}

	// finding the column which the button is connected to
	uint8_t col = 0;
	GPIO_PinState state;
	GPIO_PinState prev_state;
	for (; col < COLUMNS_N; col++) {
		prev_state = HAL_GPIO_ReadPin(ROW_1_PORT, keypad->rows_pins[last_row]);
		HAL_GPIO_WritePin(COLUMN_1_PORT, keypad->cols_pins[col],
				GPIO_PIN_RESET);
		state = HAL_GPIO_ReadPin(ROW_1_PORT, keypad->rows_pins[last_row]);
		HAL_GPIO_WritePin(COLUMN_1_PORT, keypad->cols_pins[col], GPIO_PIN_SET);
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
	__HAL_GPIO_EXTI_CLEAR_IT(keypad->rows_pins[last_row]);

	//now save the pressed key, the time and increase buffer
	keypad->buffer[keypad->index++] = KEYS[last_row][col];
	keypad->last_pressed_time = HAL_GetTick();
	return;
}

