#ifndef __HEADFILE_H
#define __HEADFILE_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "delay.h"
#include "clock.h"
#include <stdint.h> 
#include <stdbool.h>

#include "pid.h"
#include "bluetooth.h"


#define mySgn(val)           ((val) < 0 ? -1 : ((val) > 0 ? 1 : 0))
#define PI 3.14159f


#define CAR_NUMBER 2

#define ENCODE_TO_DIS 0.0291     //±àÂëÆ÷×ªÊý - ÐÐÊ»1m

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int16_t int16;
typedef int32_t int32;

typedef enum SIGN_E
{
    SIGN_OFF,
    SIGN_ON,
    SIGN_BLINK
} sigh_enum;

typedef enum TRACKING_STATE_enum
{
    INNER,
    OUTER,
    TURN_LEFT,
    TURN_RIGHT
} tracking_state_enum;

typedef struct
{
	uint8_t state;				
	uint8_t task_mode;          
	uint8_t task_progress;

    sigh_enum led_state;
    sigh_enum buzzer_state;
    tracking_state_enum tracking_state;

    float input_speed;

} state_machine_t;

extern int32_t time_count_flag;
extern int32_t mission_time_count;

extern pid_t pid_lb;
extern pid_t pid_rb;
extern pid_t pid_lf;
extern pid_t pid_rf;
extern pid_t pid_diff;

extern state_machine_t StateMachine;
extern bluetooth_message_t BlueMessage;

#endif
