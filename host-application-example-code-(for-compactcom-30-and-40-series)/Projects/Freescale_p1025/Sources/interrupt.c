/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
#include <abcc_sw_port.h>

#ifdef __cplusplus
extern "C" {
#endif


void InterruptHandler(long cause);
asm unsigned int Get_ProcessorID();

#ifdef __cplusplus
}
#endif

#pragma e500v2_floatingpoint off

void InterruptHandler(long cause)
{
	register long aMSR;
	#pragma unused (aMSR) /* Do not warn - aMSR used in asm code */

	asm
	{
		/*If we are using SPE,  we need to make sure to enable the SPE bit in the  MSR*/
		#if defined(__PPCZen__) || defined(__PPCe500__) || defined(__PPCe500v2__)
			mfmsr  aMSR
			oris   aMSR,aMSR,0x0200
			mtmsr  aMSR
		#endif
	}

	if (cause == 0xC00)
	{
		ABCC_PORT_DebugPrint( ( "Core #%d: system call exception handler\r\n", Get_ProcessorID() ) );
	}
	else
	{
		ABCC_PORT_DebugPrint( ( "Core #%d: InterruptHandler: 0x%x exception.\r\n", Get_ProcessorID(), cause ) );
	}
}

asm unsigned int Get_ProcessorID()
{
	mfspr r3, 286
}

#pragma e500v2_floatingpoint reset
