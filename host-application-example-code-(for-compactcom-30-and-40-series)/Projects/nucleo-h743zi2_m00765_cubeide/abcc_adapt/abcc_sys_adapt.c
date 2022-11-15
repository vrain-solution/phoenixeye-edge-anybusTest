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
*/

#include "abcc.h"

#include "abcc_sys_adapt_spi.h"
#include "abcc_sys_adapt_ser.h"

#include <stm32h7xx_hal.h>
#include "abcc_sys_adapt_aux.h"

#include "main.h"

/*******************************************************************************
** Private defines
********************************************************************************
*/

/*
** The I2C addresses for the U8 + U9 I/O expanders are hardwired on the board.
*/
#define M00765_U8_I2C_ADDRESS                      ( 0x4e )
#define M00765_U9_I2C_ADDRESS                      ( 0x4c )

/*******************************************************************************
** Private typedefs
********************************************************************************
*/

/*
** Structures for the I/O on the M00765 board. All of those are connected to
** the onboard I2C I/O expanders. The storage structured must be manually
** synchronised with the I/O expanders using the 'refresh' functions declared
** below.
**
** TODO - Is 'volatile' needed for any of those?
*/
typedef struct I2C_InputsTag
{
   UINT8    bS2;     /* S2 - "HEX 1"   U8, P0[3..0] */
   UINT8    bS3;     /* S3 - "HEX 2"   U8, P0[7..4] */
   UINT8    bS4;     /* S4 - "OpMode"  U9, P0[7..4] */
   UINT8    bS5;     /* S5 - "GPI"     U9, P0[3..0] */
   UINT8    bJP11;   /* JP11 - "GPIO"  U8, P1[5..0] */
   UINT8    bMI;     /* ABCC MI1 & MI0 U9, P1[5..4] */
   UINT8    bMD;     /* ABCC MD1 & MD0 U9, P1[7..6] */
} I2C_InputsType;
typedef struct I2C_OutputsTag
{
   UINT8    bJP11;   /* JP11 - "GPIO", U8, P0[5..0] */
   UINT8    bLEDs;   /* D3-D6 - "GPO", U9, P1[3..0] */
} I2C_OutputsType;

/*******************************************************************************
** Private function prototypes
********************************************************************************
*/

/*******************************************************************************
** Private globals
********************************************************************************
*/

/*
** "SPI/UART transaction complete" callback function pointer.
*/
static ABCC_SYS_SpiDataReceivedCbfType    pnTransferCompleteCbf = NULL;

/*
** The CubeMX HAL handles for the IO interfaces that the M00765 PCB has been
** wired to. Needs to be assigned from "main()" before anything can be done
** here with those interfaces.
*/
static I2C_HandleTypeDef*     pxI2C_Handle = NULL;
#if( ABCC_CFG_DRV_SPI )
static SPI_HandleTypeDef*     pxSPI_Handle = NULL;
#endif
#if( ABCC_CFG_DRV_SERIAL )
static UART_HandleTypeDef*    pxUART_Handle = NULL;
#endif

/*
** Intermediate buffers for the SPI and UART transfers.
**
** The default linker settings in CubeIDE seems to assign all runtime variables
** to the DTCM-RAM, but the DMA controllers that services the I/O interfaces
** can't reach that memory directly.
**
** Adding appropriate 'section' attributes to the in/out buffer definitions in
** "abcc_spi_drv.c" and "abcc_serial_drv.c" would solve this, but will collide
** with how the present directory structure for the example code works. On the
** repository level the "abcc_drv" tree is common for all ports and therefore
** cannot have port-specific additions in them.
**
** As a workaround a second set of local buffers are defined here, which are
** explicitly placed in the corresponding SRAM banks which the DMA controllers
** can reach. One extra data synchronisation cycle is needed before/after each
** DMA-driven I/O transfer cycle to copy data between the DTCM-RAM and the
** SRAM, and this is presently made by "memcpy()" calls. This could also be
** made via the MDMA controller, but the benefit of this is probably too small
** to justify the extra complexity, and in a 'live' implementation one would
** rather solve this by placing the SPI/UART memory buffers in the appropriate
** SRAM area by editing those files.
**
** Also note that the default linker directives does not define the "sram1"
** or "sram4" sections by default.
**
** For more details, refer to chapter "2 - Memory and bus architecture" in
** "RM0433 - Reference manual, STM32H742, STM32H743/753 and STM32H750 Value
** line advanced Arm®-based 32-bit MCUs"
*/

