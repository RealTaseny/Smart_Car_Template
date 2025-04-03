//
// Created by Taseny on 25-4-2.
//

#include "Peripheral_Init.h"
#include "w25qxx_flash.h"
#include "bmi270.h"
#include "motor.h"
#include "servo.h"
#include "ips_160.h"
#include "ccd.h"
#include "encoder.h"

void Peripheral_Init(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);      //外设掉电
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);        //5V外设供电开启
    ips160_init();                                                     //初始化0.96寸ips屏幕
    ips160_show_string(0,0,"Hardware Init...");
    bmi270_init();                                                     //初始化bmi270陀螺仪
    ccd_init();                                                        //ccd初始化
    motor_init();                                                      //初始化电机驱动
    encoder_init();                                                    //初始化方向编码器

    HAL_UART_Receive_IT(&huart1, &rx_data, 1);

#ifdef USE_WIRELESS_UART
    HAL_UART_Receive_IT(&huart2, &rx_buffer[rx_buffer_index], 1);
#endif
    HAL_Delay(2000);
    ips160_clear();
}