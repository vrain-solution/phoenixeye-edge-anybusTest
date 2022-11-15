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
** COPYRIGHT NOTIFICATION (c) 2014 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** file_description
********************************************************************************
********************************************************************************
*/

#include "abcc_drv_cfg.h"
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_par.h"
#include "abcc.h"


/*******************************************************************************
** Public Services
********************************************************************************
*/

void ABCC_SYS_HWReset( void )
{
   *(volatile UINT32*)(pMemSys + 0x240) |= 0x00000001;
}

void ABCC_SYS_HWReleaseReset( void )
{
   *(volatile UINT32*)(pMemSys + 0x240) &= ~0x00000001;
}

BOOL ABCC_SYS_HwInit( void )
{
   ABCC_SYS_HWReset();

   return TRUE;
}

BOOL ABCC_SYS_Init( void )
{
   return TRUE;
}

void ABCC_SYS_Close( void )
{
}


