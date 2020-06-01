/*
 * keyboard.h
 *
 *  Created on: May 28, 2020
 *      Author: gioam
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "sys/_stdint.h"
#include "keypad_configuration.h"

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


const static KEYPAD_Button_t KEYS[ROWS_N][COLUMNS_N] = {
		{KEYPAD_Button_1, KEYPAD_Button_2, KEYPAD_Button_3, KEYPAD_Button_A},
		{KEYPAD_Button_4, KEYPAD_Button_5, KEYPAD_Button_6, KEYPAD_Button_B},
		{KEYPAD_Button_7, KEYPAD_Button_8, KEYPAD_Button_9, KEYPAD_Button_C},
		{KEYPAD_Button_STAR, KEYPAD_Button_0, KEYPAD_Button_HASH, KEYPAD_Button_D}
};



void KEYPAD_Init(void);

KEYPAD_Button_t KEYPAD_Read(void);

void KEYPAD_Update(uint16_t pin);

void KEYPAD_time_elapsed(void);

#endif /* INC_KEYPAD_H_ */
