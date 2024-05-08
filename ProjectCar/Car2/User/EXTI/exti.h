#ifndef __EXTI_H
#define __EXTI_H 			   

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

extern int encoder_L;
extern int encoder_R;

void EXTI_Init(void);
void TA3_IT_INIT(void);

#endif

