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
** Main program
********************************************************************************
********************************************************************************
*/
#include <stdio.h>
#include <termios.h>
#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "appl_abcc_handler.h"

#define APPL_TIMER_MS         1
#define USE_TIMER_INTERRUPT   1

#if( USE_TIMER_INTERRUPT )
static void TimerIsr( void )
{
   ABCC_RunTimerSystem( APPL_TIMER_MS );
}

static void SetupTimerInterrupt( void )
{
   // 暫定 一旦ポーリング毎に5[ms]時間が進んだとして動作させる。
   ABCC_RunTimerSystem( 5 );
}
#else
static void DelayMs( UINT32 lDelayMs )
{
}
#endif

static void Reset(void)
{
}

//
// Control functions
//
int AnybusModuleControl( char c )
{
   switch(c){
      case 'i':
         if( ABCC_HwInit() != ABCC_EC_NO_ERROR ){
            printf("ABCC_HwInit ERROR\n");
         } else {
            printf("Hardware initialized.\n");
         }
         break;

      case 'q':
      // Push quit.
      printf("Quit.\n");
      ABCC_SYS_Close();
      break;

      case 'r':
      // Reset
      printf("Anybus Reset.\n");
      ABCC_SYS_HWReset();
      break;

      case 's':
      // Set to run mode.
      printf("Anybus Run.\n");
      ABCC_SYS_HWReleaseReset();
      break;

      case 'a':
      // Set address.
      printf("Set address.\n");
      ABCC_isReadyForCommunication();
      break;
   }
   return 0;
}

int main()
{
   /*
   ** Make sure the ABCC reset signal is kept low by the platform specific
   ** initialization to keep the ABCC module in reset until the driver releases
   ** it.
   */
   // For child process.
   int fds[2];
   pid_t pid;
   int   status;
   // Build pipe.
   // if(pipe2(fds, O_NONBLOCK) < 0 ){
   if(pipe(fds) < 0 ){
      perror("Error in pipe()");
      return -1;
   }
   // Set FD to non-blocking.
   fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL, 0) | O_NONBLOCK);
   fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL, 0) | O_NONBLOCK);

   // Process fork
   if((pid = fork()) < 0 ){
      perror("Error in fork()");
      return -1;
   }

   if( pid == 0 ){
      // I am child process.
      printf("Child process started.\n");
      close(fds[0]); // Close reading pipe.
      char c;
      while(1){
         // Input loop.
      // Keyboard action.
         if((c = getchar()) != EOF ){
            if( c > 'z' ){
               break;
            }
            if( c >= 0x20){
               // printf("[%c] pushed.\n", c);
               write(fds[1], &c, 1);
            }
         } else {
            // No input.
            printf("No Input\n");
            sleep(1);
         }
         if( c == 'q' )
            break;
         
      }
      printf("Child process stopping.\n");
      return 0;
   }

   // I am parent process.
   close(fds[1]); // Close writing pipe.
   printf("hellow anybus\n");
   APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_MODULE_NO_ERROR;
   APPL_AbccHandlerStatusType last_status = APPL_MODULE_NO_ERROR;

   if( ABCC_HwInit() != ABCC_EC_NO_ERROR )
   {
      printf("ABCC_HwInit ERROR\n");
      return( 0 );
   }

   // Set last octet of IP address.
   APPL_SetAddress( 1 ) ;

#if( USE_TIMER_INTERRUPT )
   SetupTimerInterrupt();
#endif

// Test stop process.
//   while(1){
//      sleep(1);
//   }

   char c = 0;

   while( eAbccHandlerStatus == APPL_MODULE_NO_ERROR )
   {
      eAbccHandlerStatus = APPL_HandleAbcc();
      if( last_status != eAbccHandlerStatus){
         printf("eAbccHandlerStatus %d -> %d\n", last_status, eAbccHandlerStatus);
      }

#if( !USE_TIMER_INTERRUPT )
      ABCC_RunTimerSystem( APPL_TIMER_MS );
      DelayMs( APPL_TIMER_MS );
#endif

      switch( eAbccHandlerStatus )
      {
      case APPL_MODULE_NO_ERROR:
         break;
      case APPL_MODULE_RESET:
         Reset();
         break;
      case APPL_MODULE_NOT_DETECTED:
      case APPL_MODULE_NOT_SUPPORTED:
      case APPL_MODULE_NOT_ANSWERING:
      case APPL_MODULE_SHUTDOWN:
      case APPL_MODULE_UNEXPECTED_ERROR:
      default:
         break;
      }

      // Receive command from child process.
      int n = read(fds[0], &c, 1 );
      if( n > 0){
         printf("Parent received [%c]\n", c);
         AnybusModuleControl(c);

         if( c == 'q')
            break;
      }
   }

   return( 0 );
}
