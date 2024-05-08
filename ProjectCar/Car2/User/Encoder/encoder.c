#include "encoder.h"
#include "headfile.h"
#include "task_control.h"

int encode_lb, encode_rb, encode_lf, encode_rf;
int32_t encode_100ms_total;
int64_t encode_total;
float dis_total;
float odometer;
float dis_before_wait_1 = 0;
float dis_before_wait_2 = 0;

float distance_calc(void)
{
    return encode_total * 0.25 * ENCODE_TO_DIS;
}

void until_distance(float cm)
{
    float end_diatance = distance_calc() + cm;
    while (1)
    {
        dis_total = distance_calc();
        if (distance_calc() > end_diatance)
            break;
        main_control();
    }
}

void remark_distance_1()
{
    dis_before_wait_1 = distance_calc();
}
void remark_distance_2()
{
    dis_before_wait_2 = distance_calc() - dis_before_wait_1;
}

void PORT4_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    switch (status)
    {
    case GPIO_PIN0:
        if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1))
        {
            encode_lf++;
        }
        else
        {
            encode_lf--;
        }
        break;
    case GPIO_PIN1:
        if (!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN0))
        {
            encode_lf++;
        }
        else
        {
            encode_lf--;
        }

        break;
    case GPIO_PIN2:
        if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3))
        {
            encode_rf--;
        }
        else
        {
            encode_rf++;
        }

        break;
    case GPIO_PIN3:
        if (!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN2))
        {
            encode_rf--;
        }
        else
        {
            encode_rf++;
        }

        break;
    case GPIO_PIN4:
        if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN5))
        {
            encode_lb++;
        }
        else
        {
            encode_lb--;
        }

        break;
    case GPIO_PIN5:
        if (!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN4))
        {
            encode_lb++;
        }
        else
        {
            encode_lb--;
        }

        break;
    case GPIO_PIN6:
        if (GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN7))
        {
            encode_rb--;
        }
        else
        {
            encode_rb++;
        }
        break;
    case GPIO_PIN7:
        if (!GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6))
        {
            encode_rb--;
        }
        else
        {
            encode_rb++;
        }

        break;
    }

}
