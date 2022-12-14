/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
	void comando (unsigned char cmd, unsigned char type);
	void dato (unsigned char cmd, unsigned char type);
	void lcdmensaje(char *frase);
	void initled();
	volatile uint32_t contadorF= 0x00;
	volatile uint32_t Freq = 0;
	char texto[40] = {'\0'};
	char texto2[40] = {'\0'};
	TIM_HandleTypeDef htim2;
	TIM_HandleTypeDef htim3;
	const double cap=0.000000002092;
	double ind=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  initled();
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  comando(0x80,0);
	  lcdmensaje("               ");
	  comando(0x80,0);
	  lcdmensaje("Freq:");
	  if(Freq<10000){
	  	  sprintf(texto, "%d", Freq);
	  	  lcdmensaje(texto);
	  	  lcdmensaje("Hz");
	  	  HAL_Delay(500);
	  	  }
	  else if(Freq>10000 && Freq<1000000){
		  Freq=Freq/1000;
		  sprintf(texto, "%d", Freq);
		  lcdmensaje(texto);
		  lcdmensaje("kHz");
		  HAL_Delay(500);
	  }
	  else if(Freq>1000000){
	  	  Freq=Freq/1000000;
	  	  sprintf(texto, "%d", Freq);
	  	  lcdmensaje(texto);
	  	  lcdmensaje("MHz");
	  	  HAL_Delay(500);
	  }
	  comando(0xC0,0);
	  lcdmensaje("               ");
	  comando(0xC0,0);
	  lcdmensaje("induc:");
	  ind=1.0/(4.0*(3.1416*3.1416)*((double)Freq*(double)Freq)*cap);
	  if(ind>0.001){
		  ind=ind*1000.0;
		  sprintf(texto2, "%.2f",ind);
		  lcdmensaje(texto2);
		  lcdmensaje("mH");
		  HAL_Delay(500);
	  }
	  if(ind>0.000001 && ind<0.001){
	  		  ind=ind*1000000.0;
	  		  sprintf(texto2, "%.2f",ind);
	  		  lcdmensaje(texto2);
	  		  lcdmensaje("uH");
	  		  HAL_Delay(500);
	  	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 100;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 36000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SALIDA_GPIO_Port, SALIDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D4_Pin|D5_Pin|D6_Pin|D7_Pin
                          |E_Pin|RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BOTON_Pin ENTRADA_Pin */
  GPIO_InitStruct.Pin = BOTON_Pin|ENTRADA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SALIDA_Pin */
  GPIO_InitStruct.Pin = SALIDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SALIDA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D4_Pin D5_Pin D6_Pin D7_Pin
                           E_Pin RS_Pin */
  GPIO_InitStruct.Pin = D4_Pin|D5_Pin|D6_Pin|D7_Pin
                          |E_Pin|RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void comando(unsigned char cmd, unsigned char type){

	unsigned char t1,t2,i;

	HAL_GPIO_WritePin(GPIOB,E_Pin, 0);
	HAL_GPIO_WritePin(GPIOB,RS_Pin,0);
	t1=(cmd>>4)&0x0F;
	for(i=0;i<4;i++){
		t2=t1&0x01;
		switch(i){
			case 0:
				HAL_GPIO_WritePin(GPIOB,D4_Pin, t2);
				break;
			case 1:
				HAL_GPIO_WritePin(GPIOB,D5_Pin, t2);
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOB,D6_Pin, t2);
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOB,D7_Pin, t2);
				break;

		}
		t1=t1>>1;
	}
	HAL_GPIO_WritePin(GPIOB,E_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB,E_Pin, 0);
	HAL_Delay(1);

	t1=cmd & 0x0F;
				for(i=0;i<4;i++){
					t2=t1&0x01;
					switch(i){
						case 0:
							HAL_GPIO_WritePin(GPIOB,D4_Pin, t2);
							break;
						case 1:
							HAL_GPIO_WritePin(GPIOB,D5_Pin, t2);
							break;
						case 2:
							HAL_GPIO_WritePin(GPIOB,D6_Pin, t2);
							break;
						case 3:
							HAL_GPIO_WritePin(GPIOB,D7_Pin, t2);
							break;

					}
					t1=t1>>1;
				}

	HAL_GPIO_WritePin(GPIOB,E_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB,E_Pin, 0);
	HAL_Delay(1);

}
void dato(unsigned char cmd, unsigned char type){

	unsigned char t1,t2,i;

	HAL_GPIO_WritePin(GPIOB,E_Pin, 0);
	HAL_GPIO_WritePin(GPIOB,RS_Pin,1);
	t1=(cmd>>4)&0x0F;
	for(i=0;i<4;i++){
		t2=t1&0x01;
		switch(i){
			case 0:
				HAL_GPIO_WritePin(GPIOB,D4_Pin, t2);
				break;
			case 1:
				HAL_GPIO_WritePin(GPIOB,D5_Pin, t2);
				break;
			case 2:
				HAL_GPIO_WritePin(GPIOB,D6_Pin, t2);
				break;
			case 3:
				HAL_GPIO_WritePin(GPIOB,D7_Pin, t2);
				break;

		}
		t1=t1>>1;
	}
	HAL_GPIO_WritePin(GPIOB,E_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB,E_Pin, 0);
	HAL_Delay(1);

	t1=cmd & 0x0F;
				for(i=0;i<4;i++){
					t2=t1&0x01;
					switch(i){
						case 0:
							HAL_GPIO_WritePin(GPIOB,D4_Pin, t2);
							break;
						case 1:
							HAL_GPIO_WritePin(GPIOB,D5_Pin, t2);
							break;
						case 2:
							HAL_GPIO_WritePin(GPIOB,D6_Pin, t2);
							break;
						case 3:
							HAL_GPIO_WritePin(GPIOB,D7_Pin, t2);
							break;

					}
					t1=t1>>1;
				}

	HAL_GPIO_WritePin(GPIOB,E_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB,E_Pin, 0);
	HAL_Delay(1);

}
void initled(){
	HAL_Delay(20);
	comando(0x28,0);
	HAL_Delay(1);
	comando(0x01,0);
	HAL_Delay(1);
	comando(0x02,0);
	HAL_Delay(1);
	comando(0x0C,0);
	HAL_Delay(1);
	comando(0x06,0);
	HAL_Delay(1);
}
void lcdmensaje(char *frase){
	unsigned char cont=0;
	while(frase[cont]!=0){
		dato(frase[cont++],0);
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if(htim->Instance == TIM3)
    {
        contadorF = TIM2->CNT;
        Freq = contadorF * 20;
        TIM3->CNT = 0;
        TIM2->CNT = 0;
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
