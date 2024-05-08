#include "headfile.h"
#include "delay.h"
#include "clock.h"

#include "key.h"
#include "led.h"

#include "exti.h"
#include "encoder.h"
#include "motor.h"
#include "timer.h"
#include "pid.h"

#include "openmv.h"
#include "laser.h"
#include "oled.h"
#include "bluetooth.h"
#include "mission.h"

#include "infoprint.h"

/*开关状态变量*/
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;
/*上一次开关状态变量*/
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;
/*开关标志位*/
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

int32_t time_count_flag;
int32_t mission_time_count;

pid_t pid_lb;
pid_t pid_rb;
pid_t pid_lf;
pid_t pid_rf;
pid_t pid_diff;
pid_t pid_pitch;
pid_t pid_yaw;

state_machine_t StateMachine;
bluetooth_message_t BlueMessage;

uint8_t pointer = 0;

void clear_PID();
void clear_statemachine();
void clear_bluemessage();
void steering_init();

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();
    /*时钟初始化*/
    CLOCK_Init();
    delay_init();
    /*LED初始化*/
    LED_Init();
    /*按键初始化*/
    KEY_Init();
    /*声音接收初始化*/
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    /*编码器初始化*/
    EXTI_Init();
    /*定时器初始化*/
    TimA0_Init(1000 - 1, 48);
    TimA1_Init(1000 - 1, 48);
    TimA2_Init(20000 - 1, 48);
    TA3_IT_INIT();
    //--------------------------------------------------oled--------------------------------------------------
    OLED_Init();                              // OLED初始化
    OLED_Clear();                             // OLED清屏
    //--------------------------------------------------usart--------------------------------------------------
    Openmv_Init();
    Bluetooth_Init();
    Laser_Init();
    /*开启总中断*/
    MAP_Interrupt_enableMaster();

    //--------------------------------------------------motor--------------------------------------------------
    pid_initialize(&pid_lf, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_lb, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_rf, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_rb, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_diff, 2, 0,   0,  10, 15);
    //--------------------------------------------------main--------------------------------------------------
    LED1_OFF;
    BUZZER_OFF;

    steering_init();

    while(1)
    {
        if(LaserMessage.angle1 < 3 && LaserMessage.n1 == 100 && LaserMessage.angle2 < 3 && LaserMessage.n2 == 100)
        {
            //BUZZER_ON;
            LED2_RGB(1, 1, 1);
        }
        else
        {
            BUZZER_OFF;
            LED2_RGB(0, 0, 0);
        }


        //if(LaserMessage.n1 != 0 && (LaserMessage.n1 < 100 || (LaserMessage.angle1 > 50 && LaserMessage.n1 == 100)))
//            yaw_angle += pid_update(&pid_yaw, 1.0 * LaserMessage.angle1 / LaserMessage.n1);
        yaw_angle = Limiter(yaw_angle, -35, 55);
        set_yaw(yaw_angle);

        //if(LaserMessage.n2 != 0 && (LaserMessage.n2 < 100 || (LaserMessage.angle2 > 50 && LaserMessage.n2 == 100)))
//            pitch_angle += pid_update(&pid_pitch, 1.0 * LaserMessage.angle2 / LaserMessage.n2);
        pitch_angle = Limiter(pitch_angle, 15, 40);
        set_pitch(pitch_angle);

        delay_ms(100);
    }

   while(1)
   {
       clear_statemachine();
       clear_PID();
       clear_bluemessage();
       steering_init();
       LED1_ON;
       info_print_mainmenu(pointer);
       while (KEY_Input() == KEY1_PRESSED)
       {
           pointer = (pointer + 1) % 6;
           info_print_mainmenu(pointer);
       }
       LED1_OFF;

       StateMachine.state = 1;
       Mission_4();
       LED1_ON;
       delay_ms(1000);
       delay_ms(1000);
       delay_ms(1000);
       motor_finish();
       BUZZER_OFF;

   }

   while (1)
   {
       info_print_mainmenu(pointer);
       while (KEY_Input() == KEY1_PRESSED)
       {
           pointer = (pointer + 1) % 6;
           info_print_mainmenu(pointer);
       }

       clear_statemachine();
       clear_PID();
       clear_bluemessage();
       steering_init();
       LED1_OFF;
       LED2_RGB(0, 0, 0);

       StateMachine.state = 1;
       StateMachine.task_mode = pointer + 1;

       switch (pointer)
       {
       case 0:
           Mission_1();
           break;
       case 1:
           Mission_2();
           break;
       case 2:
           Mission_3();
           break;
       case 3:
           Mission_4();
           break;
       case 4:
           Mission_5();
           break;
//       case 5:
//           Mission_6();
//           break;
       default:
           break;
       }
       delay_ms(1000);
       delay_ms(1000);
       delay_ms(1000);
       motor_finish();
       BUZZER_OFF;
   }
}
void clear_PID()
{
    pid_initialize(&pid_lf, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_lb, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_rf, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_rb, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_diff, 0.5, 0, 0, 0, 20);
}
void clear_statemachine()
{
    StateMachine.state = 0;
    StateMachine.task_mode = 0;
    StateMachine.task_progress = 0;

    StateMachine.led_state = SIGN_OFF;
    StateMachine.buzzer_state = SIGN_OFF;

    StateMachine.input_speed = 0;
}
void clear_bluemessage()
{
    BlueMessage.task = 0;
    BlueMessage.speed = 0;
    BlueMessage.wait = 0;
    BlueMessage.finish = 0;
}
void steering_init()
{
    pid_initialize(&pid_pitch,  0.7, 0,  0,  6, 10);
    pid_initialize(&pid_yaw,    0.7, 0,  0,  6, 45);
    pitch_angle = 20;
    yaw_angle = 16.5;
    set_pitch(pitch_angle);
    set_yaw(yaw_angle);
}
