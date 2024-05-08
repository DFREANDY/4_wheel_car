#include "task_control.h"
#include "motor.h"
#include "openmv.h"
#include "infoprint.h"
#include "bluetooth.h"

float diff_speed;
float control_speed = 0;
float laser_angle = 0;

bool main_control_flag;

void main_control()
{
    while(1)
    {
        if(main_control_flag == 0)
            break;
        
        if (time_count_flag)
        {
            mission_time_count++;
        }

//        static int timer_100ms = 0;
//        ++timer_100ms;
//        if(timer_100ms == 10)
//        {
//            timer_100ms = 0;
//            task_and_speed_post(StateMachine.task_mode, (uint8_t)basic_speed);
//
//            laser_angle -= pid_update(&pid_laser, 1.0 * LaserMessage.angle1 / LaserMessage.angle2);
//            laser_angle = Limiter(laser_angle, -45, 45);
//            set_steering(laser_angle);
//        }

        control_speed = basic_speed;


        if(StateMachine.tracking_state == INNER)
            diff_speed = -pid_update(&pid_diff, 0 + OpenmvMessage.angle1);
        else if(StateMachine.tracking_state == OUTER)
            diff_speed = -pid_update(&pid_diff, 0 + OpenmvMessage.angle2);
        else if(StateMachine.tracking_state == TURN_LEFT)
        {
            diff_speed = -control_speed;
            control_speed = 0;
        }
        else if(StateMachine.tracking_state == TURN_RIGHT)
        {
            diff_speed = control_speed;
            control_speed = 0;
        }
        else
            diff_speed = -pid_update(&pid_diff, 0 + OpenmvMessage.angle1);

        if(basic_speed == 0)
        {
            diff_speed = 0;
        }

        pid_set(control_speed + diff_speed, control_speed - diff_speed);
        
        main_control_flag = 0;
    }
}


/* Timer A3 IRQ Hander */
void TA3_0_IRQHandler(void)
{    
    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0); //清除中断状态
    Timer_A_clearTimer(TIMER_A3_BASE);                                                      //清除定时器计数    
    
    main_control_flag = 1;
    
}
