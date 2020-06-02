/*
 * This module contains useful functions
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <ctype.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "bool.h"

/*
 * @fn		bool areEqual(const uint8_t arr1[], const uint8_t arr2[], const uint8_t n, const uint8_t m)
 * @brief	Checks if two arrays contain the same elements
 * @param	arr1	first of the two arrays to compare
 * @param	arr2	second of the two arrays to compare
 * @param	n		number of elements in arr1
 * @param	n		number of elements in arr2
 * @retval	TRUE if arr1 and arr2 contain the same elements, FALSE otherwise
 */
bool areEqual(const uint8_t arr1[], const uint8_t arr2[], const uint8_t n,
		const uint8_t m);

/*
 * @fn		bool isOnlyDigit(const uint8_t arr[], const uint8_t n)
 * @brief	Checks if all elements in array are only digits
 * @param	arr		the array containing the digits to check
 * @param	n		the number of elements in arr
 * @retval	TRUE if all elements in arr are only digits, FALSE otherwise
 */
bool isOnlyDigit(const uint8_t arr[], const uint8_t n);

/*
 * @fn		uint8_t digitsOf(const uint16_t n)
 * @brief	Computes the number of digits of an integer
 * @param	n	number to count the digits of
 * @retval	number of digits of n
 */
uint8_t digitsOf(const uint16_t n);

#endif /* INC_UTILS_H_ */
