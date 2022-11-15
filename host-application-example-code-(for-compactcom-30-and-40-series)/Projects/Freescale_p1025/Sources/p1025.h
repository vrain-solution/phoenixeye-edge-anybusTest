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
** permission. When used together with a product from HMS, this code can be   **
** modified, reproduced and distributed in binary form without any            **
** restrictions.                                                              **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
** P1025 register definitions.
********************************************************************************
********************************************************************************
** Services:
**    HW_DELAY_US()     - Delay execution.
********************************************************************************
********************************************************************************
*/

#ifndef P1025_H_
#define P1025_H_

#include "abcc_td.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*
** CCSR base address
*/
#define HW_CCSR_BASE_ADDR  ( 0xE0000000 )

/*
** Device specific register address
*/
#define HW_DSR_BASE_ADDR   ( HW_CCSR_BASE_ADDR + 0xE0000 )

/*
** Reset control register address GUTS_RSTCR
*/
#define HW_GUTS_RSTCR   *(volatile UINT32*)( HW_DSR_BASE_ADDR + 0xB0 )

/*
** Reset control register defines
*/
#define HW_RSTCR_HRESET_REQ ( 0x00000002 )

/*
** SPI functionality.
*/
#define HW_SPI_BASE_ADDR   ( HW_CCSR_BASE_ADDR + 0x7000 )

#define HW_SPI_REG_SPMODE  ( *(volatile UINT32*)HW_SPI_BASE_ADDR )            /* SPI mode register */
#define HW_SPI_REG_SPIE    ( *(volatile UINT32*)( HW_SPI_BASE_ADDR + 0x04 ) ) /* SPI event register */
#define HW_SPI_REG_SPIM    ( *(volatile UINT32*)( HW_SPI_BASE_ADDR + 0x08 ) ) /* SPI mask register */
#define HW_SPI_REG_SPCOM   ( *(volatile UINT32*)( HW_SPI_BASE_ADDR + 0x0C ) ) /* SPI command register */
#define HW_SPI_REG_SPITF   ( *(volatile UINT32*)( HW_SPI_BASE_ADDR + 0x10 ) ) /* SPI transmit FIFO access register */
#define HW_SPI_REG_SPITF_B ( *(volatile UINT8*)( HW_SPI_BASE_ADDR + 0x10 ) ) /* SPI transmit FIFO access register */
#define HW_SPI_REG_SPIRF   ( *(volatile UINT32*)( HW_SPI_BASE_ADDR + 0x14 ) ) /* SPI receive FIFO access register */
#define HW_SPI_REG_SPIRF_B ( *(volatile UINT8*)( HW_SPI_BASE_ADDR + 0x14 ) ) /* SPI receive FIFO access register */
#define HW_SPI_REG_SPMODE0 ( *(volatile UINT32*)( HW_SPI_BASE_ADDR + 0x20 ) ) /* SPI CS0 mode register */

#define HW_SPI_SPMODE_BIT_EN  0x80000000  /* Enable SPI */

#define HW_SPI_SPIE_BIT_DON         0x00004000  /* Last character transmitted, done with frame. */
#define HW_SPI_SPIE_BIT_RNE         0x00000200  /* Rx FIFO not empty */
#define HW_SPI_SPIE_BIT_TNF         0x00000100  /* Tx FIFO not full */

#define HW_SPI_SPMODE0_BIT_REV      0x20000000  /* Reverse data mode. Send MSB first when set. */
#define HW_SPI_SPMODE0_BIT_PM0      0x03000000  /* Clock divider 2x(1+PM0)= 2x(1+3) = 8 */
#define HW_SPI_SPMODE0_BIT_POL      0x00100000  /* CS0 Polarity. */

/*
** I2C base addresses
*/
#define HW_I2C1_BASE_ADDR  ( HW_CCSR_BASE_ADDR + 0x3000 )
#define HW_I2C2_BASE_ADDR  ( HW_CCSR_BASE_ADDR + 0x3100 )

#define HW_I2C_CR_BIT_MEN     0x80     /* Module enable */
#define HW_I2C_CR_BIT_MIEN    0x40     /* Module interrupt enable */
#define HW_I2C_CR_BIT_MSTA    0x20     /* Master/slave mode START */
#define HW_I2C_CR_BIT_MTX     0x10     /* Transmit/receive mode select */
#define HW_I2C_CR_BIT_TXAK    0x08     /* Transfer acknowledge */
#define HW_I2C_CR_BIT_RSTA    0x04     /* Repeated START */
#define HW_I2C_CR_BIT_BCST    0x01     /* Broadcast */

