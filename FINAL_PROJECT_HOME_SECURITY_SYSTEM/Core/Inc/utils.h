#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <ctype.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "bool.h"

bool areEqual(const uint8_t arr1[], const uint8_t arr2[], const uint8_t n, const uint8_t m);

bool isOnlyDigit(const uint8_t arr[], const uint8_t n);

uint8_t digitsOf(const uint16_t n);

#endif /* INC_UTILS_H_ */
