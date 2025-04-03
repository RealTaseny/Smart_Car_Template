//
// Created by Taseny on 24-11-11.
//

#ifndef PID_H__
#define PID_H__

#define MID_ANGLE 89.5f

typedef struct Velocity_PID_structure{
    float error_last;
    float error;
    float error_all;
    float expect_velocity;
    float Kp;
    float Ki;
    float output;
}Velocity_PID_structure;

typedef struct Balance_PID_structure{
    float error;
    float expect_angle;
    float Kp;
    float Kd;
    float output;
}Balance_PID_structure;

typedef struct Turn_PID_structure{
    float error;
    float expect_turn;
    float Kp;
    float Kd;
    float output;
}Turn_PID_structure;

float clip(float a, float x, float b);
void balance_pid_init(Balance_PID_structure *ptr_structure, float Kp, float Kd);
void velocity_pid_init(Velocity_PID_structure *ptr_structure, float Kp, float Ki);
void turn_pid_init(Turn_PID_structure *ptr_structure, float Kp, float Kd);

void balance_pid_calculate(Balance_PID_structure *ptr_structure, float measurement_angle, float measurement_omega);
void velocity_pid_calculate(Velocity_PID_structure *ptr_structure, float speed_l, float speed_r);
void turn_pid_calculate(Turn_PID_structure *ptr_structure, float ccd_error, float measurement_omega);

extern Velocity_PID_structure velocity_pid_structure;
extern Balance_PID_structure balance_pid_structure;
extern Turn_PID_structure turn_pid_structure;

#endif
