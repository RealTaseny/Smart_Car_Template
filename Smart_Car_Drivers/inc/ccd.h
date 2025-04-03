#ifndef CCD_H__
#define CCD_H__

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "adc.h"
#include "usart.h"

#define CCD_CLK_Pin GPIO_PIN_1
#define CCD_CLK_GPIO_Port GPIOB
#define CCD_SI_Pin GPIO_PIN_2
#define CCD_SI_GPIO_Port GPIOB

// 控制语句
#define CCD_SI(x)                   	((x) ? (HAL_GPIO_WritePin(CCD_SI_GPIO_Port, CCD_SI_Pin, GPIO_PIN_SET))	:	(HAL_GPIO_WritePin(CCD_SI_GPIO_Port, CCD_SI_Pin, GPIO_PIN_RESET)))
#define CCD_CLK(x)                    ((x) ? (HAL_GPIO_WritePin(CCD_CLK_GPIO_Port, CCD_CLK_Pin, GPIO_PIN_SET))	:	(HAL_GPIO_WritePin(CCD_CLK_GPIO_Port, CCD_CLK_Pin, GPIO_PIN_RESET)))

extern volatile uint8_t ccd_finish_flag;
extern uint16_t ccd_data_ch[128];

void ccd_init(void);
void ccd_send_data(const uint16_t *dat);
void ccd_collect(void);


#endif
