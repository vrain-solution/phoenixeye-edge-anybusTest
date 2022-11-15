/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2015 HMS Industrial Networks AB                 **
**                                                                            **
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, permission is      **
** granted to modify, reproduce and distribute the code in binary form        **
** without any restrictions.                                                  **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
*/

#include "abcc_drv_cfg.h"
#include "abcc_port.h"
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_spi.h"
#include "abcc_sys_adapt_par.h"
#include "abcc_sys_adapt_ser.h"

#include "stm32f4xx_hal.h"
#include "fsmc.h"
#include "spi.h"
#include "usart.h"
extern void ( *ABCC_ISR )( void );

/*******************************************************************************
** Defines
********************************************************************************
*/
#define ABCC_SYS_OM0                   GPIOI, GPIO_PIN_11
#define ABCC_SYS_OM1                   GPIOC, GPIO_PIN_0
#define ABCC_SYS_OM2                   GPIOC, GPIO_PIN_9
#define ABCC_SYS_OM3                   GPIOH, GPIO_PIN_4
#define ABCC_SYS_OM3_POR               GPIOH
#define ABCC_SYS_OM3_PIN               GPIO_PIN_4

#define ABCC_SYS_nRESET                GPIOG, GPIO_PIN_8
#define ABCC_SYS_MD0                   GPIOF, GPIO_PIN_6
#define ABCC_SYS_MD1                   GPIOF, GPIO_PIN_7
#define ABCC_SYS_MI0                   GPIOG, GPIO_PIN_7
#define ABCC_SYS_MI1                   GPIOA, GPIO_PIN_4

#define ABCC_SYS_IRQ_POLL              GPIOF, GPIO_PIN_10

#define ABCC_SYS_IRQ_PIN               GPIO_PIN_10
#define ABCC_SYS_IRQn                  EXTI15_10_IRQn

#define ABCC_SYS_SPI_HANDLE            hspi2
#define ABCC_SYS_SPI_INSTANCE          SPI2

#define ABCC_SYS_SPI_NSS               GPIOI, GPIO_PIN_0
#define ABCC_SYS_SPI_NSS_POR           GPIOI
#define ABCC_SYS_SPI_NSS_PIN           GPIO_PIN_0
#define ABCC_SYS_SPI_NSS_ON            HAL_GPIO_WritePin(ABCC_SYS_SPI_NSS,GPIO_PIN_RESET)
#define ABCC_SYS_SPI_NSS_OFF           HAL_GPIO_WritePin(ABCC_SYS_SPI_NSS,GPIO_PIN_SET)

/*
** Timeout time for performing a blocking SPI transaction
*/
#define ABCC_SYS_SPI_TX_TIMEOUT_MS     1000

/*
** Switch between 4-wire and 3-wire SPI
*/
#define ABCC_SYS_SPI_4WIRE             TRUE

/*
** Switch between SPI Clock Idle Low and Clock Idle High
** configurations. Clock Idle Low is only allowed in 4-wire mode.
** For this particular target Clock Idle Low is the preferred mode
** of operation to avoid a certain startup glitch.
*/
#define ABCC_SYS_SPI_CLK_IDLE_HIGH     TRUE

/*
** If using SPI with DMA, this must be used.
** and we must wait for completion before bringing
** chipselect high and calling the pnDataReadyCbf()
*/
#define ABCC_SYS_SPI_DELAY_CALLBACK    TRUE

#define ABCC_SYS_PAR_SRAM_HANDLE       hsram1
#define ABCC_SYS_PAR_BANK              FSMC_NORSRAM_BANK1
#define ABCC_SYS_PAR_8BIT_EN           GPIOI, GPIO_PIN_9

#define ABCC_SYS_SER_UART_HANDLE       huart6
#define ABCC_SYS_SER_UART              USART6

#define ABCC_SYS_OPT_BOARD_PWR         GPIOG, GPIO_PIN_6


#if( ABCC_CFG_DRV_SPI )
/*
** Callback for SPI data ready indication.
*/
static ABCC_SYS_SpiDataReceivedCbfType pnDataReadyCbf;
#endif

#if( ( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 ) && !( ABCC_CFG_MEMORY_MAPPED_ACCESS ) )
/*
** If not memory mapped temporary storage is needed for pd data.
*/
static UINT8    sys_abReadProcessData[ ABCC_CFG_MAX_PROCESS_DATA_SIZE ];  /* Process data byte array. */
static UINT8    sys_abWriteProcessData[ ABCC_CFG_MAX_PROCESS_DATA_SIZE ]; /* Process data byte array. */
#endif

