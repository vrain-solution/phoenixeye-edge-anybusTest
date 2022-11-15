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
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define TRACE_CLK_Pin GPIO_PIN_2
#define TRACE_CLK_GPIO_Port GPIOE
#define FSMC_NBL1_Pin GPIO_PIN_1
#define FSMC_NBL1_GPIO_Port GPIOE
#define FSMC_NBL0_Pin GPIO_PIN_0
#define FSMC_NBL0_GPIO_Port GPIOE
#define TRST_Pin GPIO_PIN_4
#define TRST_GPIO_Port GPIOB
#define TDO_SWO_Pin GPIO_PIN_3
#define TDO_SWO_GPIO_Port GPIOB
#define FSMC_NE1_Pin GPIO_PIN_7
#define FSMC_NE1_GPIO_Port GPIOD
#define MicroSDCard_CLK_Pin GPIO_PIN_12
#define MicroSDCard_CLK_GPIO_Port GPIOC
#define TDI_Pin GPIO_PIN_15
#define TDI_GPIO_Port GPIOA
#define TCK_SWCLK_Pin GPIO_PIN_14
#define TCK_SWCLK_GPIO_Port GPIOA
#define TMS_SWDIO_Pin GPIO_PIN_13
#define TMS_SWDIO_GPIO_Port GPIOA
#define TRACE_D2_Pin GPIO_PIN_5
#define TRACE_D2_GPIO_Port GPIOE
#define TRACE_D3_Pin GPIO_PIN_6
#define TRACE_D3_GPIO_Port GPIOE
#define I2C1_SDA_Pin GPIO_PIN_9
#define I2C1_SDA_GPIO_Port GPIOB
#define FSMC_NL_Pin GPIO_PIN_7
#define FSMC_NL_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define User_Button_Pin GPIO_PIN_15
#define User_Button_GPIO_Port GPIOG
#define User_Button_EXTI_IRQn EXTI15_10_IRQn
#define SmartCard_CMDVCC_Pin GPIO_PIN_12
#define SmartCard_CMDVCC_GPIO_Port GPIOG
#define FSMC_NE3_Pin GPIO_PIN_10
#define FSMC_NE3_GPIO_Port GPIOG
#define FSMC_NWAIT_Pin GPIO_PIN_6
#define FSMC_NWAIT_GPIO_Port GPIOD
#define FSMC_D2_Pin GPIO_PIN_0
#define FSMC_D2_GPIO_Port GPIOD
#define USART3_TX_Pin GPIO_PIN_11
#define USART3_TX_GPIO_Port GPIOC
#define USART3_TXC10_Pin GPIO_PIN_10
#define USART3_TXC10_GPIO_Port GPIOC
#define FSMC_NE2_Pin GPIO_PIN_9
#define FSMC_NE2_GPIO_Port GPIOG
#define FSMC_NWE_Pin GPIO_PIN_5
#define FSMC_NWE_GPIO_Port GPIOD
#define FSMC_D3_Pin GPIO_PIN_1
#define FSMC_D3_GPIO_Port GPIOD
#define SPI2_MOSI_Pin GPIO_PIN_3
#define SPI2_MOSI_GPIO_Port GPIOI
#define IO_Expander_INT_Pin GPIO_PIN_2
#define IO_Expander_INT_GPIO_Port GPIOI
#define Anti_Tamper_Pin GPIO_PIN_13
#define Anti_Tamper_GPIO_Port GPIOC
#define Enable_8Bit_LED3_Pin GPIO_PIN_9
#define Enable_8Bit_LED3_GPIO_Port GPIOI
#define FSMC_NOE_Pin GPIO_PIN_4
#define FSMC_NOE_GPIO_Port GPIOD
#define FSMC_CLK_Pin GPIO_PIN_3
#define FSMC_CLK_GPIO_Port GPIOD
#define MicroSDCard_CMD_Pin GPIO_PIN_2
#define MicroSDCard_CMD_GPIO_Port GPIOD
#define SmartCard_3_5V_Pin GPIO_PIN_15
#define SmartCard_3_5V_GPIO_Port GPIOH
#define SPI2_SCK_Pin GPIO_PIN_1
#define SPI2_SCK_GPIO_Port GPIOI
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define FSMC_A0_Pin GPIO_PIN_0
#define FSMC_A0_GPIO_Port GPIOF
#define OM0_Pin GPIO_PIN_11
#define OM0_GPIO_Port GPIOI
#define x_MicroSDCard_Detect_Pin GPIO_PIN_13
#define x_MicroSDCard_Detect_GPIO_Port GPIOH
#define NSS_Pin GPIO_PIN_0
#define NSS_GPIO_Port GPIOI
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define OM2_Pin GPIO_PIN_9
#define OM2_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_8
#define MCO_GPIO_Port GPIOA
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define MicroSDCard_D0_Pin GPIO_PIN_8
#define MicroSDCard_D0_GPIO_Port GPIOC
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define FSMC_A2_Pin GPIO_PIN_2
#define FSMC_A2_GPIO_Port GPIOF
#define FSMC_A1_Pin GPIO_PIN_1
#define FSMC_A1_GPIO_Port GPIOF
#define OM3_Pin GPIO_PIN_4
#define OM3_GPIO_Port GPIOH
#define nResetAbcc_Pin GPIO_PIN_8
#define nResetAbcc_GPIO_Port GPIOG
#define FSMC_A3_Pin GPIO_PIN_3
#define FSMC_A3_GPIO_Port GPIOF
#define FSMC_A4_Pin GPIO_PIN_4
#define FSMC_A4_GPIO_Port GPIOF
#define OTG_FS_PowerSwitchOn_Pin GPIO_PIN_5
#define OTG_FS_PowerSwitchOn_GPIO_Port GPIOH
#define MI0_Pin GPIO_PIN_7
#define MI0_GPIO_Port GPIOG
#define OptionCard_PWR_LED1_Pin GPIO_PIN_6
#define OptionCard_PWR_LED1_GPIO_Port GPIOG
#define MD1_Pin GPIO_PIN_7
#define MD1_GPIO_Port GPIOF
#define MD0_Pin GPIO_PIN_6
#define MD0_GPIO_Port GPIOF
#define FSMC_A5_Pin GPIO_PIN_5
#define FSMC_A5_GPIO_Port GPIOF
#define FSMC_A13_Pin GPIO_PIN_3
#define FSMC_A13_GPIO_Port GPIOG
#define nIRQ_Pin GPIO_PIN_10
#define nIRQ_GPIO_Port GPIOF
#define nIRQ_EXTI_IRQn EXTI15_10_IRQn
#define Potentiometer_Pin GPIO_PIN_9
#define Potentiometer_GPIO_Port GPIOF
#define FSMC_D1_Pin GPIO_PIN_15
#define FSMC_D1_GPIO_Port GPIOD
#define FSMC_A12_Pin GPIO_PIN_2
#define FSMC_A12_GPIO_Port GPIOG
#define OM1_Pin GPIO_PIN_0
#define OM1_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_2
#define SW1_GPIO_Port GPIOB
#define FSMC_A11_Pin GPIO_PIN_1
#define FSMC_A11_GPIO_Port GPIOG
#define FSMC_D0_Pin GPIO_PIN_14
#define FSMC_D0_GPIO_Port GPIOD
#define WAKEUP_Pin GPIO_PIN_0
#define WAKEUP_GPIO_Port GPIOA
#define MI1_Pin GPIO_PIN_4
#define MI1_GPIO_Port GPIOA
#define FSMC_A7_Pin GPIO_PIN_13
#define FSMC_A7_GPIO_Port GPIOF
#define FSMC_A10_Pin GPIO_PIN_0
#define FSMC_A10_GPIO_Port GPIOG
#define FSMC_D10_Pin GPIO_PIN_13
#define FSMC_D10_GPIO_Port GPIOE
#define FSMC_D15_Pin GPIO_PIN_10
#define FSMC_D15_GPIO_Port GPIOD
#define FSMC_A6_Pin GPIO_PIN_12
#define FSMC_A6_GPIO_Port GPIOF
#define FSMC_A9_Pin GPIO_PIN_15
#define FSMC_A9_GPIO_Port GPIOF
#define FSMC_D5_Pin GPIO_PIN_8
#define FSMC_D5_GPIO_Port GPIOE
#define FSMC_D6_Pin GPIO_PIN_9
#define FSMC_D6_GPIO_Port GPIOE
#define FSMC_D8_Pin GPIO_PIN_11
#define FSMC_D8_GPIO_Port GPIOE
#define FSMC_D11_Pin GPIO_PIN_14
#define FSMC_D11_GPIO_Port GPIOE
#define FSMC_D14_Pin GPIO_PIN_9
#define FSMC_D14_GPIO_Port GPIOD
#define FSMC_D13_Pin GPIO_PIN_8
#define FSMC_D13_GPIO_Port GPIOD
#define OTG_FS_OverCurrent_Pin GPIO_PIN_11
#define OTG_FS_OverCurrent_GPIO_Port GPIOF
#define OTG_FS_OverCurrent_EXTI_IRQn EXTI15_10_IRQn
#define FSMC_A8_Pin GPIO_PIN_14
#define FSMC_A8_GPIO_Port GPIOF
#define FSMC_D4_Pin GPIO_PIN_7
#define FSMC_D4_GPIO_Port GPIOE
#define FSMC_D7_Pin GPIO_PIN_10
#define FSMC_D7_GPIO_Port GPIOE
#define FSMC_D9_Pin GPIO_PIN_12
#define FSMC_D9_GPIO_Port GPIOE
#define FSMC_D12_Pin GPIO_PIN_15
#define FSMC_D12_GPIO_Port GPIOE
#define SPI2_MISO_Pin GPIO_PIN_14
#define SPI2_MISO_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
