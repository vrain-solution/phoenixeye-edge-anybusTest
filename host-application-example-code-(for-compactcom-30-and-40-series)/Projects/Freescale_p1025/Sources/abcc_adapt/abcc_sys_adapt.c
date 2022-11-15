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
** System adaption for twr p1025.
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_spi.h"
#include "abcc_sys_adapt_ser.h"
#include "abcc.h"
#include "UART.h"
#include "p1025.h"
#include "twr_abcc_io.h"
#include "i2cdrv.h"

/*******************************************************************************
** Constants
********************************************************************************
*/
static ABCC_SYS_SpiDataReceivedCbfType pnDataReadyCbf;
static ABCC_SYS_SerDataReceivedCbfType pnSerDataReceived;

/*
** Mapping of OM and reset pins on IO expander.
*/
#define HW_GPIO_OM0        HW_GPIO_EXPAND2
#define HW_GPIO_OM1        HW_GPIO_EXPAND12
#define HW_GPIO_OM2        HW_GPIO_EXPAND1
#define HW_GPIO_OM3        HW_GPIO_EXPAND13
#define HW_GPIO_RESET      HW_GPIO_EXPAND5

/*
** I2C handler for TWRP1025 IO expander.
*/
static I2CDRV_HandlerType I2C2Handler;

/*
**  HW shadow registers for IO expander.
*/
static UINT8 bTWRIOExpander_port0 = 0;
static UINT8 bTWRIOExpander_port1 = 0;

/*
*/
static UINT8 appl_bOpmode = 0;


/*******************************************************************************
** Private services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** If UART operating mode is used this function must be called to
** initialize the UART interface with the correct baud rate
**------------------------------------------------------------------------------
** Arguments:
**       bOpmode - Operating mode to set correct baud rate
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void InitSerialOpmode( UINT8 bOpmode )
{
   switch ( bOpmode)
   {
   case ABP_OP_MODE_SERIAL_19_2:
      InitializeUART(  kBaud19200 );
      break;
   case ABP_OP_MODE_SERIAL_57_6:
      InitializeUART(  kBaud57600 );
      break;
   case ABP_OP_MODE_SERIAL_115_2:
      InitializeUART(  kBaud115200 );
      break;
   default:
      APPL_UnexpectedError();
      break;
   }
}

/*------------------------------------------------------------------------------
** Init I2C IO expander on p1025 tower board.
** OM0..3 -> output.
** reset  -> output.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void HW_ConfigIoExpander( void )
{
   UINT8 abI2CData[ 3 ];

   I2CDRV_Init( &I2C2Handler, 2 );
   /*
   ** Setup OM0..3 to outputs, reset pin to output
   */
   abI2CData[ 0 ] = HW_IO_EXPAND_CMD_CONFIG_PORT0;

   abI2CData[ 1 ] = ~( HW_GPIO_OM2 | HW_GPIO_RESET | HW_GPIO_OM0 );
   abI2CData[ 2 ] =  ~( HW_GPIO_OM1 | HW_GPIO_OM3 );

   I2CDRV_Write( &I2C2Handler,  HW_IO_EXPAND_I2C_ADDR, abI2CData, 3 );
}


/*******************************************************************************
** Public Services
********************************************************************************
*/

void ABCC_SYS_HWReset( void )
{
   UINT8 abI2CData[ 2 ] = { 0, 0 };
   /*
   ** Set reset pin to low.
   */
   bTWRIOExpander_port0 &= ~HW_GPIO_RESET;

   /*
   ** H_RESET pin is set low. OM pins are dont care
   */
   abI2CData[ 0 ] = HW_IO_EXPAND_CMD_OUT_PORT0;
   abI2CData[ 1 ] = bTWRIOExpander_port0;
   I2CDRV_Write( &I2C2Handler,  HW_IO_EXPAND_I2C_ADDR, abI2CData, 2 );
}

void ABCC_SYS_HWReleaseReset( void )
{
   UINT8 abI2CData[ 2 ] = { 0, 0 };

   /*
   ** Only release reset if the board id is compliant
   ** with current operating mode.
   */
   if( !TWR_ABCC_IO_CheckOpmode( appl_bOpmode ) )
   {
      /*
      ** Inform main loop that some thing is wrong.
      */
      APPL_UnexpectedError();
      return;
   }

   bTWRIOExpander_port0 |= HW_GPIO_RESET;

   /*
   ** H_RESET in the host connector is connected to HW_GPIO_RESET which is IO0_5.
   */
   abI2CData[ 0 ] = HW_IO_EXPAND_CMD_OUT_PORT0;
   abI2CData[ 1 ] = bTWRIOExpander_port0;

   I2CDRV_Write( &I2C2Handler,  HW_IO_EXPAND_I2C_ADDR, abI2CData, 2 );

}