#if( ABCC_CFG_DRV_SERIAL )
/*
** Callback for Serial data ready indication.
*/
static ABCC_SYS_SerDataReceivedCbfType pnSerDataReadyCbf;
#endif


#if( ABCC_CFG_DRV_SERIAL )
static void ABCC_SYS_UartInit( UINT8 bOpmode )
{
   /* Ensure that OM3 is configured as an Input */

   ABCC_SYS_SER_UART_HANDLE.Instance = ABCC_SYS_SER_UART;

   if( bOpmode == ABP_OP_MODE_SERIAL_19_2 )
   {
      ABCC_SYS_SER_UART_HANDLE.Init.BaudRate = 19200;
   }
   else if( bOpmode == ABP_OP_MODE_SERIAL_57_6 )
   {
      ABCC_SYS_SER_UART_HANDLE.Init.BaudRate = 57600;
   }
   else if( bOpmode == ABP_OP_MODE_SERIAL_115_2 )
   {
      ABCC_SYS_SER_UART_HANDLE.Init.BaudRate = 115200;
   }
   else if (bOpmode == ABP_OP_MODE_SERIAL_625)
   {
      ABCC_SYS_SER_UART_HANDLE.Init.BaudRate = 625000;
   }

   ABCC_SYS_SER_UART_HANDLE.Init.WordLength = UART_WORDLENGTH_8B;
   ABCC_SYS_SER_UART_HANDLE.Init.StopBits = UART_STOPBITS_1;
   ABCC_SYS_SER_UART_HANDLE.Init.Parity = UART_PARITY_NONE;
   ABCC_SYS_SER_UART_HANDLE.Init.Mode = UART_MODE_TX_RX;
   ABCC_SYS_SER_UART_HANDLE.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   ABCC_SYS_SER_UART_HANDLE.Init.OverSampling = UART_OVERSAMPLING_16;
   HAL_UART_Init( &ABCC_SYS_SER_UART_HANDLE );
}
#endif

#if( ABCC_CFG_DRV_SPI )
static void ABCC_SYS_SpiInit(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
#if( ( ABCC_SYS_SPI_4WIRE == FALSE ) || ( ABCC_SYS_SPI_CLK_IDLE_HIGH == TRUE ) )
   UINT8 abDummy[ 1 ] = { 0x00 };
#endif /* ((ABCC_SYS_SPI_4WIRE == FALSE) || (ABCC_SYS_SPI_CLK_IDLE_HIGH == TRUE)) */

   ABCC_SYS_SPI_HANDLE.Instance       = ABCC_SYS_SPI_INSTANCE;
   ABCC_SYS_SPI_HANDLE.Init.Mode      = SPI_MODE_MASTER;
   ABCC_SYS_SPI_HANDLE.Init.Direction = SPI_DIRECTION_2LINES;
   ABCC_SYS_SPI_HANDLE.Init.DataSize  = SPI_DATASIZE_8BIT;
#if( ( ABCC_SYS_SPI_4WIRE == FALSE ) || ( ABCC_SYS_SPI_CLK_IDLE_HIGH == TRUE ) )
   ABCC_SYS_SPI_HANDLE.Init.CLKPolarity = SPI_POLARITY_HIGH;
   ABCC_SYS_SPI_HANDLE.Init.CLKPhase    = SPI_PHASE_2EDGE;
#else  /* ((ABCC_SYS_SPI_4WIRE == FALSE) || (ABCC_SYS_SPI_CLK_IDLE_HIGH == TRUE)) */
   ABCC_SYS_SPI_HANDLE.Init.CLKPolarity = SPI_POLARITY_LOW;
   ABCC_SYS_SPI_HANDLE.Init.CLKPhase    = SPI_PHASE_1EDGE;
#endif /* ((ABCC_SYS_SPI_4WIRE == FALSE) || (ABCC_SYS_SPI_CLK_IDLE_HIGH == TRUE)) */
   ABCC_SYS_SPI_HANDLE.Init.NSS               = SPI_NSS_SOFT;
   ABCC_SYS_SPI_HANDLE.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
   ABCC_SYS_SPI_HANDLE.Init.FirstBit          = SPI_FIRSTBIT_MSB;
   ABCC_SYS_SPI_HANDLE.Init.TIMode            = SPI_TIMODE_DISABLE;
   ABCC_SYS_SPI_HANDLE.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
   ABCC_SYS_SPI_HANDLE.Init.CRCPolynomial     = 10;

   if( HAL_SPI_Init( &ABCC_SYS_SPI_HANDLE ) != HAL_OK )
   {
      Error_Handler();
   }

   /* Setup CS for software CS */
   GPIO_InitStruct.Pin   = ABCC_SYS_SPI_NSS_PIN;
   GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull  = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
   HAL_GPIO_Init( ABCC_SYS_SPI_NSS_POR, &GPIO_InitStruct );

#if( ABCC_SYS_SPI_4WIRE == FALSE )
   ABCC_SYS_SPI_NSS_ON;
#else
   ABCC_SYS_SPI_NSS_OFF;
#endif /* (ABCC_SYS_SPI_4WIRE == FALSE) */

#if( ( ABCC_SYS_SPI_4WIRE == FALSE ) || ( ABCC_SYS_SPI_CLK_IDLE_HIGH == TRUE ) )
   /* Send a dummy byte through SPI, this removes a glitch in SCLK that will
   ** otherwise corrupt the first SPI message */
   HAL_SPI_Transmit( &ABCC_SYS_SPI_HANDLE, abDummy, sizeof( abDummy ), ABCC_SYS_SPI_TX_TIMEOUT_MS );
#endif
}
#endif


