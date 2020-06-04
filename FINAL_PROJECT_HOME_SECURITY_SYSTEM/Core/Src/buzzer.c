/*
 * This module contains methods to handle with a buzzer, represented with a structure holding:
 * 		a pointer to the TIM_HandleTypeDef structure representing the timer generating the PWM signal
 * 			that controls the sound of the buzzer
 * 		the current duty cycle in [0, 1[
 */

#include "buzzer.h"

/*
 * @fn		TBuzzer* buzzer_init(TIM_HandleTypeDef *htim, uint32_t Channel)
 * @brief	Instantiates a buzzer
 * @param	htim		pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 						used to generate the PWM signal that control the buzzer
 * @param	Channel		the timer channel holding the PWM signal
 * @retval	pointer to the TBuzzer structure representing the buzzer
 */
TBuzzer* buzzer_init(TIM_HandleTypeDef *htim, uint32_t Channel) {
	HAL_TIM_PWM_Start(htim, Channel);

	TBuzzer *buzzer = NULL;
	buzzer = malloc(sizeof(buzzer));
	buzzer->htim = htim;
	buzzer->current_duty_cycle = 0;

	return buzzer;
}

/*
 * @fn		TDutyCycle get_duty_cycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TDutyCycle get_duty_cycle(TBuzzer *buzzer) {
	return buzzer->current_duty_cycle;
}

/*
 * @fn		TValue get_duty_cycle_value(TBuzzer *buzzer)
 * @brief	Returns the integer duty cycle value of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current integer duty cycle value of the buzzer
 */
TValue get_duty_cycle_value(TBuzzer *buzzer) {
	return get_value_of_duty_cycle(get_duty_cycle(buzzer));
}

/*
 * @fn		void set_duty_cycle(TBuzzer *buzzer, const TDutyCycle duty_cycle)
 * @brief	Sets the duty cycle of a buzzer to a specified value in [0, 1[
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	dutyCycle	duty cycle to set
 */
void set_duty_cycle(TBuzzer *buzzer, const TDutyCycle duty_cycle) {
	assert(0 <= duty_cycle && duty_cycle < 1);

	buzzer->htim->Instance->CCR1 = get_value_of_duty_cycle(duty_cycle);
	buzzer->current_duty_cycle = duty_cycle;
}

/*
 * @fn		void set_duty_cycle_to_value(TBuzzer *buzzer, const TValue value)
 * @brief	Sets the duty cycle of a buzzer to a value corresponding to an integer value in [0, MAX_VALUE[
 * 			The conversion formula is dutyCycle = value / MAX_VALUE
 * 			For example, if value is 100, then the duty cycle will be 100 / 255 = 0,3922
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	value		integer duty cycle value to set
 */
void set_duty_cycle_to_value(TBuzzer *buzzer, const TValue value) {
	assert(0 <= value && value < MAX_VALUE);

	buzzer->htim->Instance->CCR1 = value;
	buzzer->current_duty_cycle = get_duty_cycle_of_value(value);
}

/*
 * @fn		void increase_duty_cycle(TBuzzer *buzzer)
 * @brief	Sets the duty cycle of a buzzer to a new value depending on the current value
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	value		integer duty cycle value to set
 */
void increase_duty_cycle(TBuzzer *buzzer) {
	TValue value = get_value_of_duty_cycle(buzzer->current_duty_cycle) + STEP;
	if (value >= MAX_VALUE)
		value = 0;
	set_duty_cycle_to_value(buzzer, value);
}

/*
 * @fn		void set_sound_level(TBuzzer *buzzer, uint8_t level)
 * @brief	Sets the duty cycle of a buzzer to one of N_LEVELS different levels depending.
 * 			The conversion formula for the i-th level is dutyCycle = i / N_LEVELS
 * 			For example, if N_LEVELS is 5, the 2nd level of duty cycle is 2/5 = 0.4
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	level		the duty cycle level to set
 */
void set_sound_level(TBuzzer *buzzer, uint8_t level) {
	assert(0 <= level && level <= N_LEVELS);

	TValue value = MAX_VALUE * level / (N_LEVELS + 1);
	set_duty_cycle_to_value(buzzer, value);
}