#if( ABCC_CFG_OP_MODE_SETTABLE )
void ABCC_SYS_SetOpmode( UINT8 bOpmode )
{
   UINT8 abI2CData[ 2 ] = { 0, 0 };

   /*
   ** Store local copy of operating mode.
   */
   appl_bOpmode = bOpmode;

   /*
   ** Clear OM bits in shadow register.
   */
   bTWRIOExpander_port0 &= ~( HW_GPIO_OM0 | HW_GPIO_OM2 );
   bTWRIOExpander_port1 &= ~( HW_GPIO_OM1 | HW_GPIO_OM3 );

   /*
    ** OM0 is connected to GPIO_EXPAND7 which is IO0_7. PORT0
    ** OM1 is connected to GPIO_EXPAND12 which is IO0_12. PORT1
    ** OM2 is connected to GPIO_EXPAND1 which is IO0_1. PORT0
    ** OM3 is connected to GPIO_EXPAND13 which is IO0_13. PORT1
    */
   abI2CData[ 0 ] = HW_IO_EXPAND_CMD_OUT_PORT0;

   /*
   ** OM0
   */
   if( bOpmode & 0x1 )
   {
      bTWRIOExpander_port0 |= HW_GPIO_OM0;
   }

   /*
    ** OM2
    */
   if( bOpmode & 0x04 )
   {
       bTWRIOExpander_port0 |= HW_GPIO_OM2;
   }

   abI2CData[ 1 ] = bTWRIOExpander_port0;
   I2CDRV_Write( &I2C2Handler,  HW_IO_EXPAND_I2C_ADDR, abI2CData, 2 );

   abI2CData[ 0 ] = HW_IO_EXPAND_CMD_OUT_PORT1;

   /*
   ** OM1
   */
   if( bOpmode & 0x2 )
   {
      bTWRIOExpander_port1 |= HW_GPIO_OM1;
   }

   /*
   ** OM3
   */
   if( bOpmode & 0x08 )
   {
      bTWRIOExpander_port1 |= HW_GPIO_OM3;
   }

   abI2CData[ 1 ] = bTWRIOExpander_port1;
   I2CDRV_Write( &I2C2Handler,  HW_IO_EXPAND_I2C_ADDR, abI2CData, 2 );

   if ( bOpmode >= ABP_OP_MODE_SERIAL_19_2 )
   {
      InitSerialOpmode( bOpmode );
   }
}
#endif

#if( ABCC_CFG_DRV_SPI )
void HW_InitSpi( void )
{
   /*
   ** We set the Tx- and Rx thresholds.
   */
   HW_SPI_REG_SPMODE = 0;

   HW_SPI_REG_SPMODE |= ( 0x10 << 8 );
   HW_SPI_REG_SPMODE |= 0x0F;

   /*
   ** Enable the SPI controller.
   */
   HW_SPI_REG_SPMODE |= HW_SPI_SPMODE_BIT_EN;

   /*
   ** Set up the SPI operation for SPI slave on chip select 0.
   */
   HW_SPI_REG_SPMODE0 = 0;

   /*
   ** Make sure we send the most significant bit first, reverse bit order.
   */
   HW_SPI_REG_SPMODE0 |= HW_SPI_SPMODE0_BIT_REV;

   /*
   ** Clock source is system clock/2 = 133MHz.
   ** Setting PM0 to 3 will give a SPI clock of 133/( 2x(1+PM0)) ~16.6MHz.
   */
   HW_SPI_REG_SPMODE0 |= HW_SPI_SPMODE0_BIT_PM0;

   /*
   ** Set CS0 Asserted Low, Negated High.
   */
   HW_SPI_REG_SPMODE0 |= HW_SPI_SPMODE0_BIT_POL;

   /*
   ** Character length is 8 bits.
   */
   HW_SPI_REG_SPMODE0 |= ( 7 << 16 );

   /*
   ** Insert 1 bit time after CS assertion before clock starts toggling.
   */
   HW_SPI_REG_SPMODE0 |= ( 1 << 12 );

   /*
   ** Stop toggling clock 1 bit time before CS negation.
   */
   HW_SPI_REG_SPMODE0 |= ( 1 << 8 );
}