#if( ABCC_CFG_DRV_SPI )

/*
** Size macros has been copied from "abcc_spi_drv.c"
*/
#define NUM_BYTES_2_WORDS(x) ( ( (x) + 1 ) >> 1 )
#define CRC_WORD_LEN_IN_WORDS 2
#define MAX_PAYLOAD_WORD_LEN ( ( NUM_BYTES_2_WORDS( ABCC_CFG_SPI_MSG_FRAG_LEN ) ) + ( NUM_BYTES_2_WORDS( ABCC_CFG_MAX_PROCESS_DATA_SIZE ) ) + ( CRC_WORD_LEN_IN_WORDS ) )

__attribute__((section(".sram1"))) static UINT8 abSpiMosiDmaBuff[ ( 5 + MAX_PAYLOAD_WORD_LEN ) * ABP_UINT16_SIZEOF ];
__attribute__((section(".sram1"))) static UINT8 abSpiMisoDmaBuff[ ( 5 + MAX_PAYLOAD_WORD_LEN ) * ABP_UINT16_SIZEOF ];

#endif

#if( ABCC_CFG_DRV_SERIAL )

/*
** Size macros has been copied from "abcc_serial_drv.c"
*/
#define SER_CMD_STAT_REG_LEN                       ( ABP_UINT8_SIZEOF )
#define SER_MSG_FRAG_LEN                           ( 16 * ABP_UINT8_SIZEOF )
#if( ABCC_CFG_MAX_PROCESS_DATA_SIZE > ABP_MAX_PROCESS_DATA )
   #define SER_MAX_PD_LEN                          ( ABP_MAX_PROCESS_DATA )
#else
   #define SER_MAX_PD_LEN                          ABCC_CFG_MAX_PROCESS_DATA_SIZE
#endif
#define SER_CRC_LEN                                ( ABP_UINT16_SIZEOF )

__attribute__((section(".sram4"))) static UINT8 abUartTxDmaBuff[ SER_CMD_STAT_REG_LEN + SER_MSG_FRAG_LEN + SER_MAX_PD_LEN + SER_CRC_LEN ];
__attribute__((section(".sram4"))) static UINT8 abUartRxDmaBuff[ SER_CMD_STAT_REG_LEN + SER_MSG_FRAG_LEN + SER_MAX_PD_LEN + SER_CRC_LEN ];

#endif

/*
** Copies of the destination and size for the received SPI/UART frame data,
** required to save because the 'copy RX data' work is made in a separate
** function compared to the 'send/receive data'.
**
** TODO - Is 'volatile' needed for any of those?
*/
static void*   pxInboundDest;
static UINT16  iInboundSize;

/*******************************************************************************
** Public globals
********************************************************************************
*/

/*
** Buffer space for the I2C expander input/output data.
*/
static I2C_InputsType   sI2C_Inputs;
static I2C_OutputsType  sI2C_Outputs;

/*******************************************************************************
** Private functions
********************************************************************************
*/

/*******************************************************************************
** Public functions
********************************************************************************
*/

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_Set_I2C_Handle( I2C_HandleTypeDef* pxNewHandle )
{
   if( ( ( pxI2C_Handle == NULL ) && ( pxNewHandle != NULL ) ) ||
       ( ( pxI2C_Handle != NULL ) && ( pxNewHandle == NULL ) ) )
   {
      pxI2C_Handle = pxNewHandle;
      return( TRUE );
   }

   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_PARAMETER_NOT_VALID, (UINT32)pxNewHandle );
   return( FALSE );
}

#if( ABCC_CFG_DRV_SPI )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_Set_SPI_Handle( SPI_HandleTypeDef* pxNewHandle )
{
   if( ( ( pxSPI_Handle == NULL ) && ( pxNewHandle != NULL ) ) ||
       ( ( pxSPI_Handle != NULL ) && ( pxNewHandle == NULL ) ) )
   {
      pxSPI_Handle = pxNewHandle;
      return( TRUE );
   }

   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_PARAMETER_NOT_VALID, (UINT32)pxNewHandle );
   return( FALSE );
}
#endif

