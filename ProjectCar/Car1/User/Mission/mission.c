#include "mission.h"
#include "motor.h"
#include "bluetooth.h"
#include "laser.h"
#include "task_control.h"
#include "openmv.h"
#include "led.h"

#define T_L_L       50
#define S_40        40

void path_test();
void path_1();
void path_2();
void path_3();
void path_4();
void path_5();
void until_turn();
void until_fork();
void until_stop();
void stop_the_car();
void make_a_turn();
void turn_left();
void turn_right();
void turn_around();
void wait_for();
void shoot_target();
void shoot_at_turn();

void Mission_test(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;
    task_and_speed_post(1, 40);

    path_test();

    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
//    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);
}

void Mission_1(void)
{ //ol = 5.485m, l1 = 6.085m, t1 = 15.21s
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;
    task_and_speed_post(1, S_40);

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
    task_and_speed_post(2, S_40);

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

    path_3();

    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
//    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);

}
void Mission_4(void)
{
    StateMachine.input_speed = basic_speed = S_40;

    time_count_flag = 1;
    task_and_speed_post(4, S_40);

    path_4();
    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
//    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);

}
void Mission_5(void)
{
    StateMachine.input_speed = basic_speed = 25;

    time_count_flag = 1;
    task_and_speed_post(5, S_40);

    path_5();
    stop_the_car();
    time_count_flag = 0;

    task_stop_post();
//    while(BlueMessage.finish == 0);
    BUZZER_ON;
    LED2_RGB(1, 1, 1);

}
void path_test()
{//path_2
    StateMachine.tracking_state = INNER;

    make_a_turn();

//    wait_for();

    make_a_turn();

    make_a_turn();

    until_fork();
    turn_right();

    until_distance(80);
    task_control_post(1);
    turn_around();

    until_fork();
    turn_right();
    until_distance(20);

    make_a_turn();

    until_distance(40);
}
//G-A-E-F-C-B-E-F-D-G
void path_1()
{
    StateMachine.tracking_state = INNER;

    until_distance(10);
    make_a_turn();

    until_fork();
    turn_right();

    until_distance(45);
    task_control_post(1);

    until_fork();
    turn_left();
    until_distance(20);

    make_a_turn();

    make_a_turn();

    until_fork();
    turn_left();
    until_distance(20);
    until_fork();
    turn_right();
    until_distance(20);

    make_a_turn();

    until_distance(40);
}
//A-B-C-wait-D
void path_2()
{
    StateMachine.tracking_state = INNER;

    make_a_turn();

    wait_for();

    make_a_turn();

    make_a_turn();

    until_fork();
    turn_right();

    until_distance(80);
    task_control_post(1);
    turn_around();

    until_fork();
    turn_right();
    until_distance(20);

    make_a_turn();

    until_distance(40);

}
//E-B-H-C-D-G-A-E
void path_3()
{
    StateMachine.tracking_state = INNER;

    until_distance(15);
    make_a_turn();

    until_distance(40);
    shoot_target();

    make_a_turn();

    until_fork();
    until_distance(15);

    make_a_turn();

    until_distance(40);
    shoot_target();

    make_a_turn();

    until_fork();
    until_distance(15);

}
//A-B-C-D
void path_4()
{
    StateMachine.tracking_state = INNER;

    until_turn();
    shoot_at_turn();

    until_fork();
    LED2_RGB(0, 1, 1);
    until_distance(20);
    LED2_RGB(0, 1, 0);

    until_turn();
    shoot_at_turn();

    until_turn();
    shoot_at_turn();

    until_fork();
    LED2_RGB(0, 1, 1);
    until_distance(20);
    LED2_RGB(0, 1, 0);

    until_turn();
    shoot_at_turn();

    until_distance(40);

}
//A-B-C-D
void path_5()
{
    StateMachine.tracking_state = INNER;

    make_a_turn();

    until_fork();
    LED2_RGB(0, 1, 1);
    until_distance(20);
    LED2_RGB(0, 1, 0);

    make_a_turn();

    make_a_turn();

    until_fork();
    LED2_RGB(0, 1, 1);
    until_distance(20);
    LED2_RGB(0, 1, 0);

    make_a_turn();

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

void stop_the_car()
{
    basic_speed = 0;
    motor_stop();
}
void make_a_turn()
{
    until_turn();
    LED2_RGB(0, 0, 1);
    until_distance(T_L_L);
    LED2_RGB(0, 1, 0);
}
void turn_left()
{
    StateMachine.tracking_state = TURN_LEFT;
    LED2_RGB(0, 1, 1);
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
    LED2_RGB(0, 1, 0);
}
void turn_right()
{
    StateMachine.tracking_state = TURN_RIGHT;
    LED2_RGB(0, 1, 1);
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
    LED2_RGB(0, 1, 0);
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
void shoot_target()
{
    motor_stop();

    while(1)
    {
        if(LaserMessage.angle1 == 0 && LaserMessage.n1 == 100 && LaserMessage.angle2 == 0 && LaserMessage.n2 == 100)
        {
            break;
        }

        yaw_angle += pid_update(&pid_yaw, 1.0 * LaserMessage.angle1 / LaserMessage.n1);
        yaw_angle = Limiter(yaw_angle, 0, 50);
        set_yaw(yaw_angle);

        pitch_angle += pid_update(&pid_pitch, 1.0 * LaserMessage.angle2 / LaserMessage.n2);
        pitch_angle = Limiter(pitch_angle, 15, 40);
        set_pitch(pitch_angle);
        delay_ms(100);
    }
    task_control_post(1);
    //wait_for();
    BUZZER_ON;
    LED2_RGB(1, 1, 1);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    BUZZER_OFF;
    LED2_RGB(0, 1, 0);
}
void shoot_at_turn()
{
    LED2_RGB(0, 0, 1);
    until_distance(0.8 * T_L_L);
    shoot_target();
    until_distance(0.2 * T_L_L);
    LED2_RGB(0, 1, 0);
}
