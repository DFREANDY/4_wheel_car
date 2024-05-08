/*
 * bluetooth.h
 *
 *  Created on: 2023Äê7ÔÂ12ÈÕ
 *      Author: Õ¨ÏÊÄÌ
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int16_t task;
    int16_t speed;
    int16_t wait;
    int16_t finish;
}bluetooth_message_t;


void Bluetooth_Init(void);
void speed_post(uint8_t speed_cm);
void task_and_speed_post(uint8_t task_number, uint8_t speed);
void task_stop_post(void);
void task_control_post(uint8_t control_word);

#endif /* BLUETOOTH_H_ */
