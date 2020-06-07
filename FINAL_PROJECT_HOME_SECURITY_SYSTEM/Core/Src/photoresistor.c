#include "photoresistor.h"

extern TBuzzer *buzzer;

/*
 * @fn 			void photoresistor_init(TPhotoresistor* photoresistor, uint8_t alarm_delay,
 * 											uint8_t alarm_duration, TIM_HandleTypeDef* htim, ADC_HandleTypeDef* hadc)
 * @brief  		initialize the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 * @param   	alarm_delay: value of the alarm delay
 * @param   	alarm_duration: value of the alarm duration
 * @param   	htim: reference to the timer used for control the alarm duration
 * @param   	hadc: reference to the ADC who does the conversion
 */
void photoresistor_init(TPhotoresistor *photoresistor, uint8_t alarm_delay,
		uint8_t alarm_duration, TIM_HandleTypeDef *htim,
		ADC_HandleTypeDef *hadc) {
	photoresistor->value = 0;
	photoresistor->alarm_delay = alarm_delay;
	photoresistor->counter = 0;
	photoresistor->alarm_duration = alarm_duration;
	photoresistor->state = ALARM_STATE_INACTIVE;
	photoresistor->htim = htim;
	photoresistor->hadc = hadc;
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
 * INACTIVE -> ACTIVE -> DELAYED -> ALARMED -> ACTIVE
 * INACTIVE <- ...
 */
void photoresistor_change_state(TPhotoresistor *photoresistor,
		TAlarmState new_state) {
	switch (new_state) {
	case ALARM_STATE_INACTIVE:
		photoresistor->state = ALARM_STATE_INACTIVE;
		photoresistor->counter = 0;
		set_sound_level(buzzer, 0);
		HAL_ADC_Stop_IT(photoresistor->hadc);
		HAL_TIM_Base_Stop_IT(photoresistor->htim);
		break;
	case ALARM_STATE_ACTIVE:
		photoresistor->state = ALARM_STATE_ACTIVE;
		photoresistor->counter = 0;
		set_sound_level(buzzer, 0);
		photoresistor1.hadc->Instance->HTR = 2500; //detect low light level, waiting for a thieg
		photoresistor1.hadc->Instance->LTR = 0; //ignore high light level, it's a sunny day
		HAL_TIM_Base_Start_IT(photoresistor->htim);
		break;
	case ALARM_STATE_DELAYED:
		photoresistor->state = ALARM_STATE_DELAYED;
		photoresistor1.hadc->Instance->HTR = 4095; //ignore low light level, we know we have a thief
		photoresistor1.hadc->Instance->LTR = 800; // we want to know only when the thief is gone
		photoresistor->counter = 0;
		set_sound_level(buzzer, 0);
		break;
	case ALARM_STATE_ALARMED:
		photoresistor->state = ALARM_STATE_ALARMED;
		photoresistor1.hadc->Instance->HTR = 4095;
		photoresistor1.hadc->Instance->LTR = 0; // ignore further trigger, we are in alarm
		photoresistor->counter = 0;
		set_sound_level(buzzer, 1);
		HAL_ADC_Stop_IT(photoresistor->hadc);
		break;
	default:
		break;
	}
}

/*
 * @fn 			void photoresistor_time_elapsed(TPhotoresistor* photoresistor)
 * @brief  	 	check if the period of the alarm high is elapsed
 * @param   	photoresistor: reference to the photoresistor variable
 */
/*
 void photoresistor_time_elapsed(TPhotoresistor *photoresistor) {

 static uint8_t counter = 0;



 photoresistor->remaining_delay -= 1;

 if (photoresistor->remaining_delay == 0) {
 photoresistor_change_state(photoresistor, ALARM_STATE_ALARMED);
 return;
 }

 // decreasing remaining delay will cause an underflow, so we sum it to the alarm duration (example 255 + 10 = 9 beacause uint8_t)
 // and wait until the total sum overflows the variable and then we have that the sum is zero so the alarm duration is elapsed
 uint8_t temp = photoresistor->remaining_delay + photoresistor->alarm_duration;
 if (temp == 0) {
 photoresistor_change_state(photoresistor, ALARM_STATE_ACTIVE);
 }
 }
 */
