/*
 *	This module contains methods to handle with a photoresistor
 */

#include "photoresistor.h"

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
		uint8_t alarm_duration, TIM_HandleTypeDef *htim,
		ADC_HandleTypeDef *hadc, TBuzzer *buzzer) {
	photoresistor->value = 0;
	photoresistor->alarm_delay = alarm_delay;
	photoresistor->counter = 0;
	photoresistor->alarm_duration = alarm_duration;
	photoresistor->state = ALARM_STATE_INACTIVE;
	photoresistor->htim = htim;
	photoresistor->hadc = hadc;
	photoresistor->buzzer = buzzer;
}

/*
 * @fn 			void photoresistor_activate(TPhotoresistor* photoresistor)
 * @brief  	 	activate the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 */
void photoresistor_activate(TPhotoresistor *photoresistor) {
	photoresistor_change_state(photoresistor, ALARM_STATE_ACTIVE);
}

/*
 * @fn 			void photoresistor_deactivate(TPhotoresistor* photoresistor)
 * @brief  	 	deactivate the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 */
void photoresistor_deactivate(TPhotoresistor *photoresistor) {
	photoresistor_change_state(photoresistor, ALARM_STATE_INACTIVE);
}

/*
 * @fn 			void photoresistor_get_string_state(TPhotoresistor *photoresistor, char *barrier_state)
 * @brief  	 	set in the barrier_state parameter the current state of the photoresistor
 * @param   	photoresistor: reference to the photoresistor variable
 * @param 		barrier_state: reference to the string where write the current state of photoresistor
 */
void photoresistor_get_string_state(TPhotoresistor *photoresistor,
		char *barrier_state) {
	switch (photoresistor->state) {
	case ALARM_STATE_INACTIVE:
		strncpy(barrier_state, "Inactive", 8);
		break;
	case ALARM_STATE_ACTIVE:
		strncpy(barrier_state, "Active", 6);
		break;
	case ALARM_STATE_DELAYED:
		strncpy(barrier_state, "Delayed", 7);
		break;
	case ALARM_STATE_ALARMED:
		strncpy(barrier_state, "Alarmed", 7);
		break;
	default:
		break;
	}
}