#if( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 )
static void ABCC_SYS_FsmcInit( UINT8 bOpmode )
{
   FSMC_NORSRAM_TimingTypeDef Timing;

   /*
   ** Perform the SRAM1 memory initialization sequence
   */
   ABCC_SYS_PAR_SRAM_HANDLE.Instance = FSMC_NORSRAM_DEVICE;
   ABCC_SYS_PAR_SRAM_HANDLE.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
   /* ABCC_SRAM_HANDLE.Init */
   ABCC_SYS_PAR_SRAM_HANDLE.Init.NSBank = ABCC_SYS_PAR_BANK;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.WriteOperation = FSMC_WRITE_OPERATION_DISABLE;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
   ABCC_SYS_PAR_SRAM_HANDLE.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
   /* Timing (TODO: Note! Timings here may not be optimized!) */
   Timing.AddressSetupTime = 10;
   Timing.AddressHoldTime = 10;
   Timing.DataSetupTime = 30;
   Timing.BusTurnAroundDuration = 4;
   Timing.CLKDivision = 2;
   Timing.DataLatency = 1;
   Timing.AccessMode = FSMC_ACCESS_MODE_A;
   /* ExtTiming */

   if( bOpmode == ABP_OP_MODE_8_BIT_PARALLEL)
   {
      ABCC_SYS_PAR_SRAM_HANDLE.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
      HAL_GPIO_WritePin( ABCC_SYS_PAR_8BIT_EN, GPIO_PIN_RESET );
   }
   else if( bOpmode == ABP_OP_MODE_16_BIT_PARALLEL )
   {
      ABCC_SYS_PAR_SRAM_HANDLE.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
      HAL_GPIO_WritePin( ABCC_SYS_PAR_8BIT_EN, GPIO_PIN_SET );
   }

   HAL_SRAM_Init( &ABCC_SYS_PAR_SRAM_HANDLE, &Timing, NULL );
   HAL_SRAM_WriteOperation_Enable( &ABCC_SYS_PAR_SRAM_HANDLE );
}
#endif

BOOL ABCC_SYS_HwInit( void )
{
#if( ABCC_CFG_INT_ENABLED )
   ABCC_SYS_AbccInterruptDisable();
#endif
  /* Power option board */
   HAL_GPIO_WritePin( ABCC_SYS_OPT_BOARD_PWR, GPIO_PIN_RESET );

   /*
   ** Delay to ensure power up time
   */
   HAL_Delay( 5 );
   return TRUE;
}

BOOL ABCC_SYS_Init( void )
{
   return TRUE;
}

void ABCC_SYS_Close( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}

