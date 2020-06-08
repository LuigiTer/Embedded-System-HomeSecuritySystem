#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "console.h"
#include "pir_sensor.h"
#include "photoresistor.h"
#include "datetime.h"
#include "rtc_ds1307.h"
#include "bool.h"

void write_message_with_date_time(TDatetime *datetime, const char *message);
void show_periodic_log(TDatetime *datetime);
void show_log_general_callback();

#endif /* INC_LOGGER_H_ */

