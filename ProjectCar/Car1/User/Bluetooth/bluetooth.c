/*
 * bluetooth.c
 *
 *  Created on: 2023年7月12日
 *      Author: 炸鲜奶
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include "bluetooth.h"
#include <stdio.h>
#include "headfile.h"
//配置结构体
const eUSCI_UART_ConfigV1 uartConfigBlue =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,//时钟源
    312,
    8,
    0,
    EUSCI_A_UART_NO_PARITY,//无校验
    EUSCI_A_UART_LSB_FIRST,//低位先行
    EUSCI_A_UART_ONE_STOP_BIT,//一个停止位
    EUSCI_A_UART_MODE,//UART模式
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
    EUSCI_A_UART_8_BIT_LEN//8位数据长度
};

void Bluetooth_Init(void)
{
    //GPIO配置
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9,GPIO_PIN6|GPIO_PIN7,GPIO_PRIMARY_MODULE_FUNCTION);
    //配置UART
    MAP_UART_initModule(EUSCI_A3_BASE,&uartConfigBlue);
    MAP_UART_enableModule(EUSCI_A3_BASE);
    //使能中断
    MAP_UART_enableInterrupt(EUSCI_A3_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(INT_EUSCIA3);
    MAP_Interrupt_enableMaster();
}

void communicate_handler(unsigned char ucData)
{
    static unsigned char ucRxBuffer[250];
    static unsigned char ucRxCnt = 0;
    ucRxBuffer[ucRxCnt++] = ucData; //将收到的数据存入缓冲区中

    ucRxCnt %= 250;

    if (ucRxBuffer[0] != 0x55)      //数据头不对，则重新开始寻找0x55数据头
    {
        ucRxCnt = 0;
        return;
    }
    if (ucRxCnt < 4)
    {
        return;
    } //数据不满4个，则返回
    else
    {

        if (ucRxBuffer[3] != 0xdd) //帧尾校验
        {
            ucRxCnt = 0;
            return;
        }

        if (ucRxBuffer[1] == 0xaa) //等待字节
        {
            BlueMessage.wait = ucRxBuffer[2];
        }

        if (ucRxBuffer[1] == 0xbb) //速度字节
        {
            BlueMessage.speed =ucRxBuffer[2];
        }

        if (ucRxBuffer[1] == 0xcc) //结束字节
        {
            BlueMessage.finish = 1;
        }

        ucRxCnt = 0; //清空缓存区
    }
}


//中断服务子程序
void EUSCIA3_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A3_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        uint32_t val;
        val = UART_receiveData(EUSCI_A3_BASE);//变量val就是接收到的数据的值
        communicate_handler(val);
    }
}

void speed_post(uint8_t speed_cm)
{
    uint8_t send_buffer[4];

    send_buffer[0] = 0x55;
    send_buffer[1] = 0xbb; // 0xbb表示这是速度

    if (speed_cm == 0x55) //避开帧头
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

