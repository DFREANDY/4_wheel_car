#ifndef __ENCODER_H
#define __ENCODER_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

extern int encode_lb, encode_rb, encode_lf, encode_rf;

extern int32_t encode_100ms_total;
extern int64_t encode_total;

extern float dis_total;
extern float dis_before_wait_1;
extern float dis_before_wait_2;
extern float odometer;

float distance_calc(void);
void until_distance(float cm);

void remark_distance_1();
void remark_distance_2();

void PORT4_IRQHandler(void);

#endif
