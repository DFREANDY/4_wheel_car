#include "mission.h"
#include "motor.h"
#include "bluetooth.h"
#include "task_control.h"
#include "openmv.h"
#include "led.h"

#define T_L_L       50
#define S_40        40

void path_1();
void path_2();
void path_3();
void path_4();
void path_2_mirror();
void path_3_mirror();
void path_3_delay();
void until_turn();
void until_fork();
void until_stop();
void stop_the_car();
void turn_left();
void turn_right();
void turn_around();
void wait_for();

void Mission_test(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;

    basic_speed = 40;

    path_1();
    stop_the_car();

    basic_speed = 0;
    time_count_flag = 0;
}

void Mission_1(void)
{ //ol = 5.485m, l1 = 6.085m, t1 = 15.21s
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;


    path_1();

    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);


}
void Mission_2(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;

    path_2();
    path_2();

    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);
    
}
void Mission_3(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;
    task_and_speed_post(3, S_40);


    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);

}
void Mission_4(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;
    task_and_speed_post(4, S_40);


    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);

}
void Mission_5(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;
    task_and_speed_post(5, S_40);


    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);

}
void Mission_6(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;
    task_and_speed_post(6, S_40);


    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);

}
//H-B-E-F-D-A-E-F-C-H
void path_1()
{
    StateMachine.tracking_state = INNER;

    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    wait_for();

    until_fork();
    LED2_RGB(0, 1, 1);
    turn_left();
    LED2_RGB(0, 1, 0);
    until_distance(20);


    until_fork();
    LED2_RGB(0, 1, 1);
    turn_right();
    LED2_RGB(0, 1, 0);
    until_distance(20);

    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    until_fork();
    turn_right();
    until_distance(20);
    until_fork();
    turn_left();
    until_distance(20);

    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    until_distance(40);
}
//B-wait-A-D-C
void path_2()
{
    StateMachine.tracking_state = INNER;


    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    until_fork();
    LED2_RGB(0, 1, 1);
    turn_left();
    LED2_RGB(0, 1, 0);

    until_distance(40);
    task_control_post(1);
    until_distance(40);
    turn_around();

    until_fork();
    turn_left();
    until_distance(20);

    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    until_fork();
    until_distance(20);

    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);

    until_distance(40);
    
}

void until_turn()
{
    while (1)
    {
        if (OpenmvMessage.turn)
            break;
        main_control();
    }
}
void until_fork()
{
    while (1)
    {
        if (OpenmvMessage.fork)
            break;
        main_control();
    }
}
void until_stop()
{
    while (1)
    {
        if (stop_sign)
        {
            stop_the_car();
            BUZZER_ON;
            break;
        }
        main_control();
    }
}

void stop_the_car()
{
    basic_speed = 0;
    motor_stop();
}

void turn_left()
{
    StateMachine.tracking_state = TURN_LEFT;
    uint8_t timer = 0;
    while (1)
    {
        timer++;
        if(timer > 40)
            break;
        main_control();
        delay_ms(10);
    }
    StateMachine.tracking_state = INNER;
}
void turn_right()
{
    StateMachine.tracking_state = TURN_RIGHT;
    uint8_t timer = 0;
    while (1)
    {
        timer++;
        if(timer > 40)
            break;
        main_control();
        delay_ms(10);
    }
    StateMachine.tracking_state = INNER;
}
void turn_around()
{
    StateMachine.tracking_state = TURN_LEFT;
    uint8_t timer = 0;
    while (1)
    {
        timer++;
        if(timer > 100)
            break;
        main_control();
        delay_ms(10);
    }
    StateMachine.tracking_state = INNER;
}
void wait_for()
{
    motor_stop();
    while (1)
    {
        if(BlueMessage.wait == 1)
        {
            BlueMessage.wait = 0;
            break;
        }
        delay_ms(10);
    }
}
