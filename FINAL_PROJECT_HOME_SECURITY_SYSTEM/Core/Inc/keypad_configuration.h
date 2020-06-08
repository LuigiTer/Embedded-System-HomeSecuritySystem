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

/* For the timer clock, please refer to the configuration. */
/* Used in the default initialization process. */
#define KEYPAD_TIMER 					htim11

/* The clock source  used by the timer, this is used in order to calculate timings for debouncing. */
#define TIMER_CLOCK 					(42000000U)
#define KEYPAD_PRESCALER 				((TIMER_CLOCK/1000U) - 1U)
#define KEYPAD_DELAY_PERIOD   					(75U - 1U)

/* Follow the command protocol from keypad. */
#define KEYPAD_DEFAULT_BUFFER_SIZE		(7U)

/* This amount of time, expressed in milliseconds, sets how much time an user has in order to press the next button before the
 * buffer is reset. So, for example, if the buffer has a maximum size of 7, the user has less than 35 seconds to enter the seven
 * characters needed to fill the buffer. So, if a user enters the code, but forgets to press the last key, after 5 seconds,
 * everything is discarded. Default value is 5000ms*/
#define MAX_DELAY_BETWEEN_PRESSIONS  	(5000U)


#endif /* INC_KEYPAD_CONFIGURATION_H_ */
