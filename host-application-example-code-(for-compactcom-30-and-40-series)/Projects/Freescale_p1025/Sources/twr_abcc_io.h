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
** Access to the IO of the TWR-ABCC Interface board
********************************************************************************
********************************************************************************
** Services:
********************************************************************************
********************************************************************************
*/

#ifndef TWR_ABCC_IO_H_
#define TWR_ABCC_IO_H_

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Tower ABCC Interface board identification numbers
**------------------------------------------------------------------------------
*/
typedef enum TWR_ABCC_IO_BoardId
{
   TWR_ABCC_IO_BOARD_ID_16BIT,
   TWR_ABCC_IO_BOARD_ID_8BIT,
   TWR_ABCC_IO_BOARD_ID_SPI
}
TWR_ABCC_IO_BoardIdType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Handles initialization of I2C driver and configures the I/O expanders
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    TRUE  - Initialization successful
**    FALSE - Initialization failed
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_Init( void );

/*------------------------------------------------------------------------------
** Retrieves the ABCC module ID from the host connector
**------------------------------------------------------------------------------
** Arguments:
**    pbModuleId - Pointer to buffer to write the module ID to
**
** Returns:
**    TRUE  - The Module ID was retrieved successfully and written to pbModuleId
**    FALSE - Failed to retrieve the Module ID
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_GetAbccModuleId( UINT8* pbModuleId );

/*------------------------------------------------------------------------------
** Retrieves the Module Detect value of the host connector
**------------------------------------------------------------------------------
** Arguments:
**    pbModuleDetect - Pointer to buffer to write the Module detect value.
**                     This is a bit field of 2 bits. If the bits are set to 0
**                     a module is mounted, else the bits will be set to 1.
**
** Returns:
**    TRUE  - The Module Detect was retrieved successfully and written to
**            pbModuleDetect
**    FALSE - Failed to retrieve the Module Detect
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_GetAbccModuleDetect( UINT8* pbModuleDetect );

/*------------------------------------------------------------------------------
** Retrieves the value of the Operating mode switch
**------------------------------------------------------------------------------
** Arguments:
**    pbOpMode - Pointer to buffer to write the Operating Mode to
**
** Returns:
**    TRUE  - The Operating Mode was retrieved successfully and written to
**            pbOpMode
**    FALSE - Failed to retrieve the Operating Mode
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_GetAbccOpMode( UINT8* pbOpMode );

/*------------------------------------------------------------------------------
** Retrieves the value of the General input DIP switch (4 bits)
**------------------------------------------------------------------------------
** Arguments:
**    pbInputBits - Pointer to buffer to write the input bits (bit 0-3)
**
** Returns:
**    TRUE  - The Input bits were retrieved successfully and written to
**            pbInputBits
**    FALSE - Failed to retrieve the Input bits
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_GetPianoDipSwitch( UINT8* pbInputBits );

/*------------------------------------------------------------------------------
** Sets the value of the genral output contact
**------------------------------------------------------------------------------
** Arguments:
**    bOutputBits - General output value (bit 0-5)
**
** Returns:
**    TRUE  - The Output bits were set successfully to the hardware
**    FALSE - Failed to set the Output bits
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_SetGeneralOutputs( UINT8 bOutputBits );

/*------------------------------------------------------------------------------
** Sets the value of the General output LEDs (4 bits)
**------------------------------------------------------------------------------
** Arguments:
**    bOutputBits - General output value (bit 0-3)
**
** Returns:
**    TRUE  - The Output bits were set successfully to the hardware
**    FALSE - Failed to set the Output bits
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_SetLedOutputs( UINT8 bOutputBits );

/*------------------------------------------------------------------------------
** Retrieves the id of the board mounted to the tower elevator
**------------------------------------------------------------------------------
** Arguments:
**    eBoardId - Pointer to a buffer where the board it will be written to
**
** Returns:
**    TRUE  - The board ID was retrieved successfully
**    FALSE - Failed to retrieve the board ID
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_GetBoardId( TWR_ABCC_IO_BoardIdType* eBoardId );


/*------------------------------------------------------------------------------
** Retrieves the value of rotary switch 1 on the adapter board.
**------------------------------------------------------------------------------
** Arguments:
**    pbRotarySwitch1 - Pointer to a buffer where the switch it will be written to
**
** Returns:
**    TRUE  - The value was retrieved successfully
**    FALSE - Failed to retrieve value
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_GetRotarySwitch1( UINT8* pbRotarySwitch1 );


/*------------------------------------------------------------------------------
** Retrieves the value of rotary switch 2 on the adapter board.
**------------------------------------------------------------------------------
** Arguments:
**    pbRotarySwitch1 - Pointer to a buffer where the switch it will be written to
**
** Returns:
**    TRUE  - The value was retrieved successfully
**    FALSE - Failed to retrieve value
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_GetRotarySwitch2( UINT8* pbRotarySwitch2 );


/*------------------------------------------------------------------------------
** Checks if the id of the board mounted to the tower elevator supports
** the operting mode
**------------------------------------------------------------------------------
** Arguments:
**    bOpmode - Operting mode
**
** Returns:
**    TRUE  - The board ID and opmode is compliant.
**    FALSE - The board ID and opmode is NOT compliant.
**------------------------------------------------------------------------------
*/
BOOL TWR_ABCC_IO_CheckOpmode( UINT8 bOpmode );


#endif  /* inclusion lock */
