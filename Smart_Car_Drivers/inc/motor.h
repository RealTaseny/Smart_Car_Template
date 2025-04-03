#ifndef MOTOR_H__
#define MOTOR_H__

#include "stm32f4xx_hal.h"
#include "tim.h"

#define MOTOR_DIR_PORT GPIOA
#define MOTOR_L_DIR_PIN GPIO_PIN_12
#define MOTOR_R_DIR_PIN GPIO_PIN_11
#define MOTOR_L_FORWARD GPIO_PIN_RESET
#define MOTOR_R_FORWARD GPIO_PIN_RESET
#define MOTOR_L_BACKWARD GPIO_PIN_SET
#define MOTOR_R_BACKWARD GPIO_PIN_SET

#define MAX_THROTTLE 10000.0f
#define PWM_PERIOD 5000U
#define MOTOR_L_PWM_CH 0x00000000U
#define MOTOR_R_PWM_CH 0x00000004U

void motor_init(void);
void set_motor_throttle(int16_t motor_l_throttle, int16_t motor_r_throttle);

#endif
