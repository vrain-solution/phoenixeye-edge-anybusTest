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
** COPYRIGHT NOTIFICATION (c) 2012 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** I2C driver
********************************************************************************
********************************************************************************
** Services:
**    I2CDRV_Init()  - Initialize the I2C controller
**    I2CDRV_Read()  - Read data from an I2C node
**    I2CDRV_Write() - Write data to an I2C node
********************************************************************************
********************************************************************************
*/

#ifndef I2CDRV_H_
#define I2CDRV_H_
#include "abcc_td.h"
/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/


/*------------------------------------------------------------------------------
** I2C handler conatinig addresses to the chosen I2C controller.
**------------------------------------------------------------------------------
*/
typedef struct I2CHandler
{
   volatile UINT8* pbAdr;
   volatile UINT8* pbFdr;
   volatile UINT8* pbCr;
   volatile UINT8* pbSr;
   volatile UINT8* pbDr;
}
I2CDRV_HandlerType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Initializes the I2C controller
**------------------------------------------------------------------------------
** Arguments:
**    psI2cHandler - Pointer to I2C handler.
**
** Returns:
**    TRUE  - Init successful
**    FALSE - Id not supported
**------------------------------------------------------------------------------
*/
extern BOOL I2CDRV_Init( I2CDRV_HandlerType* psI2cHandler, UINT8 bId );

/*------------------------------------------------------------------------------
** Read data from an I2C node
**------------------------------------------------------------------------------
** Arguments:
**    psI2cHandler - Pointer to I2C handler.
**    bAddress - I2C node address
**    pbData   - Pointer to data buffer which the response will be written to
**    lLength  - Number of bytes to read
**
** Returns:
**    TRUE if the read was successful, else FALSE.
**------------------------------------------------------------------------------
*/
extern BOOL I2CDRV_Read( I2CDRV_HandlerType* psI2cHandler, UINT8 bAddress, UINT8 *pbData, UINT32 lLength );

/*------------------------------------------------------------------------------
** Write data to an I2C node
**------------------------------------------------------------------------------
** Arguments:
**    psI2cHandler - Pointer to I2C handler.
**    bAddress - I2C node address
**    pbData   - Pointer to data buffer to write
**    lLength  - Number of bytes to read
**
** Returns:
**    TRUE if the write was successful, else FALSE.
**------------------------------------------------------------------------------
*/
extern BOOL I2CDRV_Write( I2CDRV_HandlerType* psI2cHandler, UINT8 bAddress, const UINT8 *pbData, UINT32 lLength );

#endif /* I2CDRV_H_ */
