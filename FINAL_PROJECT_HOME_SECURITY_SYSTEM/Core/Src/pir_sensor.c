/*
 * This module is used to represent the PIR sensor. Offers a structure to configure more than one on the same system and
 * functions to enable or disable it. The activation process should be called when the user enable the sensor, while the
 * deactivation process when the system or the sensor is disabled.
 *
 */

#include "pir_sensor.h"

/**
 * @fn 		PIR_sensor_init(TPIR_sensor *pir, uint8_t delay, uint8_t alarm_duration,
							IRQn_Type irq, GPIO_TypeDef *port, uint16_t pin,
							TIM_HandleTypeDef *timer, TBuzzer *buzzer)
 * @brief Initialize a pir sensor with the given parameters.
 * @param pir				the structure which will hold the sensor
 * @param delay				the alarm_delay in second. If zero, this will be set to NO_DELAY value
 * @param alarm_duration 	for how much time in seconds the sensor will be in alarm state
 * @param irq		 		the irq corresponding to the port of the pir sensor
 * @param port				the port which the sensor is connected to
 * @param pin				the port which the sensor is connected to
 * @param timer 			the timer dedicated to the sensor
 * @param buzzer			the buzzer associated to the sensor
 * @retval None
 */
void PIR_sensor_init(TPIR_sensor *pir, uint8_t delay, uint8_t alarm_duration,
		IRQn_Type irq, GPIO_TypeDef *port, uint16_t pin,
		TIM_HandleTypeDef *timer, TBuzzer *buzzer) {

	if(delay == 0){
		delay = NO_DELAY;
	}

	pir->state = ALARM_STATE_INACTIVE;
	pir->alarm_delay = delay;
	pir->remaining_delay = delay;
	pir->alarm_duration = alarm_duration;
	pir->irq = irq;
	pir->pin = pin;
	pir->port = port;
	pir->timer = timer;
	pir->buzzer = buzzer;
	return;
}

/**
 * @fn 			void PIR_sensor_activate(TPIR_sensor *pir)
 * @brief		Reactivate and set as active a pir sensor using the previous configuration
 * @param pir	the structure which holds the sensor to be activated
 * @retval 		None
 */
void PIR_sensor_activate(TPIR_sensor *pir) {
	PIR_change_state(pir, ALARM_STATE_ACTIVE);
	return;
}

/**
 * @fn 			 void PIR_sensor_deactivate(TPIR_sensor *pir)
 * @brief		 Deactivate and set as inactive a pir sensor. NOTE: also the irq and the timer will be disabled
 * @param pir	 the structure of the pin to deactivate
 * @retval 		 None
 */
void PIR_sensor_deactivate(TPIR_sensor *pir) {
	PIR_change_state(pir, ALARM_STATE_INACTIVE);
	return;
}


/**
 * @fn 			PIR_sensor_handler(TPIR_sensor *pir)
 * @brief 		ISR of the pir_sensor. This should be called both on rising and falling edge.
 * @param pir 	the structure of the pin which has triggered the interruption
 * @retval		None
 */
void PIR_sensor_handler(TPIR_sensor *pir) {
	__HAL_GPIO_EXTI_CLEAR_IT(pir->pin);

	//If the interrupt is on the falling edge, then check the state of the sensor
	if (HAL_GPIO_ReadPin(pir->port, pir->pin) == GPIO_PIN_RESET) {
		if (pir->state == ALARM_STATE_DELAYED) {
			//if the sensor was on delayed state, stop the timer and set the sensor back to active
			PIR_change_state(pir, ALARM_STATE_ACTIVE);
		}
		//in any other case, ignore the interrupt. There are 3 other possible states
		// active: it is not possible since if it is falling, it has raised, so it was already in delayed
		// inactive: impossible, since the interrupt was deactivated
		// alarm: ignore it, the sensor is already alarmed
		return;
	}

	// if it is rising, but it is already alarmed, don't do anything
	if (pir->state == ALARM_STATE_ALARMED) {
		return;
	}

	PIR_change_state(pir, ALARM_STATE_DELAYED);

}

/**
 * @fn			PIR_time_elapsed(TPIR_sensor *pir)
 * @brief 		ISR of the associated timer. This will reduce the remaing delay or the duration of the alarm
 * @param pir 	the structure of the pin which is currently on alarm or delayed state
 * @retval 		None
 */
void PIR_time_elapsed(TPIR_sensor *pir) {

	//Decrease the remaining delay
	--(pir->remaining_delay);
	if (pir->remaining_delay == 0U) {
		// if the delay has passed, start the alarm
		PIR_change_state(pir, ALARM_STATE_ALARMED);
		return;
	}
	//decreasing remaining delay will cause an under flow, so sum it to the duration and wait until it the overflow of the sum is zero
	uint8_t t = pir->remaining_delay + pir->alarm_duration;
	if (t == 0) {
		PIR_change_state(pir, ALARM_STATE_ACTIVE);
	}
}

/*
 * @fn        PIR_get_string_state(TPIR_sensor *pir, char *area_state)
 * @brief     set in the area_state parameter the current state of the pir sensor, as a string
 * @param     pir sensor: reference to the photoresistor variable
 * @param     area_state: reference to the buffer where to write the current state of pir sensor
 * @retval	  None
 */
void PIR_get_string_state(TPIR_sensor *pir, char *area_state) {
	switch (pir->state) {
	case ALARM_STATE_INACTIVE:
		strncpy(area_state, "Inactive", 8);
		break;
	case ALARM_STATE_ACTIVE:
		strncpy(area_state, "Active", 6);
		break;
	case ALARM_STATE_DELAYED:
		strncpy(area_state, "Delayed", 7);
		break;
	case ALARM_STATE_ALARMED:
		strncpy(area_state, "Alarmed", 7);
		break;
	default:
		break;
	}
	return;
}