/*------------------------------------------------------------------------------
** Makes an SPI transfer in task less mode.
** This means no interrupts or semaphore etc is used.
**------------------------------------------------------------------------------
** Arguments:
**    pxSendDataBuffer    - Pointer to data to transmit.
**                  NULL if no data to transmit, zeros will be transfered.
**    pxRxData    - Pointer to data to receive.
**                  NULL if received data shall be discarded.
**    iSize       - Number of bytes to transfer.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/

void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength )
{
   const UINT8* pbTxData;
   UINT8*       pbRxData;

   pbTxData = pxSendDataBuffer;
   pbRxData = pxReceiveDataBuffer;

   /*
   ** Set up the transaction length for this transaction.
   */
   HW_SPI_REG_SPCOM = (UINT32)( iLength - 1 );

   while( iLength )
   {
      /*
      ** Wait until there is room in the Tx FIFO.
      */
      while( !( HW_SPI_REG_SPIE & HW_SPI_SPIE_BIT_TNF ) ) { }

      HW_SPI_REG_SPITF_B = *pbTxData;
      pbTxData++;

      /*
      ** Wait until the RxFIFO isn't empty.
      */
      while( ( !( HW_SPI_REG_SPIE & HW_SPI_SPIE_BIT_RNE ) ) ) { }

      *pbRxData = HW_SPI_REG_SPIRF_B;
      pbRxData++;

      iLength--;
   }
   /*
   ** Make sure the last transaction has finished.
   */
   while( !( HW_SPI_REG_SPIE & HW_SPI_SPIE_BIT_DON ) ) { }

   pnDataReadyCbf();
}
#endif

BOOL ABCC_SYS_Init( void )
{
   return TRUE;
}

void ABCC_SYS_Close( void )
{
}

void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   pnDataReadyCbf = pnDataReceived;
}

BOOL ABCC_SYS_HwInit( void )
{
   BOOL fRes;
   /*
   ** Set TWR p1025 IO expander IO shadow registers to 0
   */
   bTWRIOExpander_port0 = 0;
   bTWRIOExpander_port1 = 0;

   HW_ConfigIoExpander();
   HW_InitSpi();
   ABCC_SYS_HWReset();
   fRes = TWR_ABCC_IO_Init();
   return TRUE;
}

#if( ABCC_CFG_DRV_SERIAL )

void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SerDataReceivedCbfType pnDataReceived  )
{
   pnSerDataReceived = pnDataReceived;
}

void ABCC_SYS_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{

   WriteUARTN( pxTxDataBuffer,  iTxSize );
   ReadUARTN(pxRxDataBuffer, iRxSize );
   pnSerDataReceived();
}

void ABCC_SYS_SerRestart( void )
{
   InitSerialOpmode( appl_bOpmode );
}

#endif


#if ( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 )
void ABCC_SYS_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   (void)iMemOffset;
   (void*)pxData;
   (void)iLength;
}

UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
   UINT8 bData = 0;
   ABCC_SYS_ParallelRead( iMemOffset, &bData, sizeof( UINT8 ) );
   return bData;
}

#if( ABCC_CFG_DRV_PARALLEL )
UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
   UINT16 iData = 0;
   ABCC_SYS_ParallelRead( iMemOffset, (UINT8*)&iData, sizeof( UINT16 ) );
   return iData;
}
#endif

void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   (void)iMemOffset;
   (void*)pxData;
   (void)iLength;
}

void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 bpData )
{
   ABCC_SYS_ParallelWrite( iMemOffset, (UINT8*)&bpData, sizeof( UINT8 ) );
}

#if( ABCC_CFG_DRV_PARALLEL )
void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   ABCC_SYS_ParallelWrite( iMemOffset, (UINT8*)&piData, sizeof( UINT16 ) );
}
#endif

#endif

#ifndef ABCC_CFG_ABCC_MODULE_ID
UINT8 ABCC_SYS_ReadModuleId( void )
{
   UINT8  bModuleId;

   if( !TWR_ABCC_IO_GetAbccModuleId( &bModuleId ) )
   {
      APPL_UnexpectedError();
      bModuleId = 0xff;
   }

   return( bModuleId );
}
#endif

#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
BOOL ABCC_SYS_ModuleDetect( void )
{
   UINT8 bModuleDetect;

   if( !TWR_ABCC_IO_GetAbccModuleDetect( &bModuleDetect ) )
   {
      APPL_UnexpectedError();
      return( FALSE );
   }

   return( bModuleDetect == 0 );
}
#endif

#if( ABCC_CFG_OP_MODE_GETTABLE )
UINT8 ABCC_SYS_GetOpmode( void )
{
   UINT8 bOpmode;

   if( !TWR_ABCC_IO_GetAbccOpMode( &bOpmode ) )
   {
      APPL_UnexpectedError();
      bOpmode = 0xff;
   }

   return( bOpmode );
}
#endif

/*******************************************************************************
** End of abcc_sys_adapt.c
********************************************************************************
*/


