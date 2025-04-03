#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f4xx_hal.h"
#include "tim.h"

#define SPEED_GAIN 2

void encoder_init(void);
void get_speed(int16_t *speed_l, int16_t *speed_r);

#endif