#define HW_I2C_SR_BIT_MBB     0x20     /* Bus busy */
#define HW_I2C_SR_BIT_MIF     0x02     /* Module interrupt */
#define HW_I2C_SR_BIT_RXAK    0x01     /* Received acknowledge */

/*
** I2C address of the GPIO expander located on P1025 board.
*/
#define HW_IO_EXPAND_I2C_ADDR          0x23

/*
** Commands supported by the GPIO expander located on P1025 board.
*/
#define HW_IO_EXPAND_CMD_IN_PORT0      0x00
#define HW_IO_EXPAND_CMD_IN_PORT1      0x01
#define HW_IO_EXPAND_CMD_OUT_PORT0     0x02
#define HW_IO_EXPAND_CMD_OUT_PORT1     0x03
#define HW_IO_EXPAND_CMD_POLINV_PORT0  0x04
#define HW_IO_EXPAND_CMD_POLINV_PORT1  0x05
#define HW_IO_EXPAND_CMD_CONFIG_PORT0  0x06
#define HW_IO_EXPAND_CMD_CONFIG_PORT1  0x07

/*
** Bit masks for I2C IO expander located on P1025 board
*/
#define HW_GPIO_EXPAND0    0x01
#define HW_GPIO_EXPAND1    0x02
#define HW_GPIO_EXPAND2    0x04
#define HW_GPIO_EXPAND3    0x08
#define HW_GPIO_EXPAND4    0x10
#define HW_GPIO_EXPAND5    0x20
#define HW_GPIO_EXPAND6    0x40
#define HW_GPIO_EXPAND7    0x80

#define HW_GPIO_EXPAND8    0x01
#define HW_GPIO_EXPAND9    0x02
#define HW_GPIO_EXPAND10   0x04
#define HW_GPIO_EXPAND11   0x08
#define HW_GPIO_EXPAND12   0x10
#define HW_GPIO_EXPAND13   0x20
#define HW_GPIO_EXPAND14   0x40
#define HW_GPIO_EXPAND15   0x80

/*
** PIC global timer register defines
*/
#define PIC_BASE_ADDR  ( HW_CCSR_BASE_ADDR + 0x40000 )
#define PIC_GTCCRA( n ) *(volatile UINT32*)(PIC_BASE_ADDR + 0x1100 + 64 * (n) )
#define PIC_GTBCRA( n ) *(volatile UINT32*)(PIC_BASE_ADDR + 0x1110 + 64 * (n) )
#define PIC_GTVPRA( n ) *(volatile UINT32*)(PIC_BASE_ADDR + 0x1120 + 64 * (n) )
#define PIC_CTPR        *(volatile UINT32*)(PIC_BASE_ADDR + 0x80 )
#define PIC_CTPR_CPU0   *(volatile UINT32*)(PIC_BASE_ADDR + 0x20080 )

#define PIC_GTDRA( n ) *(volatile UINT32*)(PIC_BASE_ADDR + 0x1130 + 64 * (n) )
#define PIC_TCR( n )   *(volatile UINT32*)(PIC_BASE_ADDR + 0x1300 + 4096 * (n) )

#define PIC_TCR_ROVR_TIMER_0   0x01000000
#define PIC_TCR_CASC_TIMER_0_1 0x00000001


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
** Delay execution for lDelayUs microseconds.
** Timer GTCCRA( 0 ) clock source is 33,3 MHz.
** Timer counts = 33,3 * uSec.
**-----------------------------------------------------------------------------
*/
#define HW_DELAY_US( lDelayUs )                             \
do                                                          \
{                                                           \
   UINT32 lTimeOut;                                         \
   UINT32 ltime;                                            \
   lTimeOut = ( lDelayUs ) * 333;                           \
   lTimeOut /= 10;                                          \
                                                            \
   ltime = PIC_GTCCRA( 0 );                                 \
   while( ( ltime - PIC_GTCCRA( 0 ) ) < lTimeOut ) ;        \
}                                                           \
while( 0 )

#endif /* P1025_H_ */
