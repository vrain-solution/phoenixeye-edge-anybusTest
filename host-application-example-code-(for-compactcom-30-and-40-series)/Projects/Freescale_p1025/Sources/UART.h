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
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
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
** This file describes the interface to the pre-compiled UART library that is
** included in the project.
** NOTE:
** The library is compiled using the Freescale serial example found in
** <CWInstallDir>\PA\PA_Support\Serial\P1025TWR_serial.
** The UART lib-file is found in the Lib folder.
********************************************************************************
********************************************************************************
** Services:
**    InitializeUART                  - Configure UART
**    ReadUARTN                       - Read from UART
**    WriteUARTN()                    - Write to UART
********************************************************************************
*********************************************************************************/
#ifndef UART_H
#define UART_H

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*
** UART error codes
*/
typedef int UARTError;

enum {
   kUARTDataError = -1,
   kUARTNoError = 0,
   kUARTUnknownBaudRate,
   kUARTConfigurationError,
   kUARTBufferOverflow,
   kUARTNoData
};

/*
** Supported baud rates
*/
typedef enum {
	kBaud19200 = 19200,
	kBaud57600 = 57600,
	kBaud115200 = 115200
} UARTBaudRate;

/*******************************************************************************
** Public Services Definitions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Set UART baud rate
**------------------------------------------------------------------------------
** Arguments:
**    baudRate: UART baud rate
**
** Returns:
**    UARTError
**------------------------------------------------------------------------------
*/
UARTError InitializeUART( UARTBaudRate baudRate );

/*------------------------------------------------------------------------------
** Read N bytes from the UART interface
**------------------------------------------------------------------------------
** Arguments:
**    bytes:   Pointer to destination data buffer
**    length:  Number of bytes to receive.
** Returns:
**    UARTError
**------------------------------------------------------------------------------
*/
UARTError ReadUARTN( void* bytes, unsigned long length );

/*------------------------------------------------------------------------------
** Write N bytes to the UART interface
**------------------------------------------------------------------------------
** Arguments:
**    bytes:   Pointer to source data buffer
**    length:  Number of bytes to send
** Returns:
**    UARTError
**------------------------------------------------------------------------------
*/
UARTError WriteUARTN( const void* bytes, unsigned long length );

#endif
