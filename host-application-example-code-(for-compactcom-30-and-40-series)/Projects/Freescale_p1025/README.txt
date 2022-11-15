/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
********************************************************************************
** README
********************************************************************************

This project is created with CodeWarrior Development Studio 10.4.0.
The software is intended to be used together with the ABCC adapter board.
The project is based on the Bare board project wizard example.
The code is developed and debugged using a CodeWarror USB TAP. The configuration is included in the project.


Build configurations RAM is supported.

UART:
	The static library UART2_P1025TWR.UC.a is used for serial communication ( debugging and serial operting mode )
	The library is built using the serial example project found in <CWInstallDir>\PA\PA_Support\Serial\P1025TWR_serial.
	
I2C:
	I2C1 and I2C2 are used for the IO expanders on the ABCC adapter board

SPI:
	SPI0 is used to communicate with the ABCC module in SPI operating mode.


