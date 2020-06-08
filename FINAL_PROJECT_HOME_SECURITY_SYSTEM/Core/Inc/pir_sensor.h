/*
 * pir_sensor.h
 *
 *  Created on: May 31, 2020
 *      Author: gioam
 */

#ifndef INC_PIR_SENSOR_H_
#define INC_PIR_SENSOR_H_

#include "bool.h"
#include "stm32f401xe.h"
#include "tim.h"
#include "gpio.h"
#include "sensors_state.h"
#include "buzzer.h"
#include "string.h"



/*
#define INACTIVE_STATE 		(0x0001)
#define ACTIVE_STATE		(0x0002)
#define ALARMED_STATE		(0x0004)
#define DELAYED_STATE		(0x0008)
*/

#define NO_DELAY			(1U)

#define DEFAULT_PIR_TIMER 			htim9

typedef struct PIR_sensor {
	uint8_t delay; 				// the delay of the sensor
	uint8_t remaining_delay;	// counter of the remaining delay
	uint8_t state;				// current state of the sensor
	uint8_t alarm_duration;		// the alarm duration. remainingg duration isn't necessary
	IRQn_Type irq;				// the IRQn which is dedicated to the sensor
	GPIO_TypeDef *port;			// the port which the sensor is connected to
	uint16_t pin;				// the pin which the sensor is connected to
	TIM_HandleTypeDef *timer; 	// the timer used by the pir sensor
	TBuzzer *buzzer;
} TPIR_sensor;

static inline void PIR_Change_State(TPIR_sensor *pir, TAlarmState state) {
	switch (state) {
	case ALARM_STATE_INACTIVE:
		HAL_TIM_OC_Stop_IT(pir->timer, TIM_CHANNEL_1);
		HAL_NVIC_DisableIRQ(pir->irq);
		set_sound_level(pir->buzzer, 0);
		break;
	case ALARM_STATE_ACTIVE:
		HAL_TIM_OC_Stop_IT(pir->timer, TIM_CHANNEL_1);
		HAL_NVIC_EnableIRQ(pir->irq);
		pir->remaining_delay = pir->delay;
		set_sound_level(pir->buzzer, 0);
		if(HAL_GPIO_ReadPin(pir->port, pir->pin) == GPIO_PIN_SET){
			HAL_NVIC_SetPendingIRQ(pir->irq);
		}
		break;
	case ALARM_STATE_DELAYED:
		pir->timer->Instance->CNT = 0;
		HAL_TIM_OC_Start_IT(pir->timer, TIM_CHANNEL_1);
		pir->remaining_delay = pir->delay;
		set_sound_level(pir->buzzer, 0);
		break;
	case ALARM_STATE_ALARMED:
		set_sound_level(pir->buzzer, 2);
		break;
	default:
		break;
	}
	pir->state = state;
}

void PIR_Time_elapsed();
void PIR_Sensor_handler(TPIR_sensor *pir);
void PIR_sensor_deactivate(TPIR_sensor *pir);
void PIR_sensor_activate(TPIR_sensor *pir);
void PIR_sensor_Init(TPIR_sensor *pir, uint8_t delay,uint8_t alarm_duration, IRQn_Type irq,
		GPIO_TypeDef *port, uint16_t pin,TIM_HandleTypeDef *timer, TBuzzer *buzzer);
void PIR_get_string_state(TPIR_sensor *pir, char *area_state);



TPIR_sensor PIR_4;


#endif /* INC_PIR_SENSOR_H_ */
