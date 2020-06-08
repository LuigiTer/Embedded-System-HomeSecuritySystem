/*
 *	This module contains methods to handle with a photoresistor
 */

#ifndef INC_PHOTORESISTOR_H_
#define INC_PHOTORESISTOR_H_

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "buzzer.h"
#include "sensors_state.h"
#include "string.h"

/*Factor needed to be multiplied with the alarm duration/delay of the photoresistor in order check if the photoresistor
 * has reached the max time in alarmed/delayed state and then change its current state*/
#define ALARM_COUNTER_FACTOR		(20)

/*
 * @brief	This struct represents the phoresistor's attributes.
 * 			It stores value read, alarm_delay, alarm_duration, counter, state, htim, hadc and buzzer.
 * @param	value 				value read from the photoresistor
 * @param	alarm_delay			the delay of the photoresistor
 * @param	alarm_duration		the alarm duration of photoresistor
 * @param	counter				value that count the time passed in alarmed/delayed state and
 * 								when reach the the max alarm duration/delay the photoresistor change the state respectively in active/alarmed state
 * @param	state				current state of the sensor
 * @param	htim				the timer used by the photoresistor sensor
 * @param	hadc				the adc used by the photoresistor sensor
 * @param	buzzer				the buzzer associated to the photoresistor
 */
typedef struct {
	uint16_t value;
	uint8_t alarm_delay;
	uint8_t alarm_duration;
	uint16_t counter;
	TAlarmState state;
	TIM_HandleTypeDef *htim;
	ADC_HandleTypeDef *hadc;
	TBuzzer *buzzer;

} TPhotoresistor;

TPhotoresistor photoresistor1;

/*
 * @fn 			void photoresistor_init(TPhotoresistor *photoresistor, uint8_t alarm_delay,
										uint8_t alarm_duration, TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc, TBuzzer *buzzer);
 * @brief  		initialize the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 * @param   	alarm_delay: value of the alarm delay
 * @param   	alarm_duration: value of the alarm duration
 * @param   	htim: reference to the timer used for control the alarm duration
 * @param   	hadc: reference to the ADC who does the conversion
 * @param 		buzzer: reference to the buzzer associated to the photoresistor
 */
void photoresistor_init(TPhotoresistor *photoresistor, uint8_t alarm_delay,
		uint8_t alarm_duration, TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc, TBuzzer *buzzer);

/*
 * @fn 			void photoresistor_activate(TPhotoresistor* photoresistor)
 * @brief  	 	activate the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 */
void photoresistor_activate(TPhotoresistor *photoresistor);

/*
 * @fn 			void photoresistor_deactivate(TPhotoresistor* photoresistor)
 * @brief  	 	deactivate the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 */
void photoresistor_deactivate(TPhotoresistor *photoresistor);

/*
 * @fn 			static void photoresistor_change_state(TPhotoresistor *photoresistor, TAlarmState new_state)
 * @brief 		private function that change the state and threshold of ADC watchdog  associated with
 * 				the photoresitor according to the new state passed as parameter
 * @param		photoresistor: reference to the photoresistor variable
 * @param		new_state: the new state that the photoresistor must assume
 */
static void photoresistor_change_state(TPhotoresistor *photoresistor,
		TAlarmState new_state) {
	TPulse pulse = buzzer_short_pulse();
	switch (new_state) {
	case ALARM_STATE_INACTIVE:
		photoresistor->state = ALARM_STATE_INACTIVE;
		photoresistor->counter = 0;
		buzzer_decrease_pulse(photoresistor->buzzer, pulse);
		HAL_ADC_Stop_IT(photoresistor->hadc);
		HAL_TIM_Base_Stop_IT(photoresistor->htim);
		break;
	case ALARM_STATE_ACTIVE:
		photoresistor->state = ALARM_STATE_ACTIVE;
		photoresistor->counter = 0;
		buzzer_decrease_pulse(photoresistor->buzzer, pulse);
		photoresistor1.hadc->Instance->HTR = 2500; 	//detect low light level (with HTR) and
		photoresistor1.hadc->Instance->LTR = 0; 	//ignore high light level (with LTR) in order to detect an intruder
		HAL_TIM_Base_Start_IT(photoresistor->htim);
		break;
	case ALARM_STATE_DELAYED:
		photoresistor->state = ALARM_STATE_DELAYED;
		photoresistor1.hadc->Instance->HTR = 4095; 	// ignore low light level (HTR) because now we want to check
		photoresistor1.hadc->Instance->LTR = 800; 	// if the intruder go away (photoresistor read LTR value) before that the state change to alarmed
		photoresistor->counter = 0;
		buzzer_decrease_pulse(photoresistor->buzzer, pulse);
		break;
	case ALARM_STATE_ALARMED:
		photoresistor->state = ALARM_STATE_ALARMED;
		photoresistor1.hadc->Instance->HTR = 4095;  // here with HTR and LTR we ignore both low and high value of light beacuase
		photoresistor1.hadc->Instance->LTR = 0; 	// we are in alarmed state so the intruder is detected
		photoresistor->counter = 0;
		buzzer_increase_pulse(photoresistor->buzzer, pulse);
		HAL_ADC_Stop_IT(photoresistor->hadc);
		break;
	default:
		break;
	}
}

/*
 * @fn 			void photoresistor_get_string_state(TPhotoresistor *photoresistor, char *barrier_state)
 * @brief  	 	set in the barrier_state parameter the current state of the photoresistor
 * @param   	photoresistor: reference to the photoresistor variable
 * @param 		barrier_state: reference to the string where write the current state of photoresistor
 */
void photoresistor_get_string_state(TPhotoresistor *photoresistor,
		char *barrier_state);

#endif /* INC_PHOTORESISTOR_H_ */
