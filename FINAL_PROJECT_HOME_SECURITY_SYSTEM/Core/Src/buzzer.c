/*
 * This module contains methods to handle with a buzzer, represented with a structure holding:
 * 		a pointer to the TIM_HandleTypeDef structure representing the timer generating the PWM signal
 * 			that controls the sound of the buzzer
 * 		an integer representing the timer channel holding the PWM signal
 * 		the current duty cycle in [0, 1[
 * 		the pulse corresponding to the current duty cycle, depending on the timer period
 * 		the pulse corresponding to the previously set duty cycle
 * 		a boolean variable to make the buzzer emit just a single sound instead of a periodic one
 */

#include "buzzer.h"

/* buzzer used in the main program */
extern TBuzzer buzzer;

/*
 * @fn		TBuzzer* buzzer_init(TIM_HandleTypeDef *htim, uint32_t Channel)
 * @brief	Instantiates a buzzer
 * @param	htim		pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 						used to generate the PWM signal that control the buzzer
 * @param	Channel		the timer channel holding the PWM signal
 * @retval	pointer to the TBuzzer structure representing the buzzer
 */
void buzzer_init(TBuzzer *buzzer, TIM_HandleTypeDef *htim, uint32_t Channel) {
	buzzer->htim = htim;
	buzzer->Channel = Channel;
	buzzer->pulse = 0;
	buzzer->previous_pulse = 0;
	buzzer->duty_cycle = 0;

	return buzzer;
}

/*
 * @fn		TDutyCycle buzzer_get_duty_cycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TDutyCycle buzzer_get_duty_cycle(TBuzzer *buzzer) {
	return buzzer->duty_cycle;
}

/*
 * @fn		TDutyCycle buzzer_get_duty_cycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TPulse buzzer_get_pulse(TBuzzer *buzzer) {
	return buzzer->pulse;
}

/*
 * @fn		void buzzer_set_duty_cycle(TBuzzer *buzzer, const TDutyCycle duty_cycle)
 * @brief	Sets the duty cycle of a buzzer to a specified value in [0, 1[,
 * 			without playing any sound
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	duty_cycle	duty cycle to set
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
 * @fn		void buzzer_set_pulse(TBuzzer *buzzer, const TPulse pulse)
 * @brief	Sets the duty cycle of a buzzer to a value corresponding
 * 			to an integer value in [0, BUZZER_MAX_PULSE[, without playing any sound.
 * 			The conversion formula is duty_cycle = pulse / BUZZER_MAX_PULSE
 * 			For example, if pulse is 100 and BUZZER_MAX_PULSE is 255,
 * 			then the duty cycle will be 100 / 255 = 0.3922
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	pulse		duty cycle pulse to set
 */
void buzzer_set_pulse(TBuzzer *buzzer, const TPulse pulse) {
	assert(0 <= pulse && pulse < BUZZER_MAX_PULSE);

	buzzer_change_pulse(buzzer, pulse);
	buzzer->previous_pulse = buzzer->pulse;
	buzzer->pulse = pulse;
	buzzer->duty_cycle = get_duty_cycle_of_pulse(pulse);
}

/*
 * @fn		void buzzer_play_duty_cycle(TBuzzer *buzzer, TDutyCycle duty_cycle)
 * @brief	Makes a buzzer emit a sound for a certain ratio of the timer period.
 * 			For example, if the period is 1 second and duty_cycle is 0.4,
 * 			then the buzzer will emit a sound for 400 ms.
 * @param	buzzer			pointer to the TBuzzer structure representing the buzzer
 * @param	duty_cycle		duty cycle in [0, 1[ to set
 */
void buzzer_play_duty_cycle(TBuzzer *buzzer, TDutyCycle duty_cycle) {
	buzzer_set_duty_cycle(buzzer, duty_cycle);
	HAL_TIM_PWM_Start_IT(buzzer->htim, buzzer->Channel);
}

/*
 * @fn		void buzzer_play_pulse(TBuzzer *buzzer, TPulse pulse)
 * @brief	Makes a buzzer emit a sound with a certain pulse.
 * 			For example, if is 400, then the buzzer will emit a sound for 400 ms.
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	pulse		duty cycle pulse in [0, BUZZER_MAX_PULSE[ to set
 */
void buzzer_play_pulse(TBuzzer *buzzer, TPulse pulse) {
	buzzer_set_pulse(buzzer, pulse);
	HAL_TIM_PWM_Start_IT(buzzer->htim, buzzer->Channel);
}