#if( ABCC_CFG_OP_MODE_SETTABLE )
void ABCC_SYS_SetOpmode( UINT8 bOpmode )
{
   HAL_GPIO_WritePin( ABCC_SYS_OM0, GPIO_PIN_RESET );
   HAL_GPIO_WritePin( ABCC_SYS_OM1, GPIO_PIN_RESET );
   HAL_GPIO_WritePin( ABCC_SYS_OM2, GPIO_PIN_RESET );
   HAL_GPIO_WritePin( ABCC_SYS_OM3, GPIO_PIN_RESET );

   /* OM0 */
   if( ( bOpmode & 0x01 ) == 0x01 )
   {
      HAL_GPIO_WritePin( ABCC_SYS_OM0, GPIO_PIN_SET );
   }

   /* OM1 */
   if( ( bOpmode & 0x02 ) == 0x02 )
   {
      HAL_GPIO_WritePin( ABCC_SYS_OM1, GPIO_PIN_SET );
   }

   /* OM2 */
   if( ( bOpmode & 0x04 ) == 0x04 )
   {
      HAL_GPIO_WritePin( ABCC_SYS_OM2, GPIO_PIN_SET );
   }

   /* OM3 */
   if( ( bOpmode & 0x08 ) == 0x08 )
   {
      HAL_GPIO_WritePin( ABCC_SYS_OM3, GPIO_PIN_SET );
   }

   /* Peripherals will be re-initialized according to operating mode */
   if( bOpmode >= ABP_OP_MODE_SERIAL_19_2 )
   {
#if( ABCC_CFG_DRV_SERIAL )
      /* Serial op mode selected. Configure serial port */
      ABCC_SYS_UartInit( bOpmode );
#endif
   }
   else
   {
      HAL_UART_DeInit( &huart6 );
   }

   if( bOpmode == ABP_OP_MODE_SPI )
   {
#if( ABCC_CFG_DRV_SPI )
      ABCC_SYS_SpiInit();
#endif
   }
   else
   {
      HAL_SPI_DeInit( &ABCC_SYS_SPI_HANDLE );
   }

   if( ( ( bOpmode != ABP_OP_MODE_16_BIT_PARALLEL ) &&
         ( bOpmode != ABP_OP_MODE_8_BIT_PARALLEL ) ) )
   {
#ifndef __USE_LCD
      HAL_SRAM_MspDeInit( &ABCC_SYS_PAR_SRAM_HANDLE );
#endif
   }

   if( ( bOpmode == ABP_OP_MODE_16_BIT_PARALLEL ) ||
       ( bOpmode == ABP_OP_MODE_8_BIT_PARALLEL ) )
   {
      /* Initialize parallel controller */
#if( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 )
      ABCC_SYS_FsmcInit( bOpmode );
#endif
   }
   HAL_Delay( 5 );
}
#endif

void ABCC_SYS_HWReset( void )
{
   HAL_GPIO_WritePin( ABCC_SYS_nRESET, GPIO_PIN_RESET );
   HAL_Delay( 1 );
}

void ABCC_SYS_HWReleaseReset( void )
{
   HAL_GPIO_WritePin( ABCC_SYS_nRESET, GPIO_PIN_SET );
   HAL_Delay( 1 );
}


#ifndef ABCC_CFG_ABCC_MODULE_ID
UINT8 ABCC_SYS_ReadModuleId( void )
{
   UINT8 bRet = 0x00;

   if( HAL_GPIO_ReadPin( ABCC_SYS_MI0 ) == GPIO_PIN_SET )
   {
      bRet |= 0x01;
   }

   if( HAL_GPIO_ReadPin( ABCC_SYS_MI1 ) == GPIO_PIN_SET )
   {
      bRet |= 0x02;
   }

   return bRet;
}
#endif

#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
BOOL ABCC_SYS_ModuleDetect( void )
{
   if( (HAL_GPIO_ReadPin(ABCC_SYS_MD0) == GPIO_PIN_RESET) &&
       (HAL_GPIO_ReadPin(ABCC_SYS_MD1) == GPIO_PIN_RESET) )
   {
      return (TRUE);
   }
   else
   {
      return (FALSE);
   }
}
#endif

#if( ABCC_CFG_INT_ENABLED )
void ABCC_SYS_AbccInterruptEnable( void )
{
   HAL_NVIC_EnableIRQ(ABCC_SYS_IRQn);
}


void ABCC_SYS_AbccInterruptDisable( void )
{
   HAL_NVIC_DisableIRQ(ABCC_SYS_IRQn);
}
#endif


#if( ABCC_CFG_POLL_ABCC_IRQ_PIN )
BOOL ABCC_SYS_IsAbccInterruptActive( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
   if( HAL_GPIO_ReadPin( ABCC_SYS_IRQ_POLL ) == GPIO_PIN_RESET )
   {
      return( TRUE );
   }
   else
   {
      return( FALSE );
   }
}
#endif


#if( ABCC_CFG_DRV_SPI )
void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   pnDataReadyCbf = pnDataReceived;
}

void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
   HAL_StatusTypeDef ret;

#if ( ABCC_SYS_SPI_4WIRE )
   /* /SS Low*/
   ABCC_SYS_SPI_NSS_ON;
#endif /* ( ABCC_SYS_SPI_4WIRE ) */

   ret = HAL_SPI_TransmitReceive_DMA( &ABCC_SYS_SPI_HANDLE, ( uint8_t* )pxSendDataBuffer, ( uint8_t* )pxReceiveDataBuffer, iLength );

