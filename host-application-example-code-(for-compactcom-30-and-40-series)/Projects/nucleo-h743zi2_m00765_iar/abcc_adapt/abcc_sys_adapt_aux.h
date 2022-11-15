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
** COPYRIGHT NOTIFICATION (c) 2021 HMS Industrial Networks AB                 **
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
** Defines data structures and service functions for the M00765 adapter board.
********************************************************************************
********************************************************************************
** Services:
**
** ABCC_SYS_Set_I2C_Handle()     - Assign the I2C HAL handle.
** ABCC_SYS_Set_SPI_Handle()     - Assign the SPI HAL handle.
** ABCC_SYS_Set_UART_Handle()    - Assign the UART HAL handle.
**
** ABCC_SYS_Refresh_I2C_In()     - Read input data from I2C IO
** ABCC_SYS_Refresh_I2C_Out()    - Write output data to I2C IO
**
** ABCC_SYS_GetRestartButton()   - Get the state of the S1 button.
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_SYS_ADAPT_AUX
#define ABCC_SYS_ADAPT_AUX

/*******************************************************************************
** Public defines.
********************************************************************************
*/

/*******************************************************************************
** Public typedefs.
********************************************************************************
*/

/*******************************************************************************
** Public globals.
********************************************************************************
*/

/*******************************************************************************
** Public functions.
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Register the CubeMX HAL handle to use for each communication interface to
** use with the M00765 board.
**------------------------------------------------------------------------------
** Arguments:
**    pxNewHandle - Pointer to the struct/handle that represents the selected
**                    interface hardware.
**
** Returns:
**    TRUE if the handle could be changed, FALSE otherwise.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_SYS_Set_I2C_Handle( I2C_HandleTypeDef* pxNewHandle );
#if( ABCC_CFG_DRV_SPI )
EXTFUNC BOOL ABCC_SYS_Set_SPI_Handle( SPI_HandleTypeDef* pxNewHandle );
#endif
#if( ABCC_CFG_DRV_SERIAL )
EXTFUNC BOOL ABCC_SYS_Set_UART_Handle( UART_HandleTypeDef* pxNewHandle );
#endif

/*------------------------------------------------------------------------------
** Synchronise in/out data with the I2C I/O expanders on the M00765 board.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE if the data could be read/written, FALSE otherwise.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_SYS_Refresh_I2C_In( void );
EXTFUNC BOOL ABCC_SYS_Refresh_I2C_Out( void );

/*------------------------------------------------------------------------------
** Returns the state of the S1 / RESTART button on the M00765 board.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE when the button is down, FALSE when it is up.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_SYS_GetRestartButton( void );

#endif  /* inclusion lock */
