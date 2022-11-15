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
** Example of an ADI setup with 32 ADIs each one holding a 16 bit value.
**
** In abcc_drv_cfg.h make sure that the following definitions are set to:
** ABCC_CFG_STRUCT_DATA_TYPE     ( FALSE )
** ABCC_CFG_ADI_GET_SET_CALLBACK ( TRUE )
********************************************************************************
********************************************************************************
*/

#include "appl_adi_config.h"
#include "twr_abcc_io.h"
#include "appl_abcc_handler.h"


#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_BOARD_SPECIFIC )

#if( ABCC_CFG_STRUCT_DATA_TYPE || !ABCC_CFG_ADI_GET_SET_CALLBACK  )
   #error ABCC_CFG_ADI_GET_SET_CALLBACK must be set to TRUE and ABCC_CFG_STRUCT_DATA_TYPE set to FALSE in order to run this example
#endif

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Access descriptor for the ADIs
**------------------------------------------------------------------------------
*/
#define APPL_READ_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_READ_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_SET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_WRITE_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_WRITE_PD )

#define APPL_NOT_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS )

#define APPL_NOT_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_SET_ACCESS )

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/
/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/

static void SetGeneralIO_Out( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex );
static void GetGeneralIO_In( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex );

/*------------------------------------------------------------------------------
** Data holder for the ADI instances
**------------------------------------------------------------------------------
*/
static UINT8  appl_bGeneralIO_In = 0;
static UINT8  appl_bGeneralIO_Out = 0;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Map piano dip switch as write process data and general IO as output.
** See abcc_ad_if.h for a more detailed description.
**------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------------------------------------------------
** iInstance | pabName | bDataType | bNumOfElements | bDesc | pxValuePtr | pxValuePropPtr| pnGetAdiValue | pnSetAdiValue
**-----------------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType APPL_asAdiEntryList[] =
{
   {  100,   "Piano dip switch", ABP_UINT8,   1, APPL_WRITE_MAP_READ_ACCESS_DESC,  { { &appl_bGeneralIO_In ,NULL } }, GetGeneralIO_In, NULL  },
   {  101,   "IO-Output",        ABP_UINT8,   1, APPL_READ_MAP_WRITE_ACCESS_DESC,  { { &appl_bGeneralIO_Out ,NULL } }, NULL, SetGeneralIO_Out },
};

/*
**------------------------------------------------------------------------------
** bGeneralIO_Out is mapped in PD_READ direction.
** bGeneralIO_In is mapped in PD_WRITE direction.
** See abcc_ad_if.h for a more detailed description.
**------------------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**------------------------------------------------------------------------------
*/
const AD_DefaultMapType APPL_asAdObjDefaultMap[] =
{
   { 100, PD_WRITE, AD_DEFAULT_MAP_ALL_ELEM, 0 },
   { 101, PD_READ,  AD_DEFAULT_MAP_ALL_ELEM, 0 },
   { AD_DEFAULT_MAP_END_ENTRY }
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
**  Implementation of ABCC_SetAdiValueFuncType ( described in abcc_ad_if.h ).
**  Each time ADI 100 ( appl_bGeneralIO_Out ) is updated either by read process data
**  or by explicit messaging this callback will be invoked by the application data
**  object. The 6 least significant bits are written to the general outputs on the
**  adapter board. The 4 least significant bits are also written to the adapter board
**  general purpose leds.
**------------------------------------------------------------------------------
*/
static void SetGeneralIO_Out( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex )
{
   (void)psAdiEntry;
   (void)bNumElements;
   (void)bStartIndex;

   if( !TWR_ABCC_IO_SetGeneralOutputs( appl_bGeneralIO_Out ) )
   {
      APPL_UnexpectedError();
      return;
   }

   if( !TWR_ABCC_IO_SetLedOutputs( appl_bGeneralIO_Out ) )
   {
      APPL_UnexpectedError();
      return;
   }
}

/*------------------------------------------------------------------------------
**  Implementation of ABCC_GetAdiValueFuncType ( described in abcc_ad_if.h ).
**  Each time ADI 101 ( appl_bGeneralIO_In ) is about to be read by the application
**  data object, either by explicit messaging  or when the write process data is
**  updated, this callback is invoked. The value of the piano switch is read and
**  placed in the 4 least significant bits in the ADI.
**------------------------------------------------------------------------------
*/
static void GetGeneralIO_In( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex )
{
   (void)psAdiEntry;
   (void)bNumElements;
   (void)bStartIndex;

   if( !TWR_ABCC_IO_GetPianoDipSwitch( &appl_bGeneralIO_In ) )
   {
      APPL_UnexpectedError();
   }
}


/*******************************************************************************
** Public Services
********************************************************************************
*/

void APPL_CyclicalProcessing( void )
{
   /*
   ** This function is called when read and write data have been updated. It
   ** could for example be used for operations on the ADI data.
   ** Not used in this example.
   */
}

UINT16 APPL_GetNumAdi( void )
{
   return( sizeof( APPL_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}
/*******************************************************************************
** Tasks
********************************************************************************
*/
#endif

