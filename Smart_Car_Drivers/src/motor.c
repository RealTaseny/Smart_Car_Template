#include "motor.h"

/*
*函数名:			void motor_init(void);
*功能:				初始化电机
*入口参数:		无
*返回参数:		无
*用法:				motor_init();				
*作者:				王彦皓
*创建日期:		2024/10/06
*/
void motor_init(void)
{//*配置TIM4 CH1、CH2输出20KHz PWM，溢出周期为5000*/
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 4999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim4);
	/*初始化TIM4完毕*/
	HAL_GPIO_WritePin(MOTOR_DIR_PORT, MOTOR_L_DIR_PIN, MOTOR_L_FORWARD);		//将方向控制引脚设置为低电平
	HAL_GPIO_WritePin(MOTOR_DIR_PORT, MOTOR_R_DIR_PIN, MOTOR_R_FORWARD);		//将方向控制引脚设置为低电平
	HAL_TIM_PWM_Start(&htim4, MOTOR_L_PWM_CH);								//开启左边电机PWM输出
	HAL_TIM_PWM_Start(&htim4, MOTOR_R_PWM_CH);								//开启右边电机PWM输出
	__HAL_TIM_SetCompare(&htim4, MOTOR_L_PWM_CH, 0);					//设定左边电机占空比为0
	__HAL_TIM_SetCompare(&htim4, MOTOR_R_PWM_CH, 0);					//设定右边电机占空比为0
}

/*
*函数名:		void set_motor_throttle(int16_t motor_l_throttle,  int16_t motor_r_throttle);
*功能:			设定左右有刷电机油门
*入口参数:	motor_l_throttle :左电机油门  motor_r_throttle:右电机油门
*返回参数:	无
*用法:			void set_motor_throttle(1000, 2000);		即设定左电机10%油门，右边电机20%油门(入口参数最大为+-10000)
*作者:			王彦皓
*创建日期:	2024/10/06
*/
void set_motor_throttle(int16_t motor_l_throttle, int16_t motor_r_throttle)
{
	if (motor_l_throttle >= 0)
	{
		__HAL_TIM_SetCompare(&htim4, MOTOR_L_PWM_CH, (uint32_t)(((float)motor_l_throttle / MAX_THROTTLE) * PWM_PERIOD));
		HAL_GPIO_WritePin(MOTOR_DIR_PORT, MOTOR_L_DIR_PIN, MOTOR_L_FORWARD);
	}
	else
	{
		motor_l_throttle = -motor_l_throttle;
		__HAL_TIM_SetCompare(&htim4, MOTOR_L_PWM_CH, (uint32_t)(((float)motor_l_throttle / MAX_THROTTLE) * PWM_PERIOD));
		HAL_GPIO_WritePin(MOTOR_DIR_PORT, MOTOR_L_DIR_PIN, MOTOR_L_BACKWARD);
	}
	
	if (motor_r_throttle >= 0)
	{
		__HAL_TIM_SetCompare(&htim4, MOTOR_R_PWM_CH, (uint32_t)(((float)motor_r_throttle / MAX_THROTTLE) * PWM_PERIOD));
		HAL_GPIO_WritePin(MOTOR_DIR_PORT, MOTOR_R_DIR_PIN, MOTOR_R_FORWARD);
	}
	else
	{
		motor_r_throttle = -motor_r_throttle;
		__HAL_TIM_SetCompare(&htim4, MOTOR_R_PWM_CH, (uint32_t)(((float)motor_r_throttle / MAX_THROTTLE) * PWM_PERIOD));
		HAL_GPIO_WritePin(MOTOR_DIR_PORT, MOTOR_R_DIR_PIN, MOTOR_R_BACKWARD);
	}
}
