/*
 * keypad_configuration.h
 *
 *  Created on: May 28, 2020
 *      Author: gioam
 */

#ifndef INC_KEYPAD_CONFIGURATION_H_
#define INC_KEYPAD_CONFIGURATION_H_

/* Number of columns and rows of the keypad*/
#define COLUMNS_N 		(4)
#define ROWS_N 			(4)

/* Column 1 default */
#define COLUMN_1_PORT  	GPIOC
#define COLUMN_1_PIN  	GPIO_PIN_0
/* Column 2 default */
#define COLUMN_2_PORT  	GPIOC
#define COLUMN_2_PIN  	GPIO_PIN_1
/* Column 3 default */
#define COLUMN_3_PORT  	GPIOC
#define COLUMN_3_PIN  	GPIO_PIN_2
/* Column 4 default */
#define COLUMN_4_PORT  	GPIOC
#define COLUMN_4_PIN  	GPIO_PIN_3

/* Row 1 default */
#define ROW_1_PORT  	GPIOB
#define ROW_1_PIN  		GPIO_PIN_12
/* Row 2 default */
#define ROW_2_PORT  	GPIOB
#define ROW_2_PIN  		GPIO_PIN_13
/* Row 3 default */
#define ROW_3_PORT  	GPIOB
#define ROW_3_PIN 	 	GPIO_PIN_14
/* Row 4 default */
#define ROW_4_PORT  	GPIOB
#define ROW_4_PIN  		GPIO_PIN_15

/* For the timer clock, please refer to the configuration*/
#define KEYPAD_TIMER 	htim11
#define TIMER_CLOCK 	16000000
#define KEYPAD_PRESCALER ((TIMER_CLOCK/1000) - 1)
#define DELAY_PERIOD   (150 - 1)

#endif /* INC_KEYPAD_CONFIGURATION_H_ */
