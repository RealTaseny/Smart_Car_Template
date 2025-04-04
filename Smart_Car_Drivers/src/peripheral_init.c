//
// Created by Taseny on 25-4-2.
//

#include "peripheral_init.h"
#include "w25qxx_flash.h"
#include "bmi270.h"
#include "motor.h"
#include "ips_160.h"
#include "ccd.h"
#include "encoder.h"
#include "usart.h"
#include "spi.h"
#include "gpio.h"
#include "dma.h"

static void reset_power(uint32_t power_on_delay)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);      //外设掉电
    HAL_Delay(power_on_delay);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);        //5V外设供电开启
}

void peripheral_init(void)
{
    system_gpio_init();

    system_usart1_init(921600);
    system_usart2_init(921600);

    system_dma_init();
    system_spi1_init();
    system_spi2_init();

    reset_power(1000);

    ips160_init();
    bmi270_init();
    ccd_init();
    motor_init();
    encoder_init();

    HAL_UART_Receive_IT(&huart1, &rx_data, 1);

#if USE_UART2_IT == 1
    HAL_UART_Receive_IT(&huart2, &rx_buffer[rx_buffer_index], 1);
#endif
}