#include "photoresistor.h"

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
void photoresistor_init(TPhotoresistor* photoresistor, uint8_t alarm_delay, uint8_t alarm_duration, TIM_HandleTypeDef* htim, ADC_HandleTypeDef* hadc) {
	photoresistor->value = 0;
	photoresistor->alarm_delay = alarm_delay;
	photoresistor->remaining_delay = alarm_delay;
	photoresistor->alarm_duration = alarm_duration;
	photoresistor->state = ALARM_STATE_ACTIVE;
	photoresistor->htim = htim;
	photoresistor->hadc = hadc;
}

/*
 * @fn 			void photoresistor_activate(TPhotoresistor* photoresistor)
 * @brief  	 	activate the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 */
void photoresistor_activate(TPhotoresistor* photoresistor){
	photoresistor->state = ALARM_STATE_ACTIVE;
	photoresistor->remaining_delay = photoresistor->alarm_delay;
	HAL_ADC_Start_DMA(photoresistor->hadc, &photoresistor->value, 1);
}

/*
 * @fn 			void photoresistor_deactivate(TPhotoresistor* photoresistor)
 * @brief  	 	deactivate the photoresistor module
 * @param   	photoresistor: reference to the photoresistor variable
 */
void photoresistor_deactivate(TPhotoresistor* photoresistor){
	photoresistor->state = ALARM_STATE_INACTIVE;
	HAL_TIM_OC_Stop_IT(photoresistor->htim, TIM_CHANNEL_1);
	HAL_ADC_Stop_DMA(photoresistor->hadc);
}

/*
 * @fn 			void photoresistor_time_elapsed(TPhotoresistor* photoresistor)
 * @brief  	 	check if the period of the alarm high is elapsed
 * @param   	photoresistor: reference to the photoresistor variable
 */
void photoresistor_time_elapsed(TPhotoresistor* photoresistor){
	photoresistor->remaining_delay -=1;

	if (photoresistor->remaining_delay == 0){
		photoresistor->state = ALARM_STATE_ALARMED;
		return;
	}

	uint8_t temp = photoresistor->remaining_delay + photoresistor->alarm_duration;
	if (temp == 0){
		HAL_TIM_OC_Stop_IT(photoresistor->htim, TIM_CHANNEL_1);
		photoresistor->state = ALARM_STATE_ACTIVE;
		photoresistor->remaining_delay = photoresistor->alarm_delay;
	}
}
