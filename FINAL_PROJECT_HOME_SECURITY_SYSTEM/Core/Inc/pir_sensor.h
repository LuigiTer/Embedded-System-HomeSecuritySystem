/*
 * This module is used to represent the PIR sensor. Offers a structure to configure more than one on the same system and
 * functions to enable or disable it. The activation process should be called when the user enable the sensor, while the
 * deactivation process when the system or the sensor is disabled.
 *
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

/**
 * @brief 			Structure that holds the configuration parameters of the PIR sensor. This structure allows the installation of multiple sensor
 * 						with small modifications.
 * @param alarm_delay		the delay of the sensor
 * @param remaining_delay	counter of the remaining delay
 * @param state				current state of the sensor
 * @param alarm_duration	the alarm duration. Remaining duration isn't necessary
 * @param irq				the IRQn which is dedicated to the sensor
 * @param port				the port which the sensor is connected to
 * @param pin				the pin which the sensor is connected to
 * @param timer				the timer used by the pir sensor
 * @param buzzer			the buzzer associated to the sensor
 */
typedef struct PIR_sensor {
	uint8_t alarm_delay;
	uint8_t remaining_delay;
	uint8_t state;
	uint8_t alarm_duration;
	IRQn_Type irq;
	GPIO_TypeDef *port;
	uint16_t pin;
	TIM_HandleTypeDef *timer;
	TBuzzer *buzzer;
} TPIR_sensor;


/*
 * @fn       static void PIR_change_state(TPIR_sensor *pir, TAlarmState new_state)
 * @brief    private function that change the state of the pir sensor according to the new state passed as parameter. This funcion will also disable
 * 				the corresponing IRQ and the buzzer
 * @param    pir: reference to the pir structure which needs the state to be changed
 * @param    new_state: the new state that the pir sensor must assume
 */
static void PIR_change_state(TPIR_sensor *pir, TAlarmState new_state) {
	// Get the pulse associated to the pir sensor.
	TPulse pulse = buzzer_medium_pulse();

	switch (new_state) {
	case ALARM_STATE_INACTIVE:
		HAL_TIM_Base_Stop_IT(pir->timer);
		HAL_NVIC_DisableIRQ(pir->irq);
		buzzer_decrease_pulse(pir->buzzer, pulse);
		break;
	case ALARM_STATE_ACTIVE:
		HAL_TIM_Base_Stop_IT(pir->timer);
		HAL_NVIC_EnableIRQ(pir->irq);
		pir->remaining_delay = pir->alarm_delay;
		buzzer_decrease_pulse(pir->buzzer, pulse);
		if (HAL_GPIO_ReadPin(pir->port, pir->pin) == GPIO_PIN_SET) {
			//if the sensor is still high, retrigger the interrupt
			HAL_NVIC_SetPendingIRQ(pir->irq);
		}
		break;
	case ALARM_STATE_DELAYED:
		pir->timer->Instance->CNT = 0;
		HAL_TIM_Base_Start_IT(pir->timer);
		pir->remaining_delay = pir->alarm_delay;
		buzzer_decrease_pulse(pir->buzzer, pulse);
		break;
	case ALARM_STATE_ALARMED:
		buzzer_increase_pulse(pir->buzzer, pulse);
		break;
	default:
		break;
	}
	pir->state = new_state;
}


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
 * @return None
 */
void PIR_sensor_init(TPIR_sensor *pir, uint8_t delay, uint8_t alarm_duration,
		IRQn_Type irq, GPIO_TypeDef *port, uint16_t pin,
		TIM_HandleTypeDef *timer, TBuzzer *buzzer);

/**
 * @fn 			void PIR_sensor_activate(TPIR_sensor *pir)
 * @brief		Reactivate and set as active a pir sensor using the previous configuration
 * @param pir	the structure which holds the sensor to be activated
 * @return 		None
 */
void PIR_sensor_activate(TPIR_sensor *pir);

/**
 * @fn 			 void PIR_sensor_deactivate(TPIR_sensor *pir)
 * @brief		 Deactivate and set as inactive a pir sensor. NOTE: also the irq and the timer will be disabled
 * @param pir	 the structure of the pin to deactivate
 * @retval 		 None
 */
void PIR_sensor_deactivate(TPIR_sensor *pir);

/**
 * @fn 			PIR_sensor_handler(TPIR_sensor *pir)
 * @brief 		ISR of the pir_sensor. This should be called both on rising and falling edge
 * @param pir 	the structure of the pin which has triggered the interruption
 * @retval		None
 */
void PIR_sensor_handler(TPIR_sensor *pir);

/**
 * @fn			PIR_time_elapsed(TPIR_sensor *pir)
 * @brief 		ISR of the associated timer. This will reduce the remaing delay or the duration of the alarm
 * @param pir 	the structure of the pin which is currently on alarm or delayed state
 * @retval 		None
 */
void PIR_time_elapsed();

/*
 * @fn        PIR_get_string_state(TPIR_sensor *pir, char *area_state)
 * @brief     set in the area_state parameter the current state of the pir sensor, as a string
 * @param     pir sensor: reference to the photoresistor variable
 * @param     area_state: reference to the buffer where to write the current state of pir sensor
 * @retval	  None
 */
void PIR_get_string_state(TPIR_sensor *pir, char *area_state);

#endif /* INC_PIR_SENSOR_H_ */
