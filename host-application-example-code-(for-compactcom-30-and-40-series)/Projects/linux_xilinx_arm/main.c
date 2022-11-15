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
** This file contains the linux-xilinx-arm project's main function.
** From here the different system parts are initialized directly or indirectly.
********************************************************************************
********************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include "abcc.h"
#include "abcc_versions.h"
#include "abcc_obj_cfg.h"
#include "abp_etn.h"
#include "etn_obj.h"
#include "safe_obj.h"

#include "appl_abcc_handler.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Ethernet i/f in Linux that is connected to AnybusIP MII interface
** If no Ethernet i/f is used set ABIP_LINUX_USE_MII_ETH_IF = 0
**------------------------------------------------------------------------------
*/
#define ABIP_LINUX_USE_MII_ETH_IF   1
#define ABIP_LINUX_MII_ETH_IF       "eth1"

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

unsigned char* pAbcc;
void* pMemSys;

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

int main( int argc, char **argv )
{
   /*
   ** Make sure the ABCC reset signal is kept low by the platform specific
   ** initialization to keep the ABCC module in reset until the driver releases
   ** it.
   */
   APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_MODULE_NO_ERROR;
   const UINT16 iSleepTimeMS = 1;
   int fd;
   int fdSys;
   void* pMem;
   unsigned page_addr;
   unsigned page_size=16*1024;
   BOOL8    fQuit = FALSE;
   char* pcArgsInfo = "";

   /* Determine if Safety should be enabled or not */
   if( ( argc == 2 ) &&
       ( strcmp( argv[ 1 ], "-safety" ) == 0 ) )
   {
      pcArgsInfo = "(T100 Safety enabled)";
      SAFE_SetSafetyEnable( TRUE );
   }

   printf("%s %s\n", ABCC_STARTER_KIT_VER_STRING, pcArgsInfo );

   //Setup pointer to AXI bus that provide memory access to ABCC
   fd = open( "/dev/mem", O_RDWR );
   if( fd < 1 )
   {
      printf( "Failed to open /dev/mem\n" );
      return -1;
   }

   page_addr = (0x43C00000 & ~(page_size-1));
   pMem = mmap( NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr );

   pAbcc = (unsigned char*)pMem;

   //Get pointer to system controller
   fdSys = open( "/dev/mem", O_RDWR );
   if( fdSys < 1 )
   {
      printf( "Failed to open /dev/mem for system controler\n" );
      return -1;
   }

   page_size = 0x400;
   page_addr = (0xF8000000 & ~(page_size-1));
   pMemSys = mmap( NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fdSys, page_addr );

   if( ABCC_HwInit() != ABCC_EC_NO_ERROR )
   {
      return -1;
   }

   while( !fQuit  )
   {
      eAbccHandlerStatus = APPL_HandleAbcc();

      switch( eAbccHandlerStatus )
      {
      case APPL_MODULE_NO_ERROR:
         break;
      case APPL_MODULE_RESET:
         APPL_RestartAbcc();
         break;
      case APPL_MODULE_NOT_DETECTED:
      case APPL_MODULE_NOT_SUPPORTED:
      case APPL_MODULE_NOT_ANSWERING:
      case APPL_MODULE_SHUTDOWN:
      case APPL_MODULE_UNEXPECTED_ERROR:
      default:
         printf( "\nAPPL_HandleAbcc() returned status code: %d\n", eAbccHandlerStatus );
         fQuit = TRUE;
         break;
      }

      usleep( iSleepTimeMS*1000 );
      ABCC_RunTimerSystem( iSleepTimeMS );
   }

   return( 0 );
}

/*******************************************************************************
** Callbacks
********************************************************************************
*/
void ETN_SetAttrSuccessCallback( UINT16 iInstance, UINT8 bAttribute )
{
#if ABIP_LINUX_USE_MII_ETH_IF
   static BOOL8 fHasInitialConfig = FALSE;

   if( ( iInstance == 1 ) &&
       ( bAttribute == ABP_ETN_IA_IP_CONFIGURATION ) )
   {
      ETN_IpConfigType sIpConfig;

      ETN_GetIpConfig( &sIpConfig );

      if( ( sIpConfig.sAddr.lIpAddr != 0 ) &&
          ( sIpConfig.sAddr.lSnMask != 0 ) )
      {
         char strSysCmd[ 128 ];
         UINT8 abAbccHwAddr[ 6 ];

         /* New IP configuration; reconfigure Linux system with this setting */
         memcpy( abAbccHwAddr, ETN_IA_MAC_ADDRESS_VALUE, sizeof( abAbccHwAddr ) );

         printf( "ABCC40 New IP Configuration:\n"
                 "IpAddr: %u.%u.%u.%u\n"
                 "SnMask: %u.%u.%u.%u\n"
                 "GwAddr: %u.%u.%u.%u\n\n",
                 ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 0 ], ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 1 ], ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 2 ], ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 3 ],
                 ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 0 ], ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 1 ], ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 2 ], ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 3 ],
                 ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 0 ], ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 1 ], ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 2 ], ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 3 ] );

         if( !fHasInitialConfig )
         {
            /* Take down Ethernet i/f */
            sprintf( strSysCmd, "ifconfig %s down", ABIP_LINUX_MII_ETH_IF );
            system( strSysCmd );

            /*
            ** The HW address of the Linux MII Ethernet i/f needs to be the same as
            ** AnybusIP Ethernet i/f hardware address.
            */
            sprintf( strSysCmd, "ifconfig %s hw ether %02x:%02x:%02x:%02x:%02x:%02x",
                                ABIP_LINUX_MII_ETH_IF,
                                abAbccHwAddr[ 0 ], abAbccHwAddr[ 1 ], abAbccHwAddr[ 2 ], abAbccHwAddr[ 3 ], abAbccHwAddr[ 4 ], abAbccHwAddr[ 5 ] );
            system( strSysCmd );
         }

         /* Set new IP address and Subnet mask */
         sprintf( strSysCmd, "ifconfig %s %u.%u.%u.%u netmask %u.%u.%u.%u",
                             ABIP_LINUX_MII_ETH_IF,
                             ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 0 ], ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 1 ], ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 2 ], ((UINT8*)&sIpConfig.sAddr.lIpAddr)[ 3 ],
                             ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 0 ], ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 1 ], ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 2 ], ((UINT8*)&sIpConfig.sAddr.lSnMask)[ 3 ] );
         system( strSysCmd );

         if( !fHasInitialConfig )
         {
            /* Take up Ethernet i/f */
            sprintf( strSysCmd, "ifconfig %s up", ABIP_LINUX_MII_ETH_IF );
            system( strSysCmd );
         }

         /* Reconfigure default route if gateway address is provided */
         if( sIpConfig.sAddr.lGwAddr != 0 )
         {
            sprintf( strSysCmd, "route add default gw %u.%u.%u.%u %s",
                                ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 0 ], ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 1 ], ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 2 ], ((UINT8*)&sIpConfig.sAddr.lGwAddr)[ 3 ],
                                ABIP_LINUX_MII_ETH_IF );
            system( strSysCmd );
         }

         fHasInitialConfig = TRUE;
      }
   }
#endif /* #if ABIP_LINUX_USE_MII_ETH_IF */
}
