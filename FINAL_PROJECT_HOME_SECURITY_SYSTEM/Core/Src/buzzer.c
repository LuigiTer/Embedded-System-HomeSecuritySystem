/*
 * This module contains methods to handle with a buzzer, represented with a structure holding:
 * 		a pointer to the TIM_HandleTypeDef structure representing the timer generating the PWM signal
 * 			that controls the sound of the buzzer
 * 		the current duty cycle in [0, 1[
 */

#include "buzzer.h"

/*
 * @fn		TBuzzer* buzzerInit(TIM_HandleTypeDef *htim, uint32_t Channel)
 * @brief	Instantiates a buzzer
 * @param	htim		pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 						used to generate the PWM signal that control the buzzer
 * @param	Channel		the timer channel holding the PWM signal
 * @retval	pointer to the TBuzzer structure representing the buzzer
 */
TBuzzer* buzzerInit(TIM_HandleTypeDef *htim, uint32_t Channel) {
	HAL_TIM_PWM_Start(htim, Channel);

	TBuzzer *buzzer = NULL;
	buzzer = malloc(sizeof(buzzer));
	buzzer->htim = htim;
	buzzer->currentDutyCycle = 0;

	return buzzer;
}

/*
 * @fn		TDutyCycle getDutyCycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TDutyCycle getDutyCycle(TBuzzer *buzzer) {
	return buzzer->currentDutyCycle;
}

/*
 * @fn		TValue getDutyCycleValue(TBuzzer *buzzer)
 * @brief	Returns the integer duty cycle value of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current integer duty cycle value of the buzzer
 */
TValue getDutyCycleValue(TBuzzer *buzzer) {
	return getValueOfDutyCycle(getDutyCycle(buzzer));
}

/*
 * @fn		void setDutyCycle(TBuzzer *buzzer, const TDutyCycle dutyCycle)
 * @brief	Sets the duty cycle of a buzzer to a specified value in [0, 1[
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	dutyCycle	duty cycle to set
 */
void setDutyCycle(TBuzzer *buzzer, const TDutyCycle dutyCycle) {
	assert(0 <= dutyCycle && dutyCycle < 1);

	buzzer->htim->Instance->CCR1 = getValueOfDutyCycle(dutyCycle);
	buzzer->currentDutyCycle = dutyCycle;
}

/*
 * @fn		void setDutyCycleToValue(TBuzzer *buzzer, const TValue value)
 * @brief	Sets the duty cycle of a buzzer to a value corresponding to an integer value in [0, MAX_VALUE[
 * 			The conversion formula is dutyCycle = value / MAX_VALUE
 * 			For example, if value is 100, then the duty cycle will be 100 / 255 = 0,3922
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	value		integer duty cycle value to set
 */
void setDutyCycleToValue(TBuzzer *buzzer, const TValue value) {
	assert(0 <= value && value < MAX_VALUE);

	buzzer->htim->Instance->CCR1 = value;
	buzzer->currentDutyCycle = getDutyCycleOfValue(value);
}

/*
 * @fn		void increaseDutyCycle(TBuzzer *buzzer)
 * @brief	Sets the duty cycle of a buzzer to a new value depending on the current value
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	value		integer duty cycle value to set
 */
void increaseDutyCycle(TBuzzer *buzzer) {
	TValue value = getValueOfDutyCycle(buzzer->currentDutyCycle) + STEP;
	if (value >= MAX_VALUE)
		value = 0;
	setDutyCycleToValue(buzzer, value);
}
