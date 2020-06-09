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

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdlib.h>
#include <assert.h>

#include "stm32f4xx_hal.h"
#include "utils.h"
#include "bool.h"
#include "tim.h"

/* Period of the buzzer timer */
#define BUZZER_MAX_PULSE		(1000)

/* The shortest sound the buzzer can produce */
#define BUZZER_BEEP				(0.1 * BUZZER_MAX_PULSE)

/* A short sound the buzzer can produce */
#define BUZZER_SHORT_PULSE		(0.2 * BUZZER_MAX_PULSE)

/* A medium-length sound the buzzer can produce */
#define BUZZER_MEDIUM_PULSE		(0.6 * BUZZER_MAX_PULSE)

/* The longest sound the buzzer can produce */
#define BUZZER_LONG_PULSE		(0.9 * BUZZER_MAX_PULSE)

/* the duty cycle is in [0, 1[ so it's stored in a float variable */
typedef float TDutyCycle;

/* the integer value of the duty cycle is in [0, BUZZER_MAX_PULSE[ so it's stored in an integer variable */
typedef uint32_t TPulse;

/*
 * @brief	This struct represents the buzzer,
 * 			encapsulating the timer interface used to generate the PWM signal that control the buzzer.
 * @param	htim				pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 								used to generate the PWM signal that control the buzzer
 * @param	Channel				integer representing the timer cannel holding the PWM signal
 * @param	duty_cycle			the current duty cycle
 * @param	previous_pulse		the pulse corresponding to the previously set duty cycle
 * @param	pulse				the pulse corresponding to the current duty cycle
 * @param	single_pulse_mode	a boolean variable.
 * 								if TRUE, the buzzer will emit just a single sound;
 * 								otherwise, the buzzer will emit a period sound
 */
typedef struct {
	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	TDutyCycle duty_cycle;
	TPulse previous_pulse;
	TPulse pulse;
	bool single_pulse_mode;
} TBuzzer;

/*
 * @fn		void buzzer_init(TBuzzer *buzzer, TIM_HandleTypeDef *htim, uint32_t Channel)
 * @brief	Instantiates a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer to instantiate
 * @param	htim		pointer to the TIM_HandleTypeDef structure representing the timer interface
 * 						used to generate the PWM signal that control the buzzer
 * @param	Channel		the timer channel holding the PWM signal
 * @retval	pointer to the TBuzzer structure representing the buzzer
 */
void buzzer_init(TBuzzer *buzzer, TIM_HandleTypeDef *htim, uint32_t Channel);

/*
 * @fn		TDutyCycle buzzer_get_duty_cycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TDutyCycle buzzer_get_duty_cycle(TBuzzer *buzzer);

/*
 * @fn		TDutyCycle buzzer_get_duty_cycle(TBuzzer *buzzer)
 * @brief	Returns the duty cycle of a buzzer
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @retval	current duty cycle of the buzzer
 */
TPulse buzzer_get_pulse(TBuzzer *buzzer);


/*
 * @fn		void buzzer_set_duty_cycle(TBuzzer *buzzer, const TDutyCycle duty_cycle)
 * @brief	Sets the duty cycle of a buzzer to a specified value in [0, 1[,
 * 			without playing any sound
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	duty_cycle	duty cycle to set
 */
void buzzer_set_duty_cycle(TBuzzer *buzzer, const TDutyCycle duty_cycle);

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
void buzzer_set_pulse(TBuzzer *buzzer, const TPulse pulse);

/*
 * @fn		void buzzer_play_duty_cycle(TBuzzer *buzzer, TDutyCycle duty_cycle)
 * @brief	Makes a buzzer emit a sound for a certain ratio of the timer period.
 * 			For example, if the period is 1 second and duty_cycle is 0.4,
 * 			then the buzzer will emit a sound for 400 ms.
 * @param	buzzer			pointer to the TBuzzer structure representing the buzzer
 * @param	duty_cycle		duty cycle in [0, 1[ to set
 */
