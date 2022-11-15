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
** This is the public header file for the Functional Safety object.
********************************************************************************
********************************************************************************
** Services:
**    SAFE_ProcessCmdMsg() - Processes commands sent to the Functional Safety
**                           object.
********************************************************************************
********************************************************************************
*/
#ifndef SAFE_OBJ_H
#define SAFE_OBJ_H

#include "abcc_obj_cfg.h"

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
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** The function that processes the commands sent to the Functional Safety
** object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void SAFE_ProcessCmdMsg( ABP_MsgType* psNewMessage );

/*------------------------------------------------------------------------------
** The function will control the ABP_SAFE_IA_SAFETY_ENABLED attribute
** value in runtime.
**------------------------------------------------------------------------------
** Arguments:
**    fEnabled    - TRUE: Safety is enabled; FALSE: Safety is disabled
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
#if( SAFE_IA_SAFETY_ENABLED_ENABLE )
EXTFUNC void SAFE_SetSafetyEnable( BOOL fEnabled );
#endif

#endif  /* inclusion lock */
