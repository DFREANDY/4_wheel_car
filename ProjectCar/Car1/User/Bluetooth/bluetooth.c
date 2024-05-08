/*
 * bluetooth.c
 *
 *  Created on: 2023��7��12��
 *      Author: ը����
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include "bluetooth.h"
#include <stdio.h>
#include "headfile.h"
//���ýṹ��
const eUSCI_UART_ConfigV1 uartConfigBlue =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,//ʱ��Դ
    312,
    8,
    0,
    EUSCI_A_UART_NO_PARITY,//��У��
    EUSCI_A_UART_LSB_FIRST,//��λ����
    EUSCI_A_UART_ONE_STOP_BIT,//һ��ֹͣλ
    EUSCI_A_UART_MODE,//UARTģʽ
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
    EUSCI_A_UART_8_BIT_LEN//8λ���ݳ���
};

void Bluetooth_Init(void)
{
    //GPIO����
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9,GPIO_PIN6|GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
    //����UART
    MAP_UART_initModule(EUSCI_A3_BASE,&uartConfigBlue);
    MAP_UART_enableModule(EUSCI_A3_BASE);
    //ʹ���ж�
    MAP_UART_enableInterrupt(EUSCI_A3_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA3);
    MAP_Interrupt_enableMaster();
}

void communicate_handler(unsigned char ucData)
{
    static unsigned char ucRxBuffer[250];
    static unsigned char ucRxCnt = 0;
    ucRxBuffer[ucRxCnt++] = ucData; //���յ������ݴ��뻺������

    ucRxCnt %= 250;

    if (ucRxBuffer[0] != 0x55)      //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
    {
        ucRxCnt = 0;
        return;
    }
    if (ucRxCnt < 4)
    {
        return;
    } //���ݲ���4�����򷵻�
    else
    {

        if (ucRxBuffer[3] != 0xdd) //֡βУ��
        {
            ucRxCnt = 0;
            return;
        }

        if (ucRxBuffer[1] == 0xaa) //�ȴ��ֽ�
        {
            BlueMessage.wait = ucRxBuffer[2];
        }

        if (ucRxBuffer[1] == 0xbb) //�ٶ��ֽ�
        {
            BlueMessage.speed =ucRxBuffer[2];
        }

        if (ucRxBuffer[1] == 0xcc) //�����ֽ�
        {
            BlueMessage.finish = 1;
        }

        ucRxCnt = 0; //��ջ�����
    }
}


//�жϷ����ӳ���
void EUSCIA3_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A3_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        uint32_t val;
        val = UART_receiveData(EUSCI_A3_BASE);//����val���ǽ��յ������ݵ�ֵ
        communicate_handler(val);
    }
}

void speed_post(uint8_t speed_cm)
{
    uint8_t send_buffer[4];

    send_buffer[0] = 0x55;
    send_buffer[1] = 0xbb; // 0xbb��ʾ�����ٶ�

    if (speed_cm == 0x55) //�ܿ�֡ͷ
        send_buffer[2] = 0x56;
    else
        send_buffer[2] = speed_cm;

    send_buffer[3] = 0xdd;

    MAP_UART_transmitData(EUSCI_A3_BASE, send_buffer[0]);
    MAP_UART_transmitData(EUSCI_A3_BASE, send_buffer[1]);
    MAP_UART_transmitData(EUSCI_A3_BASE, send_buffer[2]);
    MAP_UART_transmitData(EUSCI_A3_BASE, send_buffer[3]);
}
void task_and_speed_post(uint8_t task_number, uint8_t speed)
{
	MAP_UART_transmitData(EUSCI_A3_BASE, 0x55);
	MAP_UART_transmitData(EUSCI_A3_BASE, task_number);
	MAP_UART_transmitData(EUSCI_A3_BASE, speed);
	MAP_UART_transmitData(EUSCI_A3_BASE, 0xdd);
}
void task_stop_post(void)
{
	MAP_UART_transmitData(EUSCI_A3_BASE, 0x55);
	MAP_UART_transmitData(EUSCI_A3_BASE, 0xcc);
	MAP_UART_transmitData(EUSCI_A3_BASE, 0xcc);
	MAP_UART_transmitData(EUSCI_A3_BASE, 0xdd);
}
void task_control_post(uint8_t control_word)
{
    MAP_UART_transmitData(EUSCI_A3_BASE, 0x55);
	MAP_UART_transmitData(EUSCI_A3_BASE, 0xaa);
	MAP_UART_transmitData(EUSCI_A3_BASE, control_word);
	MAP_UART_transmitData(EUSCI_A3_BASE, 0xdd);
}

