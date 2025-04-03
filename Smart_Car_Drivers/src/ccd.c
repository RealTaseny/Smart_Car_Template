#include "ccd.h"

volatile uint8_t ccd_finish_flag;
uint16_t ccd_data_ch[128];

void ccd_init(void)     
{
	ADC_ChannelConfTypeDef sConfig = {0};
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

void ccd_send_data(const uint16_t *dat)
{
  uint8_t i = 0, Head_Frame[4] = {0x00, 0xff, 0x01, 0x00};
  
	#ifdef USE_WIRELESS_UART
	HAL_UART_Transmit(&huart2, Head_Frame, 4, HAL_MAX_DELAY);
  #endif

	#ifdef USE_DBG_UART
	HAL_UART_Transmit(&huart1, Head_Frame, 4, HAL_MAX_DELAY);
	#endif
	for(i=0; i<128; i++)        
	{
		#ifdef USE_WIRELESS_UART
		HAL_UART_Transmit(&huart2, (uint8_t *)(dat[i]>>8), 1, HAL_MAX_DELAY);
		#endif

		#ifdef USE_DBG_UART
		HAL_UART_Transmit(&huart1, (uint8_t *)(dat[i]>>8), 1, HAL_MAX_DELAY);
		#endif
	}
}

void ccd_collect(void)
{
	uint8_t i = 0;

	CCD_CLK(1);
	CCD_SI(0);
	CCD_CLK(0);
	CCD_SI(1);
	CCD_CLK(1);
	CCD_SI(0);
	
	for(i = 0; i < 128; i++)
	{
		CCD_CLK(0);
		HAL_ADC_Start(&hadc1);
		if(HAL_OK == HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY)) ccd_data_ch[i] = HAL_ADC_GetValue(&hadc1); 
		CCD_CLK(1); 
	}
	ccd_finish_flag = 1;
}
