/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
This host application driver runs on Linux for ARM on Xilinx Zynq.

In order to build the project, i.e. calling make, the path to the toolchain
needs to be setup first. Based on the environment and location of the toolchain
the command to setup the path would look something like this in Linux:
export PATH=/opt/Xilinx/SDK/2017.3/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin:$PATH

or this in Windows:
PATH=%PATH%;C:\Xilinx\SDK\2017.3\gnu\aarch32\nt\gcc-arm-linux-gnueabi\bin
