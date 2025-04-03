//
// Created by Taseny on 24-11-11.
//

#include "pid.h"

Velocity_PID_structure velocity_pid_structure;
Balance_PID_structure balance_pid_structure;
Turn_PID_structure turn_pid_structure;

float clip(const float a, float x, const float b)
{
    x = x < a ? a:x;
    x = x > b ? b:x;
    return x;
}


void balance_pid_init(Balance_PID_structure *ptr_structure, const float Kp, const float Kd)
{
    ptr_structure->Kp = Kp;
    ptr_structure->Kd = Kd;
    /*Init all caculate var*/
    ptr_structure->error = 0;
    ptr_structure->expect_angle = MID_ANGLE;
    ptr_structure->output = 0;
}

void velocity_pid_init(Velocity_PID_structure *ptr_structure, const float Kp, const float Ki)
{
    ptr_structure->Kp = Kp;
    ptr_structure->Ki = Ki;
    /*Init all caculate var*/
    ptr_structure->error = 0;
    ptr_structure->error_last = 0;
    ptr_structure->error_all = 0;
    ptr_structure->output = 0;
}

void turn_pid_init(Turn_PID_structure *ptr_structure, const float Kp, const float Kd)
{
    ptr_structure->Kp = Kp;
    ptr_structure->Kd = Kd;
    /*Init all caculate var*/
    ptr_structure->error = 0;
    ptr_structure->expect_turn = 0;
    ptr_structure->output = 0;
}

void balance_pid_calculate(Balance_PID_structure *ptr_structure, const float measurement_angle, const float measurement_omega)
{
    ptr_structure->error = measurement_angle - ptr_structure->expect_angle;
    ptr_structure->output = clip(-7000, ptr_structure->Kp * ptr_structure->error + ptr_structure->Kd * measurement_omega, 7000);
}

void velocity_pid_calculate(Velocity_PID_structure *ptr_structure, const float speed_l, const float speed_r)
{
    ptr_structure->error = (float)(speed_l + speed_r) * 0.5f - ptr_structure->expect_velocity;
    ptr_structure->error_all += ptr_structure->error;
    ptr_structure->output = clip(-7000, ptr_structure->Kp * ptr_structure->error + ptr_structure->Ki * ptr_structure->error_all, 7000);

    /*积分限幅*/
    if (ptr_structure->error_all > 7000) ptr_structure->error_all = 7000;
    else if (ptr_structure->error_all < -7000) ptr_structure->error_all = -7000;
    /*积分限幅*/

    ptr_structure->error_last = ptr_structure->error;
}

void turn_pid_calculate(Turn_PID_structure *ptr_structure, const float ccd_error, const float measurement_omega)
{
    ptr_structure->error = ccd_error - ptr_structure->expect_turn;
    ptr_structure->output = clip(-7000, ptr_structure->Kp * ptr_structure->error + ptr_structure->Kd * measurement_omega, 7000);
}
