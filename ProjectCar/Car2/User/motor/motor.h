#ifndef __MOTOR_H
#define __MOTOR_H
#include "headfile.h"
#include "encoder.h"
extern float basic_speed;
extern float turn_diff[10];

void pwm_duty(uint8_t tim_x0, uint8_t tim_x, uint16_t ccr);


void set_motor(uint8_t num, int8_t dir, uint16_t duty);
void Motor_Control(int16 pwm1, int16 pwm2, int16 pwm3, int16 pwm4);
void set_speed_back(float left_speed, float right_speed);
void set_speed_front(float l_speed, float r_speed);
void pid_set(float l_speed, float r_speed);
void motor_stop();
void motor_finish();
#endif
