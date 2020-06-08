#ifndef INC_PHOTORESISTOR_H_
#define INC_PHOTORESISTOR_H_

#include "stm32f4xx_hal.h"
#include "adc.h"
#include "buzzer.h"
#include "sensors_state.h"
#include "string.h"

#define MAX_AREA_RANGE 				(3000)
#define ALARM_COUNTER_FACTOR		(20)

typedef struct {
	uint16_t value;
	uint8_t alarm_delay;
	uint8_t alarm_duration;
	uint16_t counter;
	TAlarmState state;
	TIM_HandleTypeDef *htim;
	ADC_HandleTypeDef *hadc;

} TPhotoresistor;

TPhotoresistor photoresistor1;

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
		ADC_HandleTypeDef *hadc);

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

void photoresistor_change_state(TPhotoresistor *photoresistor,
		TAlarmState new_state);

void photoresistor_get_string_state(TPhotoresistor *photoresistor,
		char *barrier_state);

#endif /* INC_PHOTORESISTOR_H_ */
