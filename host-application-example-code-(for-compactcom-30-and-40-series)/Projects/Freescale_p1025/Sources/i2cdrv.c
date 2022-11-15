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
** Implementation of I2C driver for p1025
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "i2cdrv.h"
#include "p1025.h"

/*
** Help macros for I2C driver
*/
#define HW_I2C_ADR        (*psI2cHandler->pbAdr)
#define HW_I2C_FDR        (*psI2cHandler->pbFdr)
#define HW_I2C_CR         (*psI2cHandler->pbCr)
#define HW_I2C_SR         (*psI2cHandler->pbSr)
#define HW_I2C_DR         (*psI2cHandler->pbDr)

BOOL I2CDRV_Init( I2CDRV_HandlerType* psI2cHandler, UINT8 bId )
{
   UINT8 bRegValue;
   volatile UINT8* pbI2cBaseAdr;

   if( bId == 1 )
   {
      pbI2cBaseAdr = (volatile UINT8*)HW_I2C1_BASE_ADDR;
   }
   else if( bId == 2 )
   {
      pbI2cBaseAdr = (volatile UINT8*)HW_I2C2_BASE_ADDR;
   }
   else
   {
      /*
      ** Id is not supported
      */
      return FALSE;
   }

   /*
   ** Set the I2C frequency divider ration to 3072 which gives SCL ~= 87 KHz.
   */
   psI2cHandler->pbAdr = (volatile UINT8*)pbI2cBaseAdr;             /* I2C interface address register */
   psI2cHandler->pbFdr = (volatile UINT8*)( pbI2cBaseAdr + 0x04 );  /* I2C interface frequency divider register */
   psI2cHandler->pbCr  = (volatile UINT8*)( pbI2cBaseAdr + 0x08 );  /* I2C interface control register */
   psI2cHandler->pbSr  = (volatile UINT8*)( pbI2cBaseAdr + 0x0C );  /* I2C interface status register */
   psI2cHandler->pbDr  = (volatile UINT8*)( pbI2cBaseAdr + 0x10 );  /* I2C interface data register */

   /*
   ** Set the I2C frequency divider ration to 3072 which gives SCL ~= 87 KHz.
   */
   bRegValue = HW_I2C_FDR;
   bRegValue &= 0xC0;
   bRegValue |= 0x0E;
   HW_I2C_FDR = bRegValue;
   bRegValue = HW_I2C_CR;

   /*
   ** Make sure we are sending ACK on received data.
   */
   bRegValue &= ~HW_I2C_CR_BIT_TXAK;

   /*
   ** Enable the I2C interface.
   */
   bRegValue |= HW_I2C_CR_BIT_MEN;

   HW_I2C_CR = bRegValue;


   return( TRUE );
}