#if( ABCC_CFG_DRV_SERIAL )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_Set_UART_Handle( UART_HandleTypeDef* pxNewHandle )
{
   if( ( ( pxUART_Handle == NULL ) && ( pxNewHandle != NULL ) ) ||
       ( ( pxUART_Handle != NULL ) && ( pxNewHandle == NULL ) ) )
   {
      pxUART_Handle = pxNewHandle;
      return( TRUE );
   }

   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_PARAMETER_NOT_VALID, (UINT32)pxNewHandle );
   return( FALSE );
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_Refresh_I2C_In( void )
{
   HAL_StatusTypeDef    xHalStatus;
   UINT8                abBuffer[ 2 ];

   if( pxI2C_Handle == NULL )
   {
      /* No I2C interface handle registered, this should not happen! */
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
      return( FALSE );
   }

   /* Read both ports of U8 and copy the data to the correct variables. */
   xHalStatus = HAL_I2C_Master_Receive( pxI2C_Handle, M00765_U8_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return( FALSE );
   }

   /* S2, P0[3..0] */
   sI2C_Inputs.bS2 = abBuffer[ 0 ] & 0xf;

   /* S3, P0[7..4] */
   sI2C_Inputs.bS3 = ( abBuffer[ 0 ] & 0xf0 ) >> 4;

   /* JP11, P1[5..0] */
   sI2C_Inputs.bJP11 = abBuffer[ 1 ] & 0x3f;

   /* Read both ports of U8 and copy the data to the correct variables. */
   xHalStatus = HAL_I2C_Master_Receive( pxI2C_Handle, M00765_U9_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return( FALSE );
   }

   /* S5, P0[3..0] */
   sI2C_Inputs.bS5 = abBuffer[ 0 ] & 0xf;

   /* S4, P0[7..4], bit needs to be reversed for OM0 to end up at LSB. */
   sI2C_Inputs.bS4 = 0;
   if( ( abBuffer[ 0 ] & 0x10 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x8;
   }
   if( ( abBuffer[ 0 ] & 0x20 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x4;
   }
   if( ( abBuffer[ 0 ] & 0x40 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x2;
   }
   if( ( abBuffer[ 0 ] & 0x80 ) != 0 )
   {
      sI2C_Inputs.bS4 |= 0x1;
   }

   /* ABCC MI1 & MI0, P1[5..4] */
   sI2C_Inputs.bMI = ( abBuffer[ 1 ] & 0x30 ) >> 4;

   /* ABCC MD1 & MD0, P1[7..6] */
   sI2C_Inputs.bMD = ( abBuffer[ 1 ] & 0xc0 ) >> 6;

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_Refresh_I2C_Out( void )
{
   HAL_StatusTypeDef    xHalStatus;
   UINT8                abBuffer[ 2 ];

   if( pxI2C_Handle == NULL )
   {
      /* No I2C interface handle registered, this should not happen! */
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
      return( FALSE );
   }

   /*
   ** NOTE: Bits that should be used as inputs must be set 'high'. Check the
   ** datasheet for the NXP PCF8575 I/O expander for more information.
   */

   /* U8, P0[7..0] are inputs. */
   abBuffer[ 0 ] = 0xff;

   /* U8, P1[7..6] are inputs. */
   abBuffer[ 1 ] = 0xc0;

   /* JP11, GPIO, only bits 5..0 can be used as outputs. */
   abBuffer[ 1 ] |= sI2C_Outputs.bJP11 & 0x3f;

   /* Write both ports of U8. */
   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U8_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return( FALSE );
   }

   /* U9, P0[7..0] are inputs. */
   abBuffer[ 0 ] = 0xff;

   /* U9, P1[7..4] are inputs. */
   abBuffer[ 1 ] = 0xf0;

   /* LEDs D3-D6, active low. */
   abBuffer[ 1 ] |= ( ~(sI2C_Outputs.bLEDs) & 0x0f );

   /* Write both ports of U9. */
   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U9_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return( FALSE );
   }

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt_aux.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_GetRestartButton( void )
{
   if( HAL_GPIO_ReadPin( M00765_BTN_S1_GPIO_Port, M00765_BTN_S1_Pin ) == GPIO_PIN_RESET )
   {
      return( TRUE );
   }

   return( FALSE );
}

#if( ABCC_CFG_DRV_SPI )
/*------------------------------------------------------------------------------
** See "stm32...hal_spi.h" and "stm32...hal_spi.c" for more information.
**------------------------------------------------------------------------------
*/
void HAL_SPI_TxRxCpltCallback( SPI_HandleTypeDef* pxHandle )
{
   if( pxHandle == pxSPI_Handle )
   {
      /* Deactivate SS, i.e. set it high. */
      HAL_GPIO_WritePin( M00765_SPI_SS_GPIO_Port, M00765_SPI_SS_Pin, GPIO_PIN_SET );

      /* Did the SPI transfer finish correctly? */
      if( pxHandle->ErrorCode != HAL_OK )
      {
         return;
      }

      /* Copy the MISO data from the I/O buffer (SRAMx) to the driver buffer (DTCM-RAM). */
      memcpy( pxInboundDest, abSpiMisoDmaBuff, iInboundSize );

      /* And notify the driver that we have a MISO frame to process. */
      if( pnTransferCompleteCbf != NULL )
      {
         pnTransferCompleteCbf();
         return;
      }
      else
      {
         /* No completion callback registered, this should not happen! */
         ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
         return;
      }
   }
   else
   {
      /*
      ** Handle other SPI callbacks.
      **
      ** PORTING ALERT!
      **
      ** This function, and/or the contents of it, will probably have to be
      ** placed elsewhere if the DMAx controllers are used for other
      ** operations than the ABCC.
      */
   }
}
#endif

#if( ABCC_CFG_DRV_SERIAL )
/*------------------------------------------------------------------------------
** See "stm32...hal_uart.h" and "stm32...hal_uart.c"for more information.
**------------------------------------------------------------------------------
*/
void HAL_UART_RxCpltCallback( UART_HandleTypeDef* pxHandle )
{
   UINT32   lTemp;

   if( pxHandle == pxUART_Handle )
   {
      lTemp = pxHandle->ErrorCode;

      /*
      ** Terminate any ongoing UART DMA operations. Either this or something
      ** similar seems to be required in order to get the HAL 'gState' of the
      ** UART back to HAL_UART_STATE_READY, it does not seem to happen on its
      ** own when the DMA transfers has been finished.
      **
      ** TODO - Is there a better way to do this?
      */
      HAL_UART_Abort( pxUART_Handle );

      /* Did the UART transfer finish correctly? */
      if( lTemp != HAL_OK )
      {
         return;
      }

      /* Copy the RX data from the I/O buffer (SRAMx) to the driver buffer (DTCM-RAM). */
      memcpy( pxInboundDest, abUartRxDmaBuff, iInboundSize );

      /* And notify the driver that we have a RX frame to process. */
      if( pnTransferCompleteCbf != NULL )
      {
         pnTransferCompleteCbf();
         return;
      }
      else
      {
         /* No completion callback registered, this should not happen! */
         ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
         return;
      }
   }
   else
   {
      /*
      ** Handle other UART callbacks.
      **
      ** PORTING ALERT!
      **
      ** This function, and/or the contents of it, will probably have to be
      ** placed elsewhere if the DMAx controllers are used for other
      ** operations than the ABCC.
      */
   }
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_HwInit( void )
{
   HAL_StatusTypeDef    xHalStatus;
   UINT8                abBuffer[ 2 ];

   if( pxI2C_Handle == NULL )
   {
      /* No I2C interface handle registered, this should not happen! */
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
      return( FALSE );
   }

   /*
   ** Reset the I2C IO expanders to 'all inputs'. Check the datasheet for the
   ** NXP PCF8575 I/O expander for more information.
   */
   abBuffer[ 0 ] = 0xFF;
   abBuffer[ 1 ] = 0xFF;

   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U8_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return( FALSE );
   }

   xHalStatus = HAL_I2C_Master_Transmit( pxI2C_Handle, M00765_U9_I2C_ADDRESS, abBuffer, 2, 1000 );
   if( xHalStatus != HAL_OK )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return( FALSE );
   }

   /*
   ** Refresh the I2C inputs so that MD and MI contains up-to-date values.
   */
   if( !ABCC_SYS_Refresh_I2C_In() )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, 0 );
      return( FALSE );
   }

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_Init( void )
{
   /* Pull the RESET signal low. */
   HAL_GPIO_WritePin( M00765_RESET_GPIO_Port, M00765_RESET_Pin, GPIO_PIN_RESET );

   return( TRUE );
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_Close( void )
{
   /* Nothing to do. */
}

#if( ABCC_CFG_OP_MODE_SETTABLE )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SetOpmode( UINT8 bOpmode )
{
   HAL_StatusTypeDef    xHalStatus;

   /*
   ** Step 1, update the OM pins on the ABCC.
   */

   if( ( bOpmode & 0x1 ) != 0 )
   {
      HAL_GPIO_WritePin( M00765_OM0_GPIO_Port, M00765_OM0_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM0_GPIO_Port, M00765_OM0_Pin, GPIO_PIN_RESET );
   }

   if( ( bOpmode & 0x2 ) != 0 )
   {
      HAL_GPIO_WritePin( M00765_OM1_GPIO_Port, M00765_OM1_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM1_GPIO_Port, M00765_OM1_Pin, GPIO_PIN_RESET );
   }

   if( ( bOpmode & 0x4 ) != 0 )
   {
      HAL_GPIO_WritePin( M00765_OM2_GPIO_Port, M00765_OM2_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM2_GPIO_Port, M00765_OM2_Pin, GPIO_PIN_RESET );
   }

   if( ( bOpmode & 0x8 ) != 0 ) {
      HAL_GPIO_WritePin( M00765_OM3_GPIO_Port, M00765_OM3_Pin, GPIO_PIN_SET );
   }
   else
   {
      HAL_GPIO_WritePin( M00765_OM3_GPIO_Port, M00765_OM3_Pin, GPIO_PIN_RESET );
   }

   /*
   ** Step 2, any other OpMode-dependent initialisations that are needed.
   */

   switch( bOpmode )
   {
#if( ABCC_CFG_DRV_SPI )
   case ABP_OP_MODE_SPI:

      /* Is any handle registered for the HW interface in question? */
      if( pxSPI_Handle == NULL )
      {
         /* No SPI interface handle registered, this should not happen! */
         ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
         return;
      }

      /* Update any interface-dependent settings that depends on the OpMode. */

      /* Re-initialise the interface HW with the new settings. */

   break;
#endif
#if( ABCC_CFG_DRV_SERIAL )
   case ABP_OP_MODE_SERIAL_19_2:
   case ABP_OP_MODE_SERIAL_57_6:
   case ABP_OP_MODE_SERIAL_115_2:
   case ABP_OP_MODE_SERIAL_625:

      /* Is any handle registered for the HW interface in question? */
      if( pxUART_Handle == NULL )
      {
         /* No UART interface handle registered, this should not happen! */
         ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
         return;
      }

      /* Update any interface-dependent settings that depends on the OpMode. */
      switch( bOpmode )
      {
      case ABP_OP_MODE_SERIAL_19_2:
         pxUART_Handle->Init.BaudRate = 19200;
         break;
      case ABP_OP_MODE_SERIAL_57_6:
         pxUART_Handle->Init.BaudRate = 57600;
         break;
      case ABP_OP_MODE_SERIAL_115_2:
         pxUART_Handle->Init.BaudRate = 115200;
         break;
      case ABP_OP_MODE_SERIAL_625:
         pxUART_Handle->Init.BaudRate = 625000;
         break;
      default:
         ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_INCORRECT_OPERATING_MODE, 0 );
         break;
      }

      /* Re-initialise the interface HW with the new settings. */
      xHalStatus = HAL_UART_Init( pxUART_Handle );
      if( xHalStatus != HAL_OK )
      {
         ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
         return;
      }

      break;
#endif
   default:
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_INCORRECT_OPERATING_MODE, 0 );
      break;
   }

   return;
}
#endif

