#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"

/*****************************************************************
 *Function: EXTI_Init(void)
 *Description:外部中断初始化
 *Input:无
 *Output:无
 *Return:无
 *Others:无
 *Data:2023/07/12
 *****************************************************************/
void EXTI_Init(void)
{
    /*配置为上拉输入引脚*/
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN7);
    /*清除中断标志位*/
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN7);
    /*配置触发方式*/
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN7, GPIO_HIGH_TO_LOW_TRANSITION);
    /*设置中断优先级，注意中断优先级必须设置为一样，否则会有bug*/
    Interrupt_setPriority(INT_TA0_0, 1 << 5);
    Interrupt_setPriority(INT_TA2_0, 1 << 5);
    Interrupt_setPriority(INT_PORT4, 1 << 5);
    /*开启外部中断*/
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN7);
    /*开启端口中断*/
    Interrupt_enableInterrupt(INT_PORT4);
}
