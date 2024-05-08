#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"

/*****************************************************************
 *Function: EXTI_Init(void)
 *Description:�ⲿ�жϳ�ʼ��
 *Input:��
 *Output:��
 *Return:��
 *Others:��
 *Data:2023/07/12
 *****************************************************************/
void EXTI_Init(void)
{
    /*����Ϊ������������*/
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN7);
    /*����жϱ�־λ*/
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN7);
    /*���ô�����ʽ*/
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN7, GPIO_HIGH_TO_LOW_TRANSITION);
    /*�����ж����ȼ���ע���ж����ȼ���������Ϊһ�����������bug*/
    Interrupt_setPriority(INT_TA0_0, 1 << 5);
    Interrupt_setPriority(INT_TA2_0, 1 << 5);
    Interrupt_setPriority(INT_PORT4, 1 << 5);
    /*�����ⲿ�ж�*/
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN7);
    /*�����˿��ж�*/
    Interrupt_enableInterrupt(INT_PORT4);
}
