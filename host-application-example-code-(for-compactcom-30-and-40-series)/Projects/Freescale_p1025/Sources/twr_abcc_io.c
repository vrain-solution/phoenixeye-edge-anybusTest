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
*/

#include "string.h"

#include "abcc_td.h"
#include "abcc_sys_adapt_spi.h"
#include "i2cdrv.h"
#include "twr_abcc_io.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** I2C addresses of the I2C IO Expander circuits mounted on the Tower ABCC
** Interface board
**------------------------------------------------------------------------------
*/
typedef enum twr_abcc_io_Expander
{
   TWR_ABCC_IO_EXPANDER_U8 = 0x27,
   TWR_ABCC_IO_EXPANDER_U9 = 0x26
}
twr_abcc_io_ExpanderType;

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

/*
** I2C handler for adapter IO expander.
*/
static I2CDRV_HandlerType I2C1Handler;

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

BOOL TWR_ABCC_IO_Init( void )
{
   static const UINT8 twr_abcc_io_abExpander1InitValue[ 2 ] = { 0xFF, 0xFF };
   static const UINT8 twr_abcc_io_abExpander2InitValue[ 2 ] = { 0xFF, 0xFF };

   if( !I2CDRV_Init( &I2C1Handler, 1 ) )
   {
      return( FALSE );
   }

   /*
   ** Configure the I/O expanders
   */
   if( !I2CDRV_Write( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U8, twr_abcc_io_abExpander1InitValue, 2 ) )
   {
      return( FALSE );
   }

   if( !I2CDRV_Write( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U9, twr_abcc_io_abExpander2InitValue, 2 ) )
   {
      return( FALSE );
   }

   return( TRUE );
}

BOOL TWR_ABCC_IO_GetAbccOpMode( UINT8* pbOpMode )
{
   UINT8 bBitNo;
   UINT8 abOpMode[ 2 ];

   *pbOpMode = 0;

   if( !I2CDRV_Read( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U9, abOpMode, 2 ) )
   {
      return( FALSE );
   }

   abOpMode[ 0 ] >>= 4;
   abOpMode[ 0 ] &= 0x0F;

   for( bBitNo = 0; bBitNo < 4; bBitNo++ )
   {
      if( abOpMode[ 0 ] & ( 1 << bBitNo ) )
      {
         *pbOpMode |= ( 1 << ( 3 - bBitNo ) );
      }
   }

   return( TRUE );
}

BOOL TWR_ABCC_IO_GetAbccModuleId( UINT8* pbModuleId )
{
   UINT8 abModuleId[ 2 ];

   if( !I2CDRV_Read( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U9, abModuleId, 2 ) )
   {
      return( FALSE );
   }

   *pbModuleId = ( abModuleId[ 1 ] >> 4 ) & 0x03;

   return( TRUE );
}

BOOL TWR_ABCC_IO_GetAbccModuleDetect( UINT8* pbModuleDetect )
{
   UINT8 abModuleDetect[ 2 ];

   if( !I2CDRV_Read( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U9, abModuleDetect, 2 ) )
   {
      return( FALSE );
   }

   *pbModuleDetect = ( abModuleDetect[ 1 ] >>= 6 ) & 0x03;

   return( TRUE );
}

BOOL TWR_ABCC_IO_GetPianoDipSwitch( UINT8* pbInputBits )
{
   UINT8 abInputBits[ 2 ];

   if( !I2CDRV_Read( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U9, abInputBits, 2 ) )
   {
      return( FALSE );
   }

   *pbInputBits = ( abInputBits[ 0 ] ^ 0x0F ) & 0x0F;

   return( TRUE );
}

BOOL TWR_ABCC_IO_GetRotarySwitch1( UINT8* pbRotarySwitch1 )
{
   UINT8 abSwitch1[ 2 ];

   if( !I2CDRV_Read( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U8, abSwitch1, 2 ) )
   {
      return( FALSE );
   }

   *pbRotarySwitch1 = abSwitch1[ 0 ] & 0x0F;

   return( TRUE );
}

BOOL TWR_ABCC_IO_GetRotarySwitch2( UINT8* pbRotarySwitch2 )
{
   UINT8 abSwitch2[ 2 ];

   if( !I2CDRV_Read( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U8, abSwitch2, 2 ) )
   {
      return( FALSE );
   }

   *pbRotarySwitch2 = ( abSwitch2[ 0 ] >> 4 ) & 0x0F;

   return( TRUE );
}

BOOL TWR_ABCC_IO_SetLedOutputs( UINT8 bOutputBits )
{
   UINT8 abOutputBits[ 2 ];

   abOutputBits[ 0 ] = 0xFF;
   abOutputBits[ 1 ] = ( 0xF0 | ( ( bOutputBits ^ 0x0F ) & 0x0F ) );

   if( !I2CDRV_Write( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U9, abOutputBits, 2 ) )
   {
      return( FALSE );
   }

   return( TRUE );
}

BOOL TWR_ABCC_IO_SetGeneralOutputs( UINT8 bOutputBits )
{
   UINT8 abOutputBits[ 2 ];

   abOutputBits[ 0 ] = 0xFF;
   abOutputBits[ 1 ] = bOutputBits | 0xC0;

   if( !I2CDRV_Write( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U8, abOutputBits, 2 ) )
   {
      return( FALSE );
   }

   return( TRUE );
}

BOOL TWR_ABCC_IO_GetBoardId( TWR_ABCC_IO_BoardIdType* peBoardId )
{
   UINT8 abPins[ 2 ];

   if( !I2CDRV_Read( &I2C1Handler, (UINT8)TWR_ABCC_IO_EXPANDER_U8, abPins, 2 ) )
   {
      return( FALSE );
   }

   *peBoardId = ( ( abPins[ 1 ] >> 6 ) & 0x03 );

   return( TRUE );
}

BOOL TWR_ABCC_IO_CheckOpmode( UINT8 bOpmode )
{
   TWR_ABCC_IO_BoardIdType eBoardId;
   TWR_ABCC_IO_GetBoardId( &eBoardId );

   /*
   ** Verify the mounted Tower ABCC Interface board is compatible with the
   ** chosen operating mode.
   */
   switch( bOpmode )
   {
   case ABP_OP_MODE_16_BIT_PARALLEL:

      if( eBoardId != TWR_ABCC_IO_BOARD_ID_16BIT )
      {
         return( FALSE );
      }
      break;

   case ABP_OP_MODE_SERIAL_115_2:
   case ABP_OP_MODE_SERIAL_19_2:
   case ABP_OP_MODE_SERIAL_57_6:
   case ABP_OP_MODE_SERIAL_625:

      return( TRUE );
      break;

   case ABP_OP_MODE_8_BIT_PARALLEL:

      if( eBoardId != TWR_ABCC_IO_BOARD_ID_8BIT )
      {
         return( FALSE );
      }
      break;

   case ABP_OP_MODE_SPI:

      if( eBoardId != TWR_ABCC_IO_BOARD_ID_SPI )
      {
         return( FALSE );
      }

      return( TRUE );
      break;

   default:

      return( FALSE );
      break;
   }

   return( TRUE );
}

/*******************************************************************************
** Tasks
********************************************************************************
*/