void buzzer_play_duty_cycle(TBuzzer *buzzer, TDutyCycle duty_cycle);

/*
 * @fn		void buzzer_play_pulse(TBuzzer *buzzer, TPulse pulse)
 * @brief	Makes a buzzer emit a sound with a certain pulse.
 * 			For example, if is 400, then the buzzer will emit a sound for 400 ms.
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	pulse		duty cycle pulse in [0, BUZZER_MAX_PULSE[ to set
 */
void buzzer_play_pulse(TBuzzer *buzzer, TPulse pulse);

/*
 * @fn		void buzzer_play_beep(TBuzzer *buzzer)
 * @brief	Makes a buzzer emit the shortest sound it can produce: it is not periodic
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 */
void buzzer_play_beep(TBuzzer *buzzer);

/*
 * @fn		void buzzer_stop(TBuzzer *buzzer)
 * @brief	Stops a buzzer, making it silent and stopping the PWM signal generation
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 */
void buzzer_stop(TBuzzer *buzzer);

/*
 * @fn		TPulse buzzer_beep()
 * @brief	Returns the pulse value of BUZZER_BEEP
 * @retval	BUZZER_BEEP as an integer
 */
TPulse buzzer_beep();

/*
 * @fn		TPulse buzzer_short_pulse()
 * @brief	Returns the pulse value of BUZZER_SHORT_PULSE
 * @retval	BUZZER_SHORT_PULSE as an integer
 */
TPulse buzzer_short_pulse();

/*
 * @fn		TPulse buzzer_beep()
 * @brief	Returns the pulse value of BUZZER_MEDIUM_PULSE
 * @retval	BUZZER_MEDIUM_PULSE as an integer
 */
TPulse buzzer_medium_pulse();

/*
 * @fn		TPulse buzzer_beep()
 * @brief	Returns the pulse value of BUZZER_LONG_PULSE
 * @retval	BUZZER_LONG_PULSE as an integer
 */
TPulse buzzer_long_pulse();

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
void buzzer_increase_pulse(TBuzzer *buzzer, TPulse next_pulse);

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
void buzzer_decrease_pulse(TBuzzer *buzzer, TPulse previous_pulse);

/*
 * @fn		static void buzzer_change_pulse(TBuzzer *buzzer, uint16_t pulse)
 * @brief	Re-initializes the timer channel holding the PWM signal, setting a specified pulse,
 * 			without playing any sound
 * @param	buzzer		pointer to the TBuzzer structure representing the buzzer
 * @param	pulse		duty cycle pulse in [0, BUZZER_MAX_PULSE[ to set
 */
static void buzzer_change_pulse(TBuzzer *buzzer, uint16_t pulse) {
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

/*
 * @fn		static TPulse get_pulse_of_duty_cycle(const TDutyCycle duty_cycle)
 * @brief	Converts a duty cycle float value in [0,1[ to the corresponding pulse value in [0, BUZZER_MAX_PULSE[
 * @param	dutyCycle	duty cycle value to convert
 * @retval	pulse value corresponding to duty_cycle
 */
static TPulse get_pulse_of_duty_cycle(const TDutyCycle duty_cycle) {
	return (TPulse) map(duty_cycle, 0.0, 1.0, 0, BUZZER_MAX_PULSE);
}

/*
 * @fn		static TDutyCycle get_duty_cycle_of_pulse(const TPulse pulse)
 * @brief	Converts a duty cycle integer value in [0, MAX_VALUE[ to the corresponding float value in [0,1[
 * @param	value	integer duty cycle value to convert
 * @retval	float duty cycle value corresponding to value
 */
static TDutyCycle get_duty_cycle_of_pulse(const TPulse pulse) {
	return map(pulse, 0, BUZZER_MAX_PULSE, 0.0, 1.0);
}

#endif /* INC_BUZZER_H_ */

