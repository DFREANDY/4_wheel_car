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

state_machine_t StateMachine;
bluetooth_message_t BlueMessage;

uint8_t pointer = 0;

void clear_PID();
void clear_statemachine();
void clear_bluemessage();

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
    /*编码器初始化*/
    EXTI_Init();
    /*定时器初始化*/
    TimA0_Init(1000 - 1, 48);
    TimA1_Init(1000 - 1, 48);

    //--------------------------------------------------motor--------------------------------------------------
    pid_initialize(&pid_lf, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_lb, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_rf, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_rb, 5, 0.5, 1.5, 100, 100);
    pid_initialize(&pid_diff, 2, 0,   0,  10, 15);

    //--------------------------------------------------usart--------------------------------------------------
    Openmv_Init();
    Bluetooth_Init();
    //--------------------------------------------------oled--------------------------------------------------
    //OLED_Init();                              // OLED初始化
    //OLED_Clear();                             // OLED清屏
    //--------------------------------------------------ta it--------------------------------------------------
    TA3_IT_INIT();
    /*开启总中断*/
    MAP_Interrupt_enableMaster();
    //--------------------------------------------------main--------------------------------------------------
    LED1_OFF;
    BUZZER_OFF;

//    while(1)
//    {
//        clear_statemachine();
//        clear_PID();
//        clear_bluemessage();
//
//        LED1_ON;
//        LED2_RGB(1, 0, 0);
//        if(KEY_Input());
//        //while(StateMachine.state == 0);
//        LED2_RGB(0, 1, 0);
//        LED1_OFF;
//
//        StateMachine.state = 1;
//        Mission_test();
//        //BUZZER_ON;
//        delay_ms(1000);
//        delay_ms(1000);
//        delay_ms(1000);
//        motor_finish();
//        BUZZER_OFF;
//        LED2_RGB(0, 0, 0);
//
//    }

    while (1)
    {
        clear_statemachine();
        clear_PID();
        clear_bluemessage();
        LED1_ON;
        while(StateMachine.state == 0);

//        if(KEY_Input());
//        StateMachine.task_mode = 1;

        LED1_OFF;
        info_print_mainmenu(StateMachine.task_mode - 1);

        switch (StateMachine.task_mode - 1)
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
        case 5:
            Mission_6();
            break;
        default:
            break;
        }
        delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
        motor_finish();
        BUZZER_OFF;

        stop_sign = 0;
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
