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
** Platform specific product configuration file.
**
** A symbol defined in this file will override the default product
** configurations in abcc_identification.h, abcc_obj_cfg.h, and abcc_drv_cfg.h.
** It can be used whenever the target platform needs to override a
** default configuration to realize its implementation.
********************************************************************************
********************************************************************************
** Services:
********************************************************************************
********************************************************************************
*/
#ifndef ABCC_PLATFORM_CFG_H_
#define ABCC_PLATFORM_CFG_H_

/*******************************************************************************
** Constants
********************************************************************************
*/

/*
** The ABIP variant of the ABCC40 requires MAC addresses to be assigned from
** the application, so the Ethernet Host Object needs to be enabled by default,
** as well as the MAC address instance attributes.
**
** PORTING ALERT!
**
** MAC address values *must* be unique for each produced unit!
**
** In this example hard-coded values are used for simplicity reasons. This
** will allow for evaluation and basic pre-release testing of the ABIP in an
** isolated network, but hard-coded values must NOT be used in a finished
** implementation! It is up to the implementor to provide unique MAC address
** values to each built unit.
*/

#define ETN_OBJ_ENABLE                          TRUE

#define ETN_IA_MAC_ADDRESS_ENABLE               TRUE
#define ETN_IA_MAC_ADDRESS_VALUE                "\x02\x41\x42\x49\x50\x30" /* "ABIP0" */

#define ETN_IA_PORT1_MAC_ADDRESS_ENABLE         TRUE
#define ETN_IA_PORT1_MAC_ADDRESS_VALUE          "\x02\x41\x42\x49\x50\x31" /* "ABIP1" */

#define ETN_IA_PORT2_MAC_ADDRESS_ENABLE         TRUE
#define ETN_IA_PORT2_MAC_ADDRESS_VALUE          "\x02\x41\x42\x49\x50\x32" /* "ABIP2" */

#define ETN_IA_IP_CONFIGURATION_ENABLE          TRUE
#define ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK   TRUE

/*
** The ABIP variant of the ABCC40 requires the serial number to be assigned
** from the application, so the Application Object needs to be enabled by
** default, as well as the Serial number instance attribute.
**
** Also note that a serial number can be supplied in a network-specific format
** in the network-specific object. If present they will override this value.
**
** PORTING ALERT!
**
** The serial number *must* be unique for each produced unit!
**
** In this example a hard-coded value is used for simplicity reasons. It will
** allow for evaluation and basic pre-release testing of the ABIP, but a hard-
** coded value must NOT be used in a finished implementation! It is up to the
** implementor to provide a unique serial number to each built unit.
*/

#define APP_OBJ_ENABLE                          TRUE

#define APP_IA_SER_NUM_ENABLE                   TRUE
#define APP_IA_SER_NUM_VALUE                    0x11223344

#define SAFE_OBJ_ENABLE                         TRUE

#define SAFE_IA_SAFETY_ENABLED_ENABLE           TRUE
#define SAFE_IA_SAFETY_ENABLED_VALUE            FALSE

#define SAFE_IA_BAUD_RATE_ENABLE                TRUE
#define SAFE_IA_BAUD_RATE_VALUE                 1020000L

#define SAFE_IA_CYCLE_TIME_ENABLE               TRUE
#define SAFE_IA_CYCLE_TIME_VALUE                2

#endif /* #ifndef ABCC_PLATFORM_CFG_H_ */
