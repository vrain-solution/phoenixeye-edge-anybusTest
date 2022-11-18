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
** Main program
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "appl_abcc_handler.h"

#define APPL_TIMER_MS         1
#define USE_TIMER_INTERRUPT   1

#if( USE_TIMER_INTERRUPT )
static void TimerIsr( void )
{
   ABCC_RunTimerSystem( APPL_TIMER_MS );
}

static void SetupTimerInterrupt( void )
{
}
#else
static void DelayMs( UINT32 lDelayMs )
{
}
#endif

static void Reset( void )
{
}

int main()
{
   /*
   ** Make sure the ABCC reset signal is kept low by the platform specific
   ** initialization to keep the ABCC module in reset until the driver releases
   ** it.
   */

   printf("hellow anybus\n");
   APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_MODULE_NO_ERROR;

   if( ABCC_HwInit() != ABCC_EC_NO_ERROR )
   {
      printf("ABCC_HwInit ERROR\n");
      return( 0 );
   }

#if( USE_TIMER_INTERRUPT )
   SetupTimerInterrupt();
#endif

   while( eAbccHandlerStatus == APPL_MODULE_NO_ERROR )
   {
      eAbccHandlerStatus = APPL_HandleAbcc();
      printf("eAbccHandlerStatus %d\n", eAbccHandlerStatus);

#if( !USE_TIMER_INTERRUPT )
      ABCC_RunTimerSystem( APPL_TIMER_MS );
      DelayMs( APPL_TIMER_MS );
#endif

      switch( eAbccHandlerStatus )
      {
      case APPL_MODULE_NO_ERROR:
         break;
      case APPL_MODULE_RESET:
         Reset();
         break;
      case APPL_MODULE_NOT_DETECTED:
      case APPL_MODULE_NOT_SUPPORTED:
      case APPL_MODULE_NOT_ANSWERING:
      case APPL_MODULE_SHUTDOWN:
      case APPL_MODULE_UNEXPECTED_ERROR:
      default:
         break;
      }
   }

   return( 0 );
}
