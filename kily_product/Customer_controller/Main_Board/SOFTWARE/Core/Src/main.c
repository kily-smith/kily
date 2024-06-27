/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "st7735.h"
#include "fonts.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
//	uint8_t *p[20];
//	*p[0]=1;
//	uint8_t receiveDate[50];
	ST7735_Init();
	HAL_GPIO_WritePin (SPI1_BL_GPIO_Port ,SPI1_BL_Pin ,GPIO_PIN_SET );
	ST7735_FillScreen(ST7735_BLACK);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		
	  // /*仿FOC显示***********************************************************************
	ST7735_DrawString(0, 0, "[INIT]", ST7735_YELLOW, ST7735_BLACK, &Font_11x18);
						//ST7735_DrawString(0, 0, "abcdefghigklim", ST7735_YELLOW, ST7735_BLACK, &Font_8x10);
						//ST7735_DrawString(0, 20, "abcdefghigklim", ST7735_YELLOW, ST7735_BLACK, &Font_7x10);
	ST7735_DrawString(75, 0, "Vbus:", ST7735_GRAY, ST7735_BLACK, &Font_7x10);
	ST7735_DrawString(110, 0, "NO-DC", ST7735_RED, ST7735_BLACK, &Font_7x10);
	ST7735_DrawString(145, 0, " V", ST7735_GRAY, ST7735_BLACK, &Font_7x10);


	ST7735_DrawString(75, 10, "Ibus:", ST7735_GRAY, ST7735_BLACK, &Font_7x10);
 	ST7735_DrawString(110, 10, "0.00", ST7735_GREEN, ST7735_BLACK, &Font_7x10);
	ST7735_DrawString(145, 10, " A", ST7735_GRAY, ST7735_BLACK, &Font_7x10);
	ST7735_DrawString(0, 0, "[INIT]", ST7735_YELLOW, ST7735_BLACK, &Font_11x18);
	  

	ST7735_DrawString(0, 20, "M0[", ST7735_GRAY, ST7735_BLACK, &Font_11x18);
	ST7735_DrawString(34, 20, "Pos", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
	ST7735_DrawString(65, 20, "]M1[", ST7735_GRAY, ST7735_BLACK, &Font_11x18);
	ST7735_DrawString(110, 20, "Pos", ST7735_WHITE, ST7735_BLACK, &Font_11x18);
	ST7735_DrawString(140, 20, "]", ST7735_GRAY, ST7735_BLACK, &Font_11x18);

	ST7735_DrawString(0, 40, "init communication", ST7735_WHITE, ST7735_BLACK, &Font_7x10);
	ST7735_DrawString(0, 50, "pwm in init", ST7735_WHITE, ST7735_BLACK, &Font_7x10);
	ST7735_DrawString(0, 60, "abs spi cs pin init", ST7735_WHITE, ST7735_BLACK, &Font_7x10);
 	ST7735_DrawString(0, 70, "motor setup", ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		// *************************************************************************/


//	 //ST7735_DrawImage(0, 0, 100, 100,IMG);
//	  int i =-45;
//	  int j = i+45;
//	  for(i =-45;i<=100;i=i+5){
//		  j = i+45;
//		  	  	 ST7735_DrawImage(0, i, 160, 45, IMG_DATA1);
//		  HAL_Delay(100);
// ST7735_DrawImage(0, j, 160, 45, IMG_DATA1);
//	  
//	  	 ST7735_DrawImage(0, -5, 160, 45, IMG_DATA1);
//	  	 ST7735_DrawImage(0, 40, 160, 45, IMG_DATA1);

//ST7735_DrawImage(46+92, 0, 46, 80, gImage_123);

//ST7735_DrawImage(0, 0, 46, 80, gImage_123);

//		ST7735_FillScreen(ST7735_BLACK);
//		HAL_Delay(500);
//		ST7735_FillScreen(ST7735_BLUE);
//		HAL_Delay(500);
//		ST7735_FillScreen(ST7735_RED);
//		HAL_Delay(500);		//st7735屏幕显示
//	  	ST7735_FillScreen(ST7735_GREEN);
//		HAL_Delay(500);
//		ST7735_FillScreen(ST7735_CYAN);
//		HAL_Delay(500);
//		ST7735_FillScreen(ST7735_MAGENTA);
//		HAL_Delay(500);		//st7735屏幕显示		
//		ST7735_FillScreen(ST7735_YELLOW);
//		HAL_Delay(500);
//		ST7735_FillScreen(ST7735_WHITE);
//		HAL_Delay(500);		//st7735屏幕显示
//		ST7735_DrawRectangle(0, 20, 80, 20, ST7735_BLACK);
//		HAL_Delay(1000);
//		ST7735_DrawRectangle(0, 0, 80, 20, ST7735_BLUE);
//		HAL_Delay(1000);
//		ST7735_DrawRectangle(0, 40, 80, 20, ST7735_RED);
//		HAL_Delay(1000);
//		ST7735_DrawRectangle(0, 60, 80, 20, ST7735_CYAN);
//		HAL_Delay(1000);
//		ST7735_DrawRectangle(0, 80, 80, 20, ST7735_GREEN);
//		HAL_Delay(1000);
//		ST7735_DrawRectangle(0, 100, 80, 20, ST7735_MAGENTA);
//		HAL_Delay(1000);
//		ST7735_DrawRectangle(0, 120, 80, 20, ST7735_YELLOW);
//		HAL_Delay(1000);
//		ST7735_DrawRectangle(0, 140, 80, 20, ST7735_WHITE);

/*板载功能测试***********************************************************************
	  //HAL_GPIO_WritePin (RS485_DE_GPIO_Port ,RS485_DE_Pin ,GPIO_PIN_SET);  //RS485_DE 使能
//	  HAL_GPIO_WritePin (RGB_G_GPIO_Port ,RGB_G_Pin ,GPIO_PIN_RESET );
//	  HAL_Delay (600);
//	  HAL_GPIO_WritePin (RGB_G_GPIO_Port ,RGB_G_Pin ,GPIO_PIN_SET );
//	  //HAL_GPIO_WritePin (SPI1_BL_GPIO_Port ,SPI1_BL_Pin ,GPIO_PIN_RESET );  //IPS背光使能
//	  //HAL_Delay (500);
//	  HAL_UART_Transmit(&huart1, "RoboMaster\r\n", 12, 100);
      //HAL_Delay(100);
	  
//	  HAL_UART_Transmit (&huart1,p[0],1,500);
//	   HAL_UART_Transmit (&huart1,(uint8_t *)p,8,500);
//	void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//	{
//		HAL_UART_Transmit_DMA (&huart6 ,receiveDate ,Size);	
//		HAL_UART_Transmit_DMA(&huart1 ,receiveDate ,Size);	

//	    HAL_UARTEx_ReceiveToIdle_DMA  (&huart1 ,receiveDate ,sizeof(receiveDate));
//	    HAL_UARTEx_ReceiveToIdle_DMA  (&huart6 ,receiveDate ,sizeof(receiveDate));
*************************************************************************/



//	}
	  
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
