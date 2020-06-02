#include "utils.h"

/*
 * Returns TRUE if arr1 and arr2 contain the same elements, FALSE otherwise
 */
bool areEqual(const uint8_t arr1[], const uint8_t arr2[], const uint8_t n, const uint8_t m) {
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
 * Returns TRUE if arr elements are only digits, FALSE otherwise
 */
bool isOnlyDigit(const uint8_t arr[], const uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		if (!isdigit(arr[i])) {
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * Returns the number of digits of an integer
 */
uint8_t digitsOf(const uint16_t n) {
	uint8_t count = 0;
	uint16_t num = n;
    /* Run loop till num is greater than 0 */
    do {
        /* Increment digit count */
        count++;

        /* Remove last digit of 'num' */
        num /= 10;
    } while(num != 0);

    return count;
}
