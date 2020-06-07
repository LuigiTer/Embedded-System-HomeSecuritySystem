#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "console.h"
#include "pir_sensor.h"
#include "photoresistor.h"
#include "datetime.h"
#include "rtc_ds1307.h"

void write_message_with_date_time(const char *message);
void show_date_time_callback(TDatetime *datetime);

#endif /* INC_LOGGER_H_ */

