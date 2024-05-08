#include "task_control.h"
#include "motor.h"
#include "openmv.h"
#include "laser.h"
#include "infoprint.h"
#include "bluetooth.h"

float diff_speed;
float control_speed = 0;
float laser_angle = 0;

bool main_control_flag;
bool aming = 0;

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

        static int timer_100ms = 0;
        ++timer_100ms;
        if(timer_100ms == 10)
        {
            timer_100ms = 0;
            task_and_speed_post(StateMachine.task_mode, (uint8_t)basic_speed);

            if(LaserMessage.angle1 < 10 && LaserMessage.n1 == 100 && LaserMessage.angle2 < 10 && LaserMessage.n2 == 100)
            {
                BUZZER_ON;
//                LED2_RGB(1, 1, 1);
//                delay_ms(1000);
//                delay_ms(1000);
//                delay_ms(1000);
//                BUZZER_OFF;
//                LED2_RGB(0, 1, 0);
//                aming = 1;
//                return;
            }
            else
            {
                BUZZER_OFF;
                LED2_RGB(0, 1, 0);
            }


            //if(LaserMessage.n1 != 0 && (LaserMessage.n1 < 100 || (LaserMessage.angle1 > 50 && LaserMessage.n1 == 100)))
                yaw_angle += pid_update(&pid_yaw, 1.0 * LaserMessage.angle1 / LaserMessage.n1);
            yaw_angle = Limiter(yaw_angle, -35, 50);
            set_yaw(yaw_angle);

            //if(LaserMessage.n2 != 0 && (LaserMessage.n2 < 100 || (LaserMessage.angle2 > 50 && LaserMessage.n2 == 100)))
                pitch_angle += pid_update(&pid_pitch, 1.0 * LaserMessage.angle2 / LaserMessage.n2);
            pitch_angle = Limiter(pitch_angle, 15, 40);
            set_pitch(pitch_angle);
        }

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
