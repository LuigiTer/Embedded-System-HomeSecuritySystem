/*
 * This module contains useful functions
 */

#include "utils.h"

/*
 * @fn		bool are_equal(const uint8_t arr1[], const uint8_t arr2[], const uint8_t n, const uint8_t m)
 * @brief	Checks if two arrays contain the same elements
 * @param	arr1	first of the two arrays to compare
 * @param	arr2	second of the two arrays to compare
 * @param	n		number of elements in arr1
 * @param	n		number of elements in arr2
 * @retval	TRUE if arr1 and arr2 contain the same elements, FALSE otherwise
 */
bool are_equal(const uint8_t arr1[], const uint8_t arr2[], const uint8_t n,
		const uint8_t m) {
	// If lengths of array are not equal means
	// array are not equal
	if (n != m)
		return FALSE;

	// Linearly compare elements
	for (uint8_t i = 0; i < n; i++)
		if (arr1[i] != arr2[i])
			return FALSE;

	// If all elements were same.
	return TRUE;
}

/*
 * @fn		bool is_only_digit(const uint8_t arr[], const uint8_t n)
 * @brief	Checks if all elements in array are only digits
 * @param	arr		the array containing the digits to check
 * @param	n		the number of elements in arr
 * @retval	TRUE if all elements in arr are only digits, FALSE otherwise
 */
bool is_only_digit(const uint8_t arr[], const uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		if (!isdigit(arr[i])) {
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * @fn		uint8_t digits_of(const uint16_t n)
 * @brief	Computes the number of digits of an integer
 * @param	n	number to count the digits of
 * @retval	number of digits of n
 */
uint8_t digits_of(const uint16_t n) {
	uint8_t count = 0;
	uint16_t num = n;
	/* Run loop till num is greater than 0 */
	do {
		/* Increment digit count */
		count++;

		/* Remove last digit of 'num' */
		num /= 10;
	} while (num != 0);

	return count;
}

/*
 * @fn		float map(float x, float inMin, float inMax, float outMin, float outMax)
 * @brief	Converts a number from an input range to an output range.
 * 			For example, if 75 is in [0, 100] and the output range is [50, 250], the result is 200
 * @param	x			number to convert
 * @param	in_min		lower bound of the input range
 * @param	in_max		upper bound of the output range
 * @param	out_min		lower bound of the input range
 * @param	out_max		upper bound of the output range
 * @retval	the converted value in [out_min, out_max]
 */
float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
