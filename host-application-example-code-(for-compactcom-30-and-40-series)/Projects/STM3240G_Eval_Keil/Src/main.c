/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */

/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "appl_abcc_handler.h"
#include "Serial.h"
#ifdef __USE_LCD
#include "GLCD.h"
#endif
#include "abcc_versions.h"
#include "appl_adi_config.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* ABCC defines */
#define APPL_TIMER_MS         1
#define USE_TIMER_INTERRUPT   1

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/


/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_BOARD_SPECIFIC )
void DemoInit( void )
{
   UINT8 abData[2] = {0xFF, 0x00};
   #ifdef __USE_LCD
   UINT8 bOpMode;
   #endif

   /* Link UART3 to STDIO and print a basic message */
   SER_LinkStdio(&huart3);
   printf("HMS Industrial Networks\n\r");
   printf("Embedded ABCC Demo\n\r");
   printf("STM3240G-EVAL with ABCC Option Card\n\r");
   printf( ABCC_STARTER_KIT_VER_STRING );
   printf("\n\r\n\r");

   #ifdef __USE_LCD
   /* Initialize the staticly displayed region of the LCD */
   GLCD_Init();
   GLCD_Clear(White);
   GLCD_SetBackColor(Blue);
   GLCD_SetTextColor(White);
   GLCD_DisplayString(0, 0, 1, (unsigned char *)"    HMS Networks    ");
   GLCD_DisplayString(1, 0, 1, (unsigned char *)" Embedded ABCC Demo ");
   GLCD_DisplayString(2, 0, 1, (unsigned char *)"  hms-networks.com  ");
   GLCD_SetTextColor(White);
   GLCD_SetBackColor(Black);
   GLCD_ClearLn (9, 0);
   GLCD_ClearLn (10, 0);
   bOpMode = ABCC_GetOpmode();
   if( bOpMode == ABP_OP_MODE_16_BIT_PARALLEL )
   {
      GLCD_DisplayString(10, 0, 0, (unsigned char *)"16-BIT PARALLEL MODE");
   }
   else if( bOpMode == ABP_OP_MODE_8_BIT_PARALLEL )
   {
      GLCD_DisplayString(10, 0, 0, (unsigned char *)"8-BIT PARALLEL MODE");
   }
   else if( bOpMode == ABP_OP_MODE_SPI )
   {
      GLCD_DisplayString(10, 0, 0, (unsigned char *)"SPI MODE");
   }
   else if ( ( bOpMode == ABP_OP_MODE_SERIAL_19_2 )  ||
             ( bOpMode == ABP_OP_MODE_SERIAL_57_6 )  ||
             ( bOpMode == ABP_OP_MODE_SERIAL_115_2 ) ||
             ( bOpMode == ABP_OP_MODE_SERIAL_625 ) )
   {
      GLCD_DisplayString(10, 0, 0, (unsigned char *)"SERIAL MODE");
   }
   GLCD_DisplayString(9, 0, 0, (unsigned char *)ABCC_STARTER_KIT_VER_STRING );
   GLCD_SetBackColor(White);
   GLCD_SetTextColor(Blue);
   GLCD_DisplayString(4, 0, 1, (unsigned char *)"Pot Val:");
   GLCD_DisplayString(5, 0, 1, (unsigned char *)"Key State  :");
   GLCD_DisplayString(6, 0, 1, (unsigned char *)"4-LED State:");
   GLCD_DisplayString(7, 0, 1, (unsigned char *)"8-LED State:");
   GLCD_DisplayString(8, 0, 1, (unsigned char *)"8-I/O State:");
   GLCD_DisplayString(9, 0, 1, (unsigned char *)"TIM Cnt:");
   GLCD_SetTextColor(Red);
   GLCD_DisplayString(5, 13, 1, (unsigned char *)"FALSE");
   GLCD_DisplayString(6, 13, 1, (unsigned char *)"0x0");
   GLCD_DisplayString(7, 13, 1, (unsigned char *)"0x00");
   GLCD_DisplayString(8, 13, 1, (unsigned char *)"0x00");
   GLCD_DisplayString(9, 9,  1, (unsigned char *)"0x00000000");
   #endif
   /* Reset the I2C I/O-Expanders to default 'off' state */
   HAL_I2C_Master_Transmit(&hi2c1, 0x44 , abData, 2, 1000);
   abData[0] = 0xFF;
   abData[1] = 0xFF;
   HAL_I2C_Master_Transmit(&hi2c1, 0x40 , abData, 2, 1000);

}
#endif /* ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_BOARD_SPECIFIC ) */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
   /*
   ** Make sure the ABCC reset signal is kept low by the platform specific
   ** initialization to keep the ABCC module in reset until the driver releases
   ** it.
   */
   APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_MODULE_NO_ERROR;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_ADC3_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM5_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  if( ABCC_HwInit() != ABCC_EC_NO_ERROR )
  {
     Error_Handler();
  }

#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_BOARD_SPECIFIC )
   DemoInit();
#endif /* ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_BOARD_SPECIFIC ) */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
   while(eAbccHandlerStatus == APPL_MODULE_NO_ERROR )
   {
      eAbccHandlerStatus = APPL_HandleAbcc();

#if( !USE_TIMER_INTERRUPT )
      ABCC_RunTimerSystem( APPL_TIMER_MS );
      HAL_Delay( APPL_TIMER_MS );
#endif
      switch( eAbccHandlerStatus )
      {
      case APPL_MODULE_NO_ERROR:
         break;
      case APPL_MODULE_RESET:
         HAL_NVIC_SystemReset();
         break;
      case APPL_MODULE_NOT_DETECTED:
      case APPL_MODULE_NOT_SUPPORTED:
      case APPL_MODULE_NOT_ANSWERING:
      case APPL_MODULE_SHUTDOWN:
      case APPL_MODULE_UNEXPECTED_ERROR:
      default:
         printf( "\nAPPL_HandleAbcc() returned status code: %d\n", eAbccHandlerStatus );
         break;
      }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
   }

   #ifdef __USE_LCD
   GLCD_Clear(Black);
   GLCD_SetBackColor(Red);
   GLCD_SetTextColor(White);
   GLCD_DisplayString(5, 0, 1, (unsigned char *)" APPLICATION HALTED ");
   #endif

   printf("Application HALTED!\n\n");
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

void HAL_SYSTICK_Callback(void)
{
   /* call timer functions */
   #if( USE_TIMER_INTERRUPT )
   ABCC_RunTimerSystem( APPL_TIMER_MS );
   #endif
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