BOOL I2CDRV_Read( I2CDRV_HandlerType* psI2cHandler, UINT8 bAddress, UINT8 *pbData, UINT32 lLength )
{
   UINT8 bRegValue;
   UINT8 bDummy;

   /*
   ** Wait until the bus is available.
   */
   while( HW_I2C_SR & HW_I2C_SR_BIT_MBB ) { }

   /*
   ** First we must write the command to the GPIO Expander.
   ** Set the I2C controller to transmit mode and clear the TXAK bit to make sure we acknowledge data.
   */
   bRegValue = HW_I2C_CR;
   bRegValue |= HW_I2C_CR_BIT_MTX;
   bRegValue &= ~HW_I2C_CR_BIT_TXAK;

   /*
   ** Generate the START condition.
   */
   bRegValue |= HW_I2C_CR_BIT_MSTA;
   HW_I2C_CR = bRegValue;

   /*
   ** Write the slave address to the 7 most significant bits of the data register and
   ** set the least significant bit to indicate we want to receive data.
   */
   HW_I2C_DR = ( ( bAddress << 1 ) | 0x01 );

   /*
   ** Wait for transmission to be completed and clear the interrupt flag.
   */
   while( !( HW_I2C_SR & HW_I2C_SR_BIT_MIF ) ) { }

   bRegValue = HW_I2C_SR;
   bRegValue &= ~HW_I2C_SR_BIT_MIF;
   HW_I2C_SR = bRegValue;

   bRegValue = HW_I2C_CR;
   bRegValue &= ~HW_I2C_CR_BIT_MTX;
   HW_I2C_CR = bRegValue;

   HW_DELAY_US( 10 );

   /*
   ** Perform the dummy read.
   */
   bDummy = HW_I2C_DR;

   /*
   ** Wait for the slave to complete the transmission and clear the interrupt flag.
   */
   while( !( HW_I2C_SR & HW_I2C_SR_BIT_MIF ) ) { }

   bRegValue = HW_I2C_SR;
   bRegValue &= ~HW_I2C_SR_BIT_MIF;
   HW_I2C_SR = bRegValue;

   while( lLength > 0 )
   {
      if( ( lLength - 1 ) == 0 )
      {
         /*
         ** Next to last byte, we must set the TXAK flag to make us stop acknowledging data.
         */
         bRegValue = HW_I2C_CR;
         bRegValue |= HW_I2C_CR_BIT_TXAK;
         HW_I2C_CR = bRegValue;
      }

      *pbData = HW_I2C_DR;
      pbData++;
      lLength--;

      /*
      ** Wait for the slave to complete the transmission and clear the interrupt flag.
      */
      while( !( HW_I2C_SR & HW_I2C_SR_BIT_MIF ) ) { }

      bRegValue = HW_I2C_SR;
      bRegValue &= ~HW_I2C_SR_BIT_MIF;
      HW_I2C_SR = bRegValue;
   }

   /*
   ** Generate STOP.
   */
   bRegValue = HW_I2C_CR;
   bRegValue &= ~HW_I2C_CR_BIT_MSTA;
   HW_I2C_CR = bRegValue;
}

BOOL I2CDRV_Write( I2CDRV_HandlerType* psI2cHandler, UINT8 bAddress, const UINT8 *pbData, UINT32 lLength )
{
   BOOL fStatus;
   UINT8 bRegValue;
   fStatus = TRUE;
    /*
    ** Wait until the bus is available.
    */
    while( HW_I2C_SR & HW_I2C_SR_BIT_MBB ) { }

    bRegValue = HW_I2C_CR;

    /*
    ** Set the I2C controller to transmit mode.
    */
    bRegValue |= HW_I2C_CR_BIT_MTX;

    /*
    ** Generate the START condition.
    */
    bRegValue |= HW_I2C_CR_BIT_MSTA;
    HW_I2C_CR = bRegValue;

    /*
    ** Write the slave address to the 7 most significant bits of the data register.
    */
    HW_I2C_DR = ( bAddress << 1 );

    /*
    ** Wait for transmission to be completed and clear the interrupt flag.
    */
    while( !( HW_I2C_SR & HW_I2C_SR_BIT_MIF ) ) { }

    bRegValue = HW_I2C_SR;
    bRegValue &= ~HW_I2C_SR_BIT_MIF;
    HW_I2C_SR = bRegValue;

    while( lLength )
    {
       if( HW_I2C_SR & HW_I2C_SR_BIT_RXAK )
       {
          /*
          ** No acknowledge received from the slave, we need to generate STOP.
          */
          fStatus = FALSE;
          break;
       }

       HW_I2C_DR = *pbData;
       pbData++;
       lLength--;

       /*
       ** Wait for transmission to be completed and clear the interrupt flag.
       */
       while( !( HW_I2C_SR & HW_I2C_SR_BIT_MIF ) ) { }

       bRegValue = HW_I2C_SR;
       bRegValue &= ~HW_I2C_SR_BIT_MIF;
       HW_I2C_SR = bRegValue;
    }

    /*
    ** Generate STOP.
    */
    bRegValue = HW_I2C_CR;
    bRegValue &= ~HW_I2C_CR_BIT_MSTA;
    HW_I2C_CR = bRegValue;

    return( fStatus );
}
