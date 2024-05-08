#include "motor.h"
#include "pid.h"
/*PWM限幅*/
#define CCR_MAX 999

float basic_speed;                  // [-100,100]
float set_speed[5];

void pwm_duty(uint8_t tim_x0, uint8_t tim_x, uint16_t ccr)
{
    if(tim_x0 == 0)
    {
        switch (tim_x)
            {
            case 1:
                MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, ccr);
                break;
            case 2:
                MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, ccr);
                break;
            case 3:
                MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, ccr);
                break;
            case 4:
                MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, ccr);
                break;
            default:
                break;
            }
    }
    else if(tim_x0 == 1)
    {
        switch (tim_x)
            {
            case 1:
                MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, ccr);
                break;
            case 2:
                MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, ccr);
                break;
            case 3:
                MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, ccr);
                break;
            case 4:
                MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, ccr);
                break;
            default:
                break;
            }
    }
}

/// @brief 控制电机
/// @param num  电机编号
/// @param dir  方向
/// @param duty_ratio 控制速度，-100 ~ 100
void set_motor(uint8_t num, int8_t dir, uint16_t duty_ratio)
{
    uint16_t ccr = duty_ratio * CCR_MAX / 100;
    if(dir == 1)
    {
        pwm_duty(0, num, 0);
        pwm_duty(1, num, ccr);
    }
    else if(dir == -1)
    {
        pwm_duty(0, num, ccr);
        pwm_duty(1, num, 0);
    }
    else
    {
        pwm_duty(0, num, 0);
        pwm_duty(1, num, 0);
    }
    
}
void motor_stop()
{
    pwm_duty(0, 1, CCR_MAX);
    pwm_duty(1, 1, CCR_MAX);
    pwm_duty(0, 2, CCR_MAX);
    pwm_duty(1, 2, CCR_MAX);
    pwm_duty(0, 3, CCR_MAX);
    pwm_duty(1, 3, CCR_MAX);
    pwm_duty(0, 4, CCR_MAX);
    pwm_duty(1, 4, CCR_MAX);



}
void motor_finish()
{
    MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 0);
    MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, 0);
    MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, 0);
    MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 0);
    MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 0);
    MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, 0);
    MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, 0);
    MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 0);

}
void Motor_Control(int16 pwm1, int16 pwm2, int16 pwm3, int16 pwm4)
{
    pwm1 = Limiter(pwm1, -100, 100);
    pwm2 = Limiter(pwm2, -100, 100);
    pwm3 = Limiter(pwm3, -100, 100);
    pwm4 = Limiter(pwm4, -100, 100);

    set_motor(1, mySgn(pwm1), pwm1);
    set_motor(2, mySgn(pwm2), pwm2);
    set_motor(3, mySgn(pwm3), pwm3);
    set_motor(4, mySgn(pwm4), pwm4);
  
}

/**
 * @brief : 设置后侧车轮的pwm占空比
 * @note  : -100 ~ 100 负数反转
 * @param {float} l_speed
 * @param {float} r_speed
 */
void set_speed_back(float l_speed, float r_speed)
{
    if (r_speed < -0.03)
    {
        set_motor(4, mySgn(r_speed), -r_speed);
        
    }
    else if (r_speed > 0.03)
    {
        set_motor(4, mySgn(r_speed), r_speed);
    }
    else
    {
        set_motor(4, 1, 0);
    }
    if (l_speed < -0.03)
    {
        set_motor(2, mySgn(l_speed), -l_speed);
    }
    else if (l_speed > 0.03)
    {
        set_motor(2, mySgn(l_speed), l_speed);
    }
    else
    {
        set_motor(2, 1, 0);
    }
}
void set_speed_front(float l_speed, float r_speed)
{
    if (r_speed < -0.03)
    {
        set_motor(3, mySgn(r_speed), -r_speed);
        
    }
    else if (r_speed > 0.03)
    {
        set_motor(3, mySgn(r_speed), r_speed);
    }
    else
    {
        set_motor(3, 1, 0);
    }
    
    if (l_speed < -0.03)
    {
        set_motor(1, mySgn(l_speed), -l_speed);
    }
    else if (l_speed > 0.03)
    {
        set_motor(1, mySgn(l_speed), l_speed);
    }
    else
    {
        set_motor(1, 1, 0);
    }
}



/**
 * @brief : 闭环控制
 * @note  :
 * @param {float} l_speed   左侧速度
 * @param {float} r_speed   右侧速度
 */
void pid_set(float l_speed, float r_speed)
{
    int encode_local_encode_lb = encode_lb;
    int encode_local_encode_rb = encode_rb;
    int encode_local_encode_lf = encode_lf;
    int encode_local_encode_rf = encode_rf;

    encode_lb = 0;
    encode_rb = 0;
    encode_lf = 0;
    encode_rf = 0;

    float speed_to_encode_10ms = 0.01 / ENCODE_TO_DIS;

    float l_tar = l_speed * speed_to_encode_10ms;
    float r_tar = r_speed * speed_to_encode_10ms;

    float speed_l, speed_r;

    speed_l = pid_update(&pid_lb, l_tar - encode_local_encode_lb);
    speed_r = pid_update(&pid_rb, r_tar - encode_local_encode_rb);

    set_speed_back(speed_l, speed_r);
    set_speed[2] =  speed_l;
    set_speed[4] =  speed_r;

    speed_l = pid_update(&pid_lf, l_tar - encode_local_encode_lf);
    speed_r = pid_update(&pid_rf, r_tar - encode_local_encode_rf);

    set_speed_front(speed_l, speed_r);

    set_speed[1] =  speed_l;
    set_speed[3] =  speed_r;

    encode_total += encode_local_encode_lb;
    encode_total += encode_local_encode_rb;
    encode_total += encode_local_encode_lf;
    encode_total += encode_local_encode_rf;

}



