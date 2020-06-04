/*
 * This module contains methods to handle with a buzzer, represented with a structure holding:
 * 		a pointer to the TIM_HandleTypeDef structure representing the timer generating the PWM signal
 * 			that controls the sound of the buzzer
 * 		the current duty cycle in [0, 1[
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdlib.h>
#include <assert.h>

#include "stm32f4xx_hal.h"
#include "utils.h"

#define MAX_VALUE	(255)
#define STEP		(50)
#define N_LEVELS	(3)

// the duty cycle is in [0, 1[ so it's stored in a float variable
typedef float TDutyCycle;

// the integer value of the duty cycle is in [0, MAX_VALUE[ so it's stored in an integer variable
typedef uint32_t TValue;

/*
 * @brief	This struct represents the buzzer,
 * 			encapsulating the timer interface used to generate the PWM signal that control the buzzer.
 * @param	htim				pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 								used to generate the PWM signal that control the buzzer
 * @param	currentDutyCycle	the current duty cycle
 */
typedef struct {
	TIM_HandleTypeDef *htim;
	TDutyCycle currentDutyCycle;
} TBuzzer;

/*
 * @fn		TBuzzer* buzzerInit(TIM_HandleTypeDef *htim, uint32_t Channel)
 * @brief	Instantiates a buzzer
 * @param	htim		pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 						used to generate the PWM signal that control the buzzer
 * @param	Channel		the timer channel holding the PWM signal
 * @retval	pointer to the TBuzzer structure representing the buzzer
 */
TBuzzer* buzzerInit(TIM_HandleTypeDef *htim, uint32_t Channel);

/*
 * @fn		TDutyCycle getDutyCycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TDutyCycle getDutyCycle(TBuzzer *buzzer);

/*
 * @fn		TValue getDutyCycleValue(TBuzzer *buzzer)
 * @brief	Returns the integer duty cycle value of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current integer duty cycle value of the buzzer
 */
TValue getDutyCycleValue(TBuzzer *buzzer);

/*
 * @fn		void setDutyCycle(TBuzzer *buzzer, const TDutyCycle dutyCycle)
 * @brief	Sets the duty cycle of a buzzer to a specified value in [0, 1[
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	dutyCycle	duty cycle to set
 */
void setDutyCycle(TBuzzer *buzzer, const TDutyCycle dutyCycle);

/*
 * @fn		void setDutyCycleToValue(TBuzzer *buzzer, const TValue value)
 * @brief	Sets the duty cycle of a buzzer to a value corresponding to an integer value in [0, MAX_VALUE[
 * 			The conversion formula is dutyCycle = value / MAX_VALUE
 * 			For example, if value is 100, then the duty cycle will be 100 / 255 = 0,3922
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	value		integer duty cycle value to set
 */
void setDutyCycleToValue(TBuzzer *buzzer, const TValue value);

/*
 * @fn		void increaseDutyCycle(TBuzzer *buzzer)
 * @brief	Sets the duty cycle of a buzzer to a new value depending on the current value
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	value		integer duty cycle value to set
 */
void increaseDutyCycle(TBuzzer *buzzer);

/*
 * @fn		void setSoundLevel(TBuzzer *buzzer, uint8_t level)
 * @brief	Sets the duty cycle of a buzzer to one of N_LEVELS different levels depending.
 * 			The conversion formula for the i-th level is dutyCycle = i / N_LEVELS
 * 			For example, if N_LEVELS is 5, the 2nd level of duty cycle is 2/5 = 0.4
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	level		the duty cycle level to set
 */
void setSoundLevel(TBuzzer *buzzer, uint8_t level);

/*
 * @fn		static TValue getValueOfDutyCycle(const TDutyCycle dutyCycle)
 * @brief	Converts a duty cycle float value in [0,1[ to the corresponding interger value in [0, MAX_VALUE[
 * @param	dutyCycle	duty cycle value to convert
 * @retval	integer value corresponding to dutyCycle
 */
static TValue getValueOfDutyCycle(const TDutyCycle dutyCycle) {
	return (TValue) map(dutyCycle, 0.0, 1.0, 0, MAX_VALUE);
}

/*
 * @fn		static TDutyCycle getDutyCycleOfValue(const TValue value)
 * @brief	Converts a duty cycle integer value in [0, MAX_VALUE[ to the corresponding float value in [0,1[
 * @param	value	integer duty cycle value to convert
 * @retval	float duty cycle value corresponding to value
 */
static TDutyCycle getDutyCycleOfValue(const TValue value) {
	return map(value, 0, MAX_VALUE, 0.0, 1.0);
}

#endif /* INC_BUZZER_H_ */