#if( ABCC_CFG_OP_MODE_GETTABLE )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
UINT8 ABCC_SYS_GetOpmode( void )
{
   if( !ABCC_SYS_Refresh_I2C_In() )
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, 0 );
      return( 0xf ); /* Service mode. */
   }

   return( sI2C_Inputs.bS4 );
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_HWReset( void )
{
   HAL_GPIO_WritePin( M00765_RESET_GPIO_Port, M00765_RESET_Pin, GPIO_PIN_RESET );
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_HWReleaseReset( void )
{
   HAL_GPIO_WritePin( M00765_RESET_GPIO_Port, M00765_RESET_Pin, GPIO_PIN_SET );
}

#ifndef ABCC_CFG_ABCC_MODULE_ID
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
UINT8 ABCC_SYS_ReadModuleId( void )
{
   return( sI2C_Inputs.bMI );
}
#endif

#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_ModuleDetect( void )
{
   if( sI2C_Inputs.bMD == 0 )
   {
      return( TRUE );
   }

   return( FALSE );
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SyncInterruptEnable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SyncInterruptDisable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif

#if( ABCC_CFG_INT_ENABLED )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_AbccInterruptEnable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_AbccInterruptDisable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif

#if( ABCC_CFG_POLL_ABCC_IRQ_PIN )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
BOOL ABCC_SYS_IsAbccInterruptActive( void )
{
   if( HAL_GPIO_ReadPin( M00765_IRQ_GPIO_Port, M00765_IRQ_Pin ) == GPIO_PIN_RESET )
   {
      return( TRUE );
   }

   return( FALSE );
}
#endif

#if( ABCC_CFG_DRV_SPI )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   if( pnDataReceived != NULL )
   {
      pnTransferCompleteCbf = pnDataReceived;
   }
   else
   {
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_PARAMETER_NOT_VALID, 0 );
   }

   return;
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
   HAL_StatusTypeDef    xHalStatus;

   if( pxSPI_Handle == NULL )
   {
      /* No SPI interface handle registered, this should not happen! */
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
      return;
   }

   /* Save the destination pointer and size for later. */
   pxInboundDest = pxReceiveDataBuffer;
   iInboundSize = iLength;

   /* Activate SS, i.e. set it low. */
   /* TODO - The SPI interface should have some kind of logic to directly    */
   /* TODO - control the SS signal, investigate if this can be used instead. */
   HAL_GPIO_WritePin( M00765_SPI_SS_GPIO_Port, M00765_SPI_SS_Pin, GPIO_PIN_RESET );

   /* Copy the MOSI data from the driver buffer (DTCM-RAM) to the I/O buffer (SRAMx). */
   memcpy( abSpiMosiDmaBuff, pxSendDataBuffer, iLength );

   /* Start the SPI transfer. */
   xHalStatus = HAL_SPI_TransmitReceive_DMA( pxSPI_Handle, abSpiMosiDmaBuff, abSpiMisoDmaBuff, iLength );
   if( xHalStatus != HAL_OK )
   {
      /* SPI transfer could not be started. */
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );

      /* Deactivate SS, i.e. set it high. */
      HAL_GPIO_WritePin( M00765_SPI_SS_GPIO_Port, M00765_SPI_SS_Pin, GPIO_PIN_SET );
   }

   return;
}
#endif

