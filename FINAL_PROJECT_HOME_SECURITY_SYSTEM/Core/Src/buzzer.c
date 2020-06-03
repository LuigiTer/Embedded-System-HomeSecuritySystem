#include "buzzer.h"

TBuzzer* buzzerInit(TIM_HandleTypeDef *htim, uint32_t Channel) {
	HAL_TIM_PWM_Start(htim, Channel);

	TBuzzer *buzzer = NULL;
	buzzer = malloc(sizeof(buzzer));
	buzzer->htim = htim;
	buzzer->currentDutyCycle = 0;

	return buzzer;
}

TDutyCycle getDutyCycle(TBuzzer *buzzer) {
	return buzzer->currentDutyCycle;
}

TValue getDutyCycleValue(TBuzzer *buzzer) {
	return getValueOfDutyCycle(getDutyCycle(buzzer));
}

void setDutyCycle(TBuzzer *buzzer, const TDutyCycle dutyCycle) {
	assert(0 <= dutyCycle && dutyCycle < 1);

	buzzer->htim->Instance->CCR1 = getValueOfDutyCycle(dutyCycle);
	buzzer->currentDutyCycle = dutyCycle;
}

void setDutyCycleToValue(TBuzzer *buzzer, const TValue value) {
	assert(0 <= value && value < MAX_VALUE);

	buzzer->htim->Instance->CCR1 = value;
	buzzer->currentDutyCycle = getDutyCycleOfValue(value);
}

void increaseDutyCycle(TBuzzer *buzzer) {
	TValue value = getValueOfDutyCycle(buzzer->currentDutyCycle) + STEP;
	if (value >= MAX_VALUE)
		value = 0;
	setDutyCycleToValue(buzzer, value);
}
