#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdlib.h>
#include <assert.h>

#include "stm32f4xx_hal.h"

#define MAX_VALUE	(255)
#define STEP		(20)

typedef float TDutyCycle;

typedef uint32_t TValue;

typedef struct {
	TIM_HandleTypeDef 	*htim;
	float 				currentDutyCycle;
} TBuzzer;

TBuzzer* buzzerInit(TIM_HandleTypeDef *htim, uint32_t Channel);

TDutyCycle getDutyCycle(TBuzzer *buzzer);

TValue getDutyCycleValue(TBuzzer *buzzer);

void setDutyCycle(TBuzzer *buzzer, const TDutyCycle dutyCycle);

void setDutyCycleToValue(TBuzzer *buzzer, const TValue value);

void increaseDutyCycle(TBuzzer *buzzer);

// TODO remove static and move to utils
static float map(float x, float inMin, float inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

static TValue getValueOfDutyCycle(const TDutyCycle dutyCycle) {
	return (TValue) map(dutyCycle, 0.0, 1.0, 0, MAX_VALUE);
}

static TDutyCycle getDutyCycleOfValue(const TValue value) {
	return map(value, 0, MAX_VALUE, 0, 1);
}

#endif /* INC_BUZZER_H_ */
