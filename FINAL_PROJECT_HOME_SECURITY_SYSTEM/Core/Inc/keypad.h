#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "keypad_configuration.h"
#include "tim.h"
#include "bool.h"
#include "pir_sensor.h"
#include "photoresistor.h"
#include "logger.h"

//when the system is disabled, only the enable command can be accepted
#define SYSTEM_STATE_DISABLED 	(0X0001U)
#define SYSTEM_STATE_ENABLED 	(0X0002U)
#define SYSTEM_STATE_ALARMED 	(0X0004U)

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
 * @param *timer			timer used for debouncing
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

//maps buttons to rows and columns number
static const TKEYPAD_Button KEYS[ROWS_N][COLUMNS_N] = { { KEYPAD_Button_1,
		KEYPAD_Button_2, KEYPAD_Button_3, KEYPAD_Button_A }, { KEYPAD_Button_4,
		KEYPAD_Button_5, KEYPAD_Button_6, KEYPAD_Button_B }, { KEYPAD_Button_7,
		KEYPAD_Button_8, KEYPAD_Button_9, KEYPAD_Button_C },
		{ KEYPAD_Button_STAR, KEYPAD_Button_0, KEYPAD_Button_HASH,
				KEYPAD_Button_D } };

//used to enable or disable commands
static uint8_t system_state;


void KEYPAD_init_columns(TKeypad *keypad);
void KEYPAD_init_default(TKeypad *keypad);
void KEYPAD_key_pressed(TKeypad *keypad, uint16_t pin);
void KEYPAD_time_elapsed(TKeypad *keypad);
void KEYPAD_check_buffer(uint8_t *buffer);

//Installed keypads should be declared here
TKeypad KEYPAD_1;

#endif /* INC_KEYPAD_H_ */
