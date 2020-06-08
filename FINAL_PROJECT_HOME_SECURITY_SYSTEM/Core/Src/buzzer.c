/*
 * This module contains methods to handle with a buzzer, represented with a structure holding:
 * 		a pointer to the TIM_HandleTypeDef structure representing the timer generating the PWM signal
 * 			that controls the sound of the buzzer
 * 		the current duty cycle in [0, 1[
 */

#include "buzzer.h"

extern TBuzzer *buzzer;

/*
 * @fn		TBuzzer* buzzer_init(TIM_HandleTypeDef *htim, uint32_t Channel)
 * @brief	Instantiates a buzzer
 * @param	htim		pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 						used to generate the PWM signal that control the buzzer
 * @param	Channel		the timer channel holding the PWM signal
 * @retval	pointer to the TBuzzer structure representing the buzzer
 */
TBuzzer* buzzer_init(TIM_HandleTypeDef *htim, uint32_t Channel) {
	TBuzzer *buzzer = NULL;
	buzzer = malloc(sizeof(buzzer));
	buzzer->htim = htim;
	buzzer->Channel = Channel;
	buzzer->pulse = 0;
	buzzer->previous_pulse = 0;
	buzzer->duty_cycle = 0;

	return buzzer;
}

/*
 * @fn		TDutyCycle get_duty_cycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TDutyCycle buzzer_get_duty_cycle(TBuzzer *buzzer) {
	return buzzer->duty_cycle;
}

/*
 * @fn		TValue get_duty_cycle_value(TBuzzer *buzzer)
 * @brief	Returns the integer duty cycle value of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current integer duty cycle value of the buzzer
 */
TPulse buzzer_get_pulse(TBuzzer *buzzer) {
	return buzzer->pulse;
}

/*
 * @fn		void set_duty_cycle(TBuzzer *buzzer, const TDutyCycle duty_cycle)
 * @brief	Sets the duty cycle of a buzzer to a specified value in [0, 1[
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	dutyCycle	duty cycle to set
 */
void buzzer_set_duty_cycle(TBuzzer *buzzer, const TDutyCycle duty_cycle) {
	assert(0 <= duty_cycle && duty_cycle < 1);

	TPulse pulse = get_pulse_of_duty_cycle(duty_cycle);
	buzzer_change_pulse(buzzer, pulse);
	buzzer->previous_pulse = buzzer->pulse;
	buzzer->pulse = pulse;
	buzzer->duty_cycle = duty_cycle;
}

/*
 * @fn		void set_duty_cycle_to_value(TBuzzer *buzzer, const TValue value)
 * @brief	Sets the duty cycle of a buzzer to a value corresponding to an integer value in [0, MAX_VALUE[
 * 			The conversion formula is dutyCycle = value / MAX_VALUE
 * 			For example, if value is 100, then the duty cycle will be 100 / 255 = 0,3922
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	value		integer duty cycle value to set
 */
void buzzer_set_pulse(TBuzzer *buzzer, const TPulse pulse) {
	assert(0 <= pulse && pulse < BUZZER_MAX_PULSE);

	buzzer_change_pulse(buzzer, pulse);
	buzzer->previous_pulse = buzzer->pulse;
	buzzer->pulse = pulse;
	buzzer->duty_cycle = get_duty_cycle_of_pulse(pulse);
}

void buzzer_play_duty_cycle(TBuzzer *buzzer, TDutyCycle duty_cycle) {
	buzzer_set_duty_cycle(buzzer, duty_cycle);
	HAL_TIM_PWM_Start_IT(buzzer->htim, buzzer->Channel);
}

void buzzer_play_pulse(TBuzzer *buzzer, TPulse pulse) {
	buzzer_set_pulse(buzzer, pulse);
	HAL_TIM_PWM_Start_IT(buzzer->htim, buzzer->Channel);
}

void buzzer_play_beep(TBuzzer *buzzer) {
	HAL_TIM_PWM_Stop_IT(buzzer->htim, buzzer->Channel);
	buzzer->single_pulse_mode = TRUE;
	buzzer_play_pulse(buzzer, buzzer_beep());
}

void buzzer_stop(TBuzzer *buzzer) {
	buzzer_set_pulse(buzzer, 0);
	HAL_TIM_PWM_Stop_IT(buzzer->htim, buzzer->Channel);
	buzzer->single_pulse_mode = FALSE;
}

void buzzer_increase_pulse(TBuzzer *buzzer, TPulse next_pulse) {
	TPulse current_pulse = buzzer->pulse;

	if (current_pulse == 0) {
		buzzer_play_pulse(buzzer, next_pulse);
	} else if (current_pulse == buzzer_short_pulse()) {
		if (next_pulse == buzzer_medium_pulse()) {
			buzzer_play_pulse(buzzer, buzzer_long_pulse());
		}
	} else if (current_pulse == buzzer_medium_pulse()) {
		if (next_pulse == buzzer_short_pulse()) {
			buzzer_play_pulse(buzzer, buzzer_long_pulse());
		}
	}
}

void buzzer_decrease_pulse(TBuzzer *buzzer, TPulse previous_pulse) {
	TPulse current_pulse = buzzer->pulse;

	if (current_pulse == buzzer_short_pulse()) {
		if (previous_pulse == buzzer_short_pulse()) {
			buzzer_stop(buzzer);
		}
	} else if (current_pulse == buzzer_medium_pulse()) {
		if (previous_pulse == buzzer_medium_pulse()) {
			buzzer_stop(buzzer);
		}
	} else if (current_pulse == buzzer_long_pulse()) {
		if (previous_pulse == buzzer_short_pulse()) {
			buzzer_play_pulse(buzzer, buzzer_medium_pulse());
		}
		if (previous_pulse == buzzer_medium_pulse()) {
			buzzer_play_pulse(buzzer, buzzer_short_pulse());
		}
	}
}

TPulse buzzer_beep() {
	return (TPulse) max(1, BUZZER_BEEP);
}

TPulse buzzer_short_pulse() {
	return (TPulse) max(buzzer_beep(), BUZZER_SHORT_PULSE);
}

TPulse buzzer_medium_pulse() {
	return (TPulse) max(buzzer_beep(), BUZZER_MEDIUM_PULSE);
}

TPulse buzzer_long_pulse() {
	return (TPulse) max(buzzer_beep(), BUZZER_LONG_PULSE);
}

void buzzer_change_pulse(TBuzzer *buzzer, uint16_t pulse) {
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_HandleTypeDef *htim = buzzer->htim;
	uint32_t Channel = buzzer->Channel;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pulse;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, Channel) != HAL_OK) {
		Error_Handler();
	}
	HAL_TIM_MspPostInit(htim);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {
		if (buzzer->single_pulse_mode) {
			TPulse previous_pulse = buzzer->previous_pulse;
			HAL_TIM_PWM_Stop_IT(buzzer->htim, buzzer->Channel);
			buzzer->previous_pulse = 0;
			buzzer->single_pulse_mode = FALSE;
			buzzer_play_pulse(buzzer, previous_pulse);
		}
	}
}
