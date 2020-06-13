/*
 * Here are defined the possible states of the sensors. These are common to both of them.
 * */

#ifndef INC_SENSORS_STATE_H_
#define INC_SENSORS_STATE_H_

/* This is the mininum amount of time that a sensor must wait before going in alarm */
#define NO_DELAY					(1U)

typedef enum {
	ALARM_STATE_INACTIVE, ALARM_STATE_ACTIVE, ALARM_STATE_ALARMED, ALARM_STATE_DELAYED
}TAlarmState;


#endif /* INC_SENSORS_STATE_H_ */