/*
 * @fn		void buzzer_play_beep(TBuzzer *buzzer)
 * @brief	Makes a buzzer emit the shortest sound it can produce: it is not periodic
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 */
void buzzer_play_beep(TBuzzer *buzzer) {
	HAL_TIM_PWM_Stop_IT(buzzer->htim, buzzer->Channel);
	buzzer->single_pulse_mode = TRUE;
	buzzer_play_pulse(buzzer, buzzer_beep());
}

/*
 * @fn		void buzzer_stop(TBuzzer *buzzer)
 * @brief	Stops a buzzer, making it silent and stopping the PWM signal generation
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 */
void buzzer_stop(TBuzzer *buzzer) {
	buzzer_set_pulse(buzzer, 0);
	HAL_TIM_PWM_Stop_IT(buzzer->htim, buzzer->Channel);
	buzzer->single_pulse_mode = FALSE;
}

/*
 * @fn		TPulse buzzer_beep()
 * @brief	Returns the pulse value of BUZZER_BEEP
 * @retval	BUZZER_BEEP as an integer
 */
TPulse buzzer_beep() {
	return (TPulse) max(1, BUZZER_BEEP);
}

/*
 * @fn		TPulse buzzer_short_pulse()
 * @brief	Returns the pulse value of BUZZER_SHORT_PULSE
 * @retval	BUZZER_SHORT_PULSE as an integer
 */
TPulse buzzer_short_pulse() {
	return (TPulse) max(buzzer_beep(), BUZZER_SHORT_PULSE);
}

/*
 * @fn		TPulse buzzer_beep()
 * @brief	Returns the pulse value of BUZZER_MEDIUM_PULSE
 * @retval	BUZZER_MEDIUM_PULSE as an integer
 */
TPulse buzzer_medium_pulse() {
	return (TPulse) max(buzzer_beep(), BUZZER_MEDIUM_PULSE);
}

/*
 * @fn		TPulse buzzer_beep()
 * @brief	Returns the pulse value of BUZZER_LONG_PULSE
 * @retval	BUZZER_LONG_PULSE as an integer
 */
TPulse buzzer_long_pulse() {
	return (TPulse) max(buzzer_beep(), BUZZER_LONG_PULSE);
}

/*
 * @fn		void buzzer_increase_pulse(TBuzzer *buzzer, TPulse next_pulse)
 * @brief	Makes a buzzer emit a longer sound, depending on the previous pulse.
 * 			This function is effective only if pulse is in {0, BUZZER_SHORT_PULSE, BUZZER_MEDIUM_PULSE}
 * 			and next_pulse is in {BUZZER_SHORT_PULSE, BUZZER_MEDIUM_PULSE}.
 * 			In this case, if the buzzer is not emitting any sound, the next_pulse sound will be emitted;
 * 			otherwise, the buzzer will emit the longest sound it can produce
 * @param	buzzer			pointer to the TBuzzer structure representing the buzzer
 * @param	next_pulse		desidered duty cycle pulse in {0, BUZZER_SHORT_PULSE, BUZZER_MEDIUM_PULSE} to set
 */
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

/*
 * @fn		void buzzer_decrease_pulse(TBuzzer *buzzer, TPulse previous_pulse)
 * @brief	Makes a buzzer emit a shorter sound, depending on the previous pulse. This function is effective
 * 			only if pulse is in {BUZZER_SHORT_PULSE, BUZZER_MEDIUM_PULSE, BUZZER_LONG_PULSE}
 * 			and previous_pulse is in {BUZZER_SHORT_PULSE, BUZZER_MEDIUM_PULSE}.
 * 			In this case, if the buzzer is not emitting the longest sound it can produce, it will be stopped;
 * 			otherwise, the buzzer will emit a shorter sound
 * @param buzzer			pointer to the TBuzzer structure representing the buzzer
 * @param previous_pulse	duty cycle pulse that will be stopped
 */
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

/*
 * This callback is called every time a pulse is over.
 * If the buzzer is set to play just a single sound (beep),
 * the previous pulse will be set again after the beep is played
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {
		if (buzzer.single_pulse_mode) {
			TPulse previous_pulse = buzzer.previous_pulse;
			HAL_TIM_PWM_Stop_IT(buzzer.htim, buzzer.Channel);
			buzzer.previous_pulse = 0;
			buzzer.single_pulse_mode = FALSE;
			buzzer_play_pulse(&buzzer, previous_pulse);
		}
	}
}