#if( ( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 ) && !ABCC_CFG_MEMORY_MAPPED_ACCESS )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}

#if( ABCC_CFG_DRV_PARALLEL_30 )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

#if( ABCC_CFG_DRV_PARALLEL )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}

#if( ABCC_CFG_DRV_PARALLEL_30 )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 pbData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

#if( ABCC_CFG_DRV_PARALLEL )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void* ABCC_SYS_ParallelGetRdPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void* ABCC_SYS_ParallelGetWrPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

#if( ABCC_CFG_DRV_SERIAL )
/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   if( pnDataReceived != NULL )
   {
      pnTransferCompleteCbf = pnDataReceived;
      return;
   }

   ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_PARAMETER_NOT_VALID, (UINT32)pnDataReceived );
   return;
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{
   HAL_StatusTypeDef    xHalStatus;

   if( pxUART_Handle == NULL )
   {
      /* No UART interface handle registered, this should not happen! */
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
      return;
   }

   /* Save the destination pointer and size for later. */
   pxInboundDest = pxRxDataBuffer;
   iInboundSize = iRxSize;

   /* Copy the TX data from the driver buffer (DTCM-RAM) to the I/O buffer (SRAMx). */
   memcpy( abUartTxDmaBuff, pxTxDataBuffer, iTxSize );

   /* Start the UART transmission. */
   xHalStatus = HAL_UART_Transmit_DMA( pxUART_Handle, abUartTxDmaBuff, iTxSize );
   if( xHalStatus != HAL_OK )
   {
      /* UART transmit could not be executed. */
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return;
   }

   /* Start the UART reception. */
   xHalStatus = HAL_UART_Receive_DMA( pxUART_Handle, abUartRxDmaBuff, iRxSize );
   if( xHalStatus != HAL_OK )
   {
      /* UART transmit could not be executed. */
      ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_SYS_ADAPTION_ERR, (UINT32)xHalStatus );
      return;
   }

   return;
}

/*------------------------------------------------------------------------------
** See "abcc_sys_adapt.h" for more information.
**------------------------------------------------------------------------------
*/
void ABCC_SYS_SerRestart( void )
{
   if( pxUART_Handle == NULL )
   {
      /* No UART interface handle registered, this should not happen! */
      ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_NO_RESOURCES, 0 );
      return;
   }

   /* Terminate any ongoing UART operations. */
   HAL_UART_Abort( pxUART_Handle );

   return;
}
#endif
