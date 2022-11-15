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
  * File Name          : FSMC.c
  * Description        : This file provides code for the configuration
  *                      of the FSMC peripheral.
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
#include "fsmc.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SRAM_HandleTypeDef hsram1;

/* FSMC initialization function */
void MX_FSMC_Init(void)
{
  FSMC_NORSRAM_TimingTypeDef Timing;

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_DISABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 0;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 30;
  Timing.BusTurnAroundDuration = 6;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

static uint32_t FSMC_Initialized = 0;

static void HAL_FSMC_MspInit(void){
  /* USER CODE BEGIN FSMC_MspInit 0 */

  /* USER CODE END FSMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct;
  if (FSMC_Initialized) {
    return;
  }
  FSMC_Initialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /** FSMC GPIO Configuration  
  PE3   ------> FSMC_A19
  PE1   ------> FSMC_NBL1
  PE0   ------> FSMC_NBL0
  PD7   ------> FSMC_NE1
  PE4   ------> FSMC_A20
  PB7   ------> FSMC_NL
  PG10   ------> FSMC_NE3
  PD6   ------> FSMC_NWAIT
  PD0   ------> FSMC_D2
  PG9   ------> FSMC_NE2
  PD5   ------> FSMC_NWE
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD3   ------> FSMC_CLK
  PF0   ------> FSMC_A0
  PF2   ------> FSMC_A2
  PF1   ------> FSMC_A1
  PF3   ------> FSMC_A3
  PF4   ------> FSMC_A4
  PF5   ------> FSMC_A5
  PG5   ------> FSMC_A15
  PG4   ------> FSMC_A14
  PG3   ------> FSMC_A13
  PD15   ------> FSMC_D1
  PG2   ------> FSMC_A12
  PG1   ------> FSMC_A11
  PD14   ------> FSMC_D0
  PD13   ------> FSMC_A18
  PF13   ------> FSMC_A7
  PG0   ------> FSMC_A10
  PE13   ------> FSMC_D10
  PD12   ------> FSMC_A17
  PD11   ------> FSMC_A16
  PD10   ------> FSMC_D15
  PF12   ------> FSMC_A6
  PF15   ------> FSMC_A9
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE11   ------> FSMC_D8
  PE14   ------> FSMC_D11
  PD9   ------> FSMC_D14
  PD8   ------> FSMC_D13
  PF14   ------> FSMC_A8
  PE7   ------> FSMC_D4
  PE10   ------> FSMC_D7
  PE12   ------> FSMC_D9
  PE15   ------> FSMC_D12
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_3|FSMC_NBL1_Pin|FSMC_NBL0_Pin|GPIO_PIN_4 
                          |FSMC_D10_Pin|FSMC_D5_Pin|FSMC_D6_Pin|FSMC_D8_Pin 
                          |FSMC_D11_Pin|FSMC_D4_Pin|FSMC_D7_Pin|FSMC_D9_Pin 
                          |FSMC_D12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = FSMC_NE1_Pin|FSMC_NWAIT_Pin|FSMC_D2_Pin|FSMC_NWE_Pin 
                          |FSMC_D3_Pin|FSMC_NOE_Pin|FSMC_CLK_Pin|FSMC_D1_Pin 
                          |FSMC_D0_Pin|GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_11 
                          |FSMC_D15_Pin|FSMC_D14_Pin|FSMC_D13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = FSMC_NL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(FSMC_NL_GPIO_Port, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = FSMC_NE3_Pin|FSMC_NE2_Pin|GPIO_PIN_5|GPIO_PIN_4 
                          |FSMC_A13_Pin|FSMC_A12_Pin|FSMC_A11_Pin|FSMC_A10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = FSMC_A0_Pin|FSMC_A2_Pin|FSMC_A1_Pin|FSMC_A3_Pin 
                          |FSMC_A4_Pin|FSMC_A5_Pin|FSMC_A7_Pin|FSMC_A6_Pin 
                          |FSMC_A9_Pin|FSMC_A8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* USER CODE BEGIN FSMC_MspInit 1 */

  /* USER CODE END FSMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspInit 0 */

  /* USER CODE END SRAM_MspInit 0 */
  HAL_FSMC_MspInit();
  /* USER CODE BEGIN SRAM_MspInit 1 */

  /* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FSMC_DeInitialized = 0;

static void HAL_FSMC_MspDeInit(void){
  /* USER CODE BEGIN FSMC_MspDeInit 0 */

  /* USER CODE END FSMC_MspDeInit 0 */
  if (FSMC_DeInitialized) {
    return;
  }
  FSMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_DISABLE();
  
  /** FSMC GPIO Configuration  
  PE3   ------> FSMC_A19
  PE1   ------> FSMC_NBL1
  PE0   ------> FSMC_NBL0
  PD7   ------> FSMC_NE1
  PE4   ------> FSMC_A20
  PB7   ------> FSMC_NL
  PG10   ------> FSMC_NE3
  PD6   ------> FSMC_NWAIT
  PD0   ------> FSMC_D2
  PG9   ------> FSMC_NE2
  PD5   ------> FSMC_NWE
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD3   ------> FSMC_CLK
  PF0   ------> FSMC_A0
  PF2   ------> FSMC_A2
  PF1   ------> FSMC_A1
  PF3   ------> FSMC_A3
  PF4   ------> FSMC_A4
  PF5   ------> FSMC_A5
  PG5   ------> FSMC_A15
  PG4   ------> FSMC_A14
  PG3   ------> FSMC_A13
  PD15   ------> FSMC_D1
  PG2   ------> FSMC_A12
  PG1   ------> FSMC_A11
  PD14   ------> FSMC_D0
  PD13   ------> FSMC_A18
  PF13   ------> FSMC_A7
  PG0   ------> FSMC_A10
  PE13   ------> FSMC_D10
  PD12   ------> FSMC_A17
  PD11   ------> FSMC_A16
  PD10   ------> FSMC_D15
  PF12   ------> FSMC_A6
  PF15   ------> FSMC_A9
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE11   ------> FSMC_D8
  PE14   ------> FSMC_D11
  PD9   ------> FSMC_D14
  PD8   ------> FSMC_D13
  PF14   ------> FSMC_A8
  PE7   ------> FSMC_D4
  PE10   ------> FSMC_D7
  PE12   ------> FSMC_D9
  PE15   ------> FSMC_D12
  */

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_3|FSMC_NBL1_Pin|FSMC_NBL0_Pin|GPIO_PIN_4 
                          |FSMC_D10_Pin|FSMC_D5_Pin|FSMC_D6_Pin|FSMC_D8_Pin 
                          |FSMC_D11_Pin|FSMC_D4_Pin|FSMC_D7_Pin|FSMC_D9_Pin 
                          |FSMC_D12_Pin);

  HAL_GPIO_DeInit(GPIOD, FSMC_NE1_Pin|FSMC_NWAIT_Pin|FSMC_D2_Pin|FSMC_NWE_Pin 
                          |FSMC_D3_Pin|FSMC_NOE_Pin|FSMC_CLK_Pin|FSMC_D1_Pin 
                          |FSMC_D0_Pin|GPIO_PIN_13|GPIO_PIN_12|GPIO_PIN_11 
                          |FSMC_D15_Pin|FSMC_D14_Pin|FSMC_D13_Pin);

  HAL_GPIO_DeInit(FSMC_NL_GPIO_Port, FSMC_NL_Pin);

  HAL_GPIO_DeInit(GPIOG, FSMC_NE3_Pin|FSMC_NE2_Pin|GPIO_PIN_5|GPIO_PIN_4 
                          |FSMC_A13_Pin|FSMC_A12_Pin|FSMC_A11_Pin|FSMC_A10_Pin);

  HAL_GPIO_DeInit(GPIOF, FSMC_A0_Pin|FSMC_A2_Pin|FSMC_A1_Pin|FSMC_A3_Pin 
                          |FSMC_A4_Pin|FSMC_A5_Pin|FSMC_A7_Pin|FSMC_A6_Pin 
                          |FSMC_A9_Pin|FSMC_A8_Pin);

  /* USER CODE BEGIN FSMC_MspDeInit 1 */

  /* USER CODE END FSMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspDeInit 0 */

  /* USER CODE END SRAM_MspDeInit 0 */
  HAL_FSMC_MspDeInit();
  /* USER CODE BEGIN SRAM_MspDeInit 1 */

  /* USER CODE END SRAM_MspDeInit 1 */
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
