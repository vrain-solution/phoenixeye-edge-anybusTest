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
** COPYRIGHT NOTIFICATION (c) 2019 HMS Industrial Networks AB                 **
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
** Platform dependent implementation of abcc_time functions
********************************************************************************
********************************************************************************
*/

#include "abcc_time.h"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>


/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

BOOL ABCC_TIME_GetDateAndTime( ABCC_TIME_DateAndTimeType* psTime )
{
   SYSTEMTIME  sLocalTime;
   memset( &sLocalTime, 0, sizeof( sLocalTime ) );

   GetLocalTime( &sLocalTime );

   psTime->iYear = (UINT16)sLocalTime.wYear;
   psTime->bMonth = (UINT8)sLocalTime.wMonth;
   psTime->bDay = (UINT8)sLocalTime.wDay;
   psTime->bHour = (UINT8)sLocalTime.wHour;
   psTime->bMinute = (UINT8)sLocalTime.wMinute;
   psTime->bSecond = (UINT8)sLocalTime.wSecond;

   return TRUE;
}

BOOL ABCC_TIME_SetDateAndTime( ABCC_TIME_DateAndTimeType* psTime )
{
   SYSTEMTIME  sLocalTime;
   memset( &sLocalTime, 0, sizeof( sLocalTime ) );

   sLocalTime.wYear = (WORD)psTime->iYear;
   sLocalTime.wMonth = (WORD)psTime->bMonth;
   sLocalTime.wDay = (WORD)psTime->bDay;
   sLocalTime.wHour = (WORD)psTime->bHour;
   sLocalTime.wMinute = (WORD)psTime->bMinute;
   sLocalTime.wSecond = (WORD)psTime->bSecond;

   return (BOOL)SetLocalTime( &sLocalTime );
}

/*******************************************************************************
** Tasks
********************************************************************************
*/