#if ( !ABCC_SYS_SPI_DELAY_CALLBACK )
#if ( ABCC_SYS_SPI_4WIRE )
   /* /SS High */
   ABCC_SYS_SPI_NSS_OFF;
#endif /* ( ABCC_SYS_SPI_4WIRE ) */
#endif /* ( !ABCC_SYS_SPI_DELAY_CALLBACK ) */

   if( ret != HAL_OK )
   {
      return;
   }

#if ( !ABCC_SYS_SPI_DELAY_CALLBACK )
   /* call back function for SPI receive */
   if( pnDataReadyCbf != NULL )
   {
      pnDataReadyCbf();
   }
   else
   {
      /* Error */
      return;
   }
#endif /* ( !ABCC_SYS_SPI_DELAY_CALLBACK ) */
}

void ABCC_SYS_SpiSendReceiveComplete( void )
{
   /* callback function for SPI receive */
   if( pnDataReadyCbf != NULL )
   {
      pnDataReadyCbf();
   }
   else
   {
      return;
   }

#if ( ABCC_SYS_SPI_4WIRE )
   /* SS High */
   ABCC_SYS_SPI_NSS_OFF;
#endif /* ( ABCC_SYS_SPI_4WIRE ) */
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
   if( hspi->ErrorCode != HAL_OK )
   {
      return;
   }
#if ( ABCC_SYS_SPI_DELAY_CALLBACK )
   else if( hspi == &ABCC_SYS_SPI_HANDLE )
   {
      ABCC_SYS_SpiSendReceiveComplete();
   }
#endif /* ( ABCC_SYS_SPI_DELAY_CALLBACK ) */
}
#endif


#if( ( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 ) && !ABCC_CFG_MEMORY_MAPPED_ACCESS )
void ABCC_SYS_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   __IO UINT8 * pSramAddress = ( UINT8* )( ABCC_CFG_PARALLEL_BASE_ADR+iMemOffset );
   memcpy( pxData, ( void * )pSramAddress, iLength );
}


#if( ABCC_CFG_DRV_PARALLEL_30 )
UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
   UINT8 bData;
   ABCC_SYS_ParallelRead( iMemOffset, &bData, sizeof( UINT8 ) );
   return bData;
}
#endif

#if( ABCC_CFG_DRV_PARALLEL )
UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
   UINT16 iData;
   ABCC_SYS_ParallelRead( iMemOffset, ( UINT8* )&iData, sizeof( UINT16 ) );
   return iData;
}
#endif

void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   __IO UINT8 * pSramAddress = ( uint8_t* )( ABCC_CFG_PARALLEL_BASE_ADR + iMemOffset );
   memcpy( ( void * )pSramAddress, pxData, iLength );
}

#if( ABCC_CFG_DRV_PARALLEL_30 )
void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 pbData )
{
   ABCC_SYS_ParallelWrite( iMemOffset, ( UINT8* )&pbData, sizeof( UINT8 ) );
}
#endif


#if( ABCC_CFG_DRV_PARALLEL )
void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   ABCC_SYS_ParallelWrite( iMemOffset, ( UINT8* )&piData, sizeof( UINT16 ) );
}
#endif


void* ABCC_SYS_ParallelGetRdPdBuffer( void )
{
   return sys_abReadProcessData;
}


void* ABCC_SYS_ParallelGetWrPdBuffer( void )
{
   return sys_abWriteProcessData;
}
#endif


#if( ABCC_CFG_DRV_SERIAL )
void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SerDataReceivedCbfType pnDataReceived  )
{
   pnSerDataReadyCbf = pnDataReceived;
}


void ABCC_SYS_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{
   /* Perform a blocking transmission */
   if( HAL_UART_Transmit( &ABCC_SYS_SER_UART_HANDLE, pxTxDataBuffer, iTxSize, 1000 ) != HAL_OK )
   {
      /* Error */
      return;
   }

   if( HAL_UART_Receive( &ABCC_SYS_SER_UART_HANDLE, pxRxDataBuffer, iRxSize, 1000 ) != HAL_OK )
   {
      /* Error */
      return;
   }

   if( pnSerDataReadyCbf )
   {
      pnSerDataReadyCbf();
   }
}

void ABCC_SYS_SerRestart( void )
{
   /*
   ** Implement according to abcc_sys_adapt_ser.h
   */
}
#endif

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
   switch( GPIO_Pin )
   {
#if( ABCC_CFG_INT_ENABLED )
      case ABCC_SYS_IRQ_PIN:
         /* Anybus CompactCom #IRQ */
         ABCC_ISR();
         break;
      default:
         break;
#endif
   }
}
