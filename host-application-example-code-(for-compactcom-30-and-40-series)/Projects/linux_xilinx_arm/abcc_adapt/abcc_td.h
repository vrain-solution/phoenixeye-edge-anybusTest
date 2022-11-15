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
** Services:
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_TD_H
#define ABCC_TD_H

/*******************************************************************************
** Constants
********************************************************************************
*/

/*---------------------------------------------------------------------------
** These are the symbolic constants for true and false used in boolean
** data type comparisons.
**---------------------------------------------------------------------------
*/
#ifndef FALSE
   #define FALSE     0
#endif

#ifndef TRUE
   #define TRUE      ( !FALSE )
#endif

/*---------------------------------------------------------------------------
** Default value for invalid pointers.
**---------------------------------------------------------------------------
*/
#ifndef NULL
#define NULL 0
#endif

/*---------------------------------------------------------------------------
** C++ constants
**---------------------------------------------------------------------------
*/
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

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*---------------------------------------------------------------------------
**
** BOOL8
**
** INT8
** INT16
** INT32
**
** UINT8
** UINT16
** UINT32
**
** FLOAT32
**
** The standard boolean data type, 8-bit.
** The standard signed 8, 16, and 32 bit data types (respectively).
** The standard unsigned 8, 16, and 32 bit data types (respectively).
**---------------------------------------------------------------------------
*/
typedef int             BOOL;
typedef unsigned char   BOOL8;
typedef unsigned char   UINT8;
typedef signed char     INT8;
typedef unsigned short  UINT16;
typedef signed short    INT16;
typedef unsigned int    UINT32;
typedef signed int      INT32;
typedef float           FLOAT32;


/*------------------------------------------------------------------------------
** LeINT16
** LeINT32
** LeUINT16
** LeUINT32
**
** Little endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     LeINT16;
typedef INT32     LeINT32;
typedef UINT16    LeUINT16;
typedef UINT32    LeUINT32;


/*------------------------------------------------------------------------------
** BeINT16
** BeINT32
** BeUINT16
** BeUINT32
**
** Big endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     BeINT16;
typedef INT32     BeINT32;
typedef UINT16    BeUINT16;
typedef UINT32    BeUINT32;

/*---------------------------------------------------------------------------
** Compiler/platform-specific packing macros.
**---------------------------------------------------------------------------
*/
/*
** https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
** https://gcc.gnu.org/onlinedocs/gcc-10.2.0/gcc/Common-Type-Attributes.html
*/
#define ABCC_SYS_PACK_ON
#define ABCC_SYS_PACK_OFF
#define PACKED_STRUCT       __attribute__((packed))

#endif  /* inclusion lock */
