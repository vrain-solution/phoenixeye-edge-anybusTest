/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
//////////////////////////
//  Project Stationery  //
//////////////////////////

#include "stdio.h"
#include "abcc_td.h"

#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "appl_abcc_handler.h"
#include "UART.h"
#include "p1025.h"


#if SMPTARGET
#include "smp_target.h"
#endif
typedef void (IntHndlr)(long);

extern void InterruptHandler(long cause);
asm void system_call();

asm void system_call()
{
    nofralloc
    sc
    blr
}

/*------------------------------------------------------------------------------
** Setup timer for HW delay support ( HW_DELAY_US() in p1025.h
** Global timer 0 and 1 in register group A are used.
** Timer clock source: CBB clock divided by 8 = 33,3 MHz.
** CCB clock: SYS clock * 4
** SYS clock: 66.667 MHz
** To be able to use rollover behavior ( 0 -> 0xFFFFFFFF ) for timer 0,
** timer 0 and timer 1 are cascaded together. Timer 0 is used by HW_DELAY_US()
**------------------------------------------------------------------------------
** Arguments:
**   None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void InitHwTimer( void )
{
   PIC_TCR( 0 ) |= ( PIC_TCR_ROVR_TIMER_0 | PIC_TCR_CASC_TIMER_0_1 );

   /*
   ** Start timer 0 and 1. Timer 1 need to have a dummy reload value > 0.
   */
   PIC_GTBCRA( 0 ) = 0;
   PIC_GTBCRA( 1 ) = 1;
}

void main()
{
   /*
   ** Make sure the ABCC reset signal is kept low by the platform specific
   ** initialization to keep the ABCC module in reset until the driver releases
   ** it.
   */

   UINT8 bSwitch1;
   UINT8 bSwitch2;
   UINT16 iCount = 0;
   APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_MODULE_NO_ERROR;
   const UINT16 iSleepTimeMS = 10;

   if( ABCC_HwInit() != ABCC_EC_NO_ERROR )
   {
      return;
   }

   InitHwTimer();

   while( eAbccHandlerStatus == APPL_MODULE_NO_ERROR )
   {
      if( ( iCount++ % 128 ) == 0 )
      {
         /*
         ** Poll switches every second to set IP/node address and baud rate
         ** If not applicable for the network, the values will be ignored
         */
         if( TWR_ABCC_IO_GetRotarySwitch1( &bSwitch1 ) )
         {
            APPL_SetAddress( bSwitch1 );
         }
         if( TWR_ABCC_IO_GetRotarySwitch2( &bSwitch2 ) )
         {
            APPL_SetBaudrate( bSwitch2 );
         }
      }

      eAbccHandlerStatus = APPL_HandleAbcc();
      ABCC_RunTimerSystem( iSleepTimeMS );
      HW_DELAY_US(  iSleepTimeMS * 1000 );
   }

   if( eAbccHandlerStatus == APPL_MODULE_RESET )
   {
      /*
      ** Request HW reset of P1025.
      */
      HW_GUTS_RSTCR = HW_RSTCR_HRESET_REQ;
      while( 1 ){}
   }
 }
