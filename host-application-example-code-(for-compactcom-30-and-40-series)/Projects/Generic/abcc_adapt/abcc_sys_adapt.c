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

#include "spi_IF.h"
#include "GPIO_IF.h"
void* spi_handl_;

BOOL ABCC_SYS_HwInit( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */

   // GPIOピンの設定
   gpio_if_init();

   // SPI初期化
   spi_handl_ = spi_if_init();

   if(spi_handl_ == NULL){
      // 失敗
      return FALSE;
   }

//   // GPIOピンの設定
//   gpio_if_init();

   return TRUE;
}


BOOL ABCC_SYS_Init( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */

   // RESETピンをLOWにする
   gpio_if_HWReset();
   
   return TRUE;
}


void ABCC_SYS_Close( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
  gpio_if_cleanup();
}


#if( ABCC_CFG_OP_MODE_SETTABLE )
void ABCC_SYS_SetOpmode( UINT8 bOpmode )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


#if( ABCC_CFG_OP_MODE_GETTABLE )
UINT8 ABCC_SYS_GetOpmode( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


void ABCC_SYS_HWReset( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */

   // RESETピンをLOWにする
   gpio_if_HWReset();
}


void ABCC_SYS_HWReleaseReset( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
   // RESETピンをHIGHにする
   gpio_if_HWReleaseReset();
}


#ifndef ABCC_CFG_ABCC_MODULE_ID
UINT8 ABCC_SYS_ReadModuleId( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
   UINT8 mi = gpio_if_ReadMI();
   printf("mi = %d\n",mi);

   return mi;
}
#endif


#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
BOOL ABCC_SYS_ModuleDetect( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
void ABCC_SYS_SyncInterruptEnable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
void ABCC_SYS_SyncInterruptDisable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


#if( ABCC_CFG_INT_ENABLED )
void ABCC_SYS_AbccInterruptEnable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}


void ABCC_SYS_AbccInterruptDisable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


#if( ABCC_CFG_POLL_ABCC_IRQ_PIN )
BOOL ABCC_SYS_IsAbccInterruptActive( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
  static int last_irq = 1;
  int irq = gpio_if_ReadIRQ();
  if( last_irq != irq ){
    printf("irq=%d->%d\n",last_irq, irq);
    last_irq = irq;
  }

   // 割り込みはLOW     Active low interrupt signal
  if( irq == 0){
      return( TRUE );
  } else {
      return( FALSE );
  }
}
#endif

static ABCC_SYS_SpiDataReceivedCbfType pnDataReadyCbf;

#if( ABCC_CFG_DRV_SPI )
void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   /*
   ** Implement according to abcc_sys_adapt_spi.h
   */
   pnDataReadyCbf = pnDataReceived;
}

#include <unistd.h>
void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_spi.h
   */
#if 1
   spi_if_xfer(spi_handl_, (UINT8*)pxSendDataBuffer, (UINT8)iLength, (UINT8*)pxReceiveDataBuffer, (UINT8)iLength);
#else
   spi_if_xfer(spi_handl_, (UINT8*)pxSendDataBuffer, (UINT8)iLength, NULL, (UINT8)iLength);
   sleep(2);
   spi_if_xfer(spi_handl_, NULL, (UINT8)iLength, (UINT8*)pxReceiveDataBuffer, (UINT8)iLength);

#endif
   pnDataReadyCbf();
}
#endif


#if( ( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 ) && !ABCC_CFG_MEMORY_MAPPED_ACCESS )
void ABCC_SYS_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


#if( ABCC_CFG_DRV_PARALLEL_30 )
UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_PARALLEL )
UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


#if( ABCC_CFG_DRV_PARALLEL_30 )
void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 pbData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_PARALLEL )
void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

void* ABCC_SYS_ParallelGetRdPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


void* ABCC_SYS_ParallelGetWrPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_SERIAL )
void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   /*
   ** Implement according to abcc_sys_adapt_ser.h
   */
}


void ABCC_SYS_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{
   /*
   ** Implement according to abcc_sys_adapt_ser.h
   */
}


void ABCC_SYS_SerRestart( void )
{
   /*
   ** Implement according to abcc_sys_adapt_ser.h
   */
}
#endif
