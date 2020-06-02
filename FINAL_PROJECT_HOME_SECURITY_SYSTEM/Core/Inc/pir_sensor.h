/*
 * pir_sensor.h
 *
 *  Created on: May 31, 2020
 *      Author: gioam
 */

#ifndef INC_PIR_SENSOR_H_
#define INC_PIR_SENSOR_H_

#include "stdbool.h"
#include "stm32f401xe.h"
#include "tim.h"

#define INACTIVE_STATE 		(0x0001)
#define ACTIVE_STATE		(0x0002)
#define ALARMED_STATE		(0x0004)
#define DELAYED_STATE		(0x0008)

#define NO_DELAY			(1)

#define PIR_TIMER 			htim9

typedef struct PIR_sensor {
	uint8_t delay;
	uint8_t remaining_delay;
	uint8_t state;
	uint8_t alarm_duration;
	IRQn_Type irq;
	GPIO_TypeDef *port;
	uint16_t pin;
} PIR_sensor_t;

void PIR_Time_elapsed();
void PIR_Sensor_handler(PIR_sensor_t *pir);
void PIR_sensor_deInit(PIR_sensor_t *pir);
void PIR_sensor_Init(PIR_sensor_t *pir, uint8_t delay,uint8_t alarm_duration, IRQn_Type irq,
		GPIO_TypeDef *port, uint16_t pin);
void PIR_Sensor_clear_alarm(PIR_sensor_t *pir);

#endif /* INC_PIR_SENSOR_H_ */
