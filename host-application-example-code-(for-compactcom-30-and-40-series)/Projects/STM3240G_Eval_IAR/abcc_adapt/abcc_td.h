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
** Definition of ABCC types
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_TD_H
#define ABCC_TD_H

#include <stdint.h>

/*******************************************************************************
**
** Constants
**
********************************************************************************
*/

/* #define ABCC_SYS_BIG_ENDIAN  */
/* #define ABCC_SYS_16_BIT_CHAR */

/*---------------------------------------------------------------------------
**
** BOOL
** BOOL8
**
** INT8
** INT16
** INT32
** INT64
**
** UINT8
** UINT16
** UINT32
** UINT64
**
** FLOAT32
** FLOAT64
**
** The standard boolean data type, 8-bit.
** The standard signed 8, 16, 32, and 64 bit data types (respectively).
** The standard unsigned 8, 16, 32, and 64 bit data types (respectively).
**---------------------------------------------------------------------------
*/
typedef int             BOOL;
typedef unsigned char   BOOL8;
typedef uint8_t         UINT8;
typedef int8_t          INT8;
typedef uint16_t        UINT16;
typedef int16_t         INT16;
typedef uint32_t        UINT32;
typedef int32_t         INT32;
typedef uint64_t        UINT64;
typedef int64_t         INT64;
typedef float           FLOAT32;
typedef double          FLOAT64;

/*------------------------------------------------------------------------------
** LeINT16
** LeINT32
** LeINT64
** LeUINT16
** LeUINT32
** LeUINT64
**
** Little endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     LeINT16;
typedef INT32     LeINT32;
typedef INT64     LeINT64;
typedef UINT16    LeUINT16;
typedef UINT32    LeUINT32;
typedef UINT64    LeUINT64;

/*------------------------------------------------------------------------------
** BeINT16
** BeINT32
** BeINT64
** BeUINT16
** BeUINT32
** BeUINT64
**
** Big endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     BeINT16;
typedef INT32     BeINT32;
typedef INT64     BeINT64;
typedef UINT16    BeUINT16;
typedef UINT32    BeUINT32;
typedef UINT64    BeUINT64;

/*---------------------------------------------------------------------------
**
** FALSE
** TRUE
**
** These are the symbolic constants for true and false used in boolean
** data type comparisons.
**
**---------------------------------------------------------------------------
*/
#ifndef FALSE
   #define FALSE     0
#endif

#ifndef TRUE
   #define TRUE      ( !FALSE )
#endif

/*---------------------------------------------------------------------------
**
** NULL
**
** Default value for invalid pointers.
**
**---------------------------------------------------------------------------
*/
#ifndef NULL
#define NULL 0
#endif

#ifdef __cplusplus
   #define CPLUSPLUS
#endif

#ifdef CPLUSPLUS
   #define EXTFUNC extern "C"
   #define EXTVAR extern "C"
#else
   #define EXTFUNC extern
   #define EXTVAR extern
#endif

/*---------------------------------------------------------------------------
** Compiler/platform-specific packing macros.
**---------------------------------------------------------------------------
*/
/*
** "IAR C/C++ Development Guide", "Twenty-fifth edition: May 2019"
** Document "DARM-25", "EWARM_DevelopmentGuide.ENU.pdf"
**
** Chapter "Descriptions of extended keywords", page 375
** Chapter "Supported GCC attributes", page 381
** Chapter "Descriptions of pragma directives", page 400
** Chapter "Descriptions of compiler options", page 465
** Packing-related restrictions are listed on page 375
*/
#define ABCC_SYS_PACK_ON   _Pragma("pack(1)")
#define ABCC_SYS_PACK_OFF  _Pragma("pack()")
#define PACKED_STRUCT

#endif  /* inclusion lock */
