#include "logger.h"

void write_message_with_date_time(const char *message) {
	rtc_ds1307_get_datetime(get_configuration()->datetime);
	TDatetime *datetime = get_configuration()->datetime;
	char msg[512] = { '\0' };
	sprintf(msg, (char*) "[%02u-%02u-%02u %02u:%02u:%02u] %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, message);
	print_on_console(msg);
}

void show_date_time_callback(TDatetime *datetime) {
	char msg[512] = { '\0' };
	char area_state[10] = { '\0' };
	char barrier_state[10] = { '\0' };

	switch (PIR_4.state) {
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

	switch (photoresistor1.state) {
	case ALARM_STATE_INACTIVE:
		strncpy(barrier_state, "Inactive", 8);
		break;
	case ALARM_STATE_ACTIVE:
		strncpy(barrier_state, "Active", 6);
		break;
	case ALARM_STATE_DELAYED:
		strncpy(barrier_state, "Delayed", 7);
		break;
	case ALARM_STATE_ALARMED:
		strncpy(barrier_state, "Alarmed", 7);
		break;
	default:
		break;
	}

	sprintf(msg, (uint8_t*) "[%02u-%02u-%02u %02u:%02u:%02u] Area %s - Barrier %s\r\n", datetime->date,
			datetime->month, datetime->year, datetime->hour, datetime->minute,
			datetime->second, area_state, barrier_state);
	print_on_console(msg);
}
