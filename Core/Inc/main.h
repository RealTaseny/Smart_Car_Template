/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MAG_Pin GPIO_PIN_13
#define MAG_GPIO_Port GPIOC
#define MAG_EXTI_IRQn EXTI15_10_IRQn
#define RUN_SWITCH_Pin GPIO_PIN_14
#define RUN_SWITCH_GPIO_Port GPIOC
#define RUN_SWITCH_EXTI_IRQn EXTI15_10_IRQn
#define Peripheral_Control_Pin GPIO_PIN_15
#define Peripheral_Control_GPIO_Port GPIOC
#define BLDC_PWM1_Pin GPIO_PIN_0
#define BLDC_PWM1_GPIO_Port GPIOA
#define BLDC_PWM2_Pin GPIO_PIN_1
#define BLDC_PWM2_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_2
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_3
#define BT_RX_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define FLASH_SCK_Pin GPIO_PIN_5
#define FLASH_SCK_GPIO_Port GPIOA
#define FLASH_MISO_Pin GPIO_PIN_6
#define FLASH_MISO_GPIO_Port GPIOA
#define FLASH_MOSI_Pin GPIO_PIN_7
#define FLASH_MOSI_GPIO_Port GPIOA
#define CCD_AO_Pin GPIO_PIN_0
#define CCD_AO_GPIO_Port GPIOB
#define CCD_CLK_Pin GPIO_PIN_1
#define CCD_CLK_GPIO_Port GPIOB
#define CCD_SI_Pin GPIO_PIN_2
#define CCD_SI_GPIO_Port GPIOB
#define LCD_SCK_Pin GPIO_PIN_10
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_RES_Pin GPIO_PIN_13
#define LCD_RES_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_14
#define LCD_DC_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_15
#define LCD_SDA_GPIO_Port GPIOB
#define SERVO_PWM_Pin GPIO_PIN_8
#define SERVO_PWM_GPIO_Port GPIOA
#define DBG_TX_Pin GPIO_PIN_9
#define DBG_TX_GPIO_Port GPIOA
#define DBG_RX_Pin GPIO_PIN_10
#define DBG_RX_GPIO_Port GPIOA
#define MOTOR_R_DIR_Pin GPIO_PIN_11
#define MOTOR_R_DIR_GPIO_Port GPIOA
#define MOTOR_L_DIR_Pin GPIO_PIN_12
#define MOTOR_L_DIR_GPIO_Port GPIOA
#define ENCODER_R_PULSE_Pin GPIO_PIN_15
#define ENCODER_R_PULSE_GPIO_Port GPIOA
#define ENCODER_R_DIR_Pin GPIO_PIN_3
#define ENCODER_R_DIR_GPIO_Port GPIOB
#define ENCODER_L_PULSE_Pin GPIO_PIN_4
#define ENCODER_L_PULSE_GPIO_Port GPIOB
#define ENCODER_L_DIR_Pin GPIO_PIN_5
#define ENCODER_L_DIR_GPIO_Port GPIOB
#define MOTOR_L_PWM_Pin GPIO_PIN_6
#define MOTOR_L_PWM_GPIO_Port GPIOB
#define MOTOR_R_PWM_Pin GPIO_PIN_7
#define MOTOR_R_PWM_GPIO_Port GPIOB
#define GYRO_INT_Pin GPIO_PIN_8
#define GYRO_INT_GPIO_Port GPIOB
#define GYRO_INT_EXTI_IRQn EXTI9_5_IRQn
#define GYRO_CS_Pin GPIO_PIN_9
#define GYRO_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
