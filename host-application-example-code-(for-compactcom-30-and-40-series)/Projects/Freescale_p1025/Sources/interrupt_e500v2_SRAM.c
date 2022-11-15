/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
#ifdef __cplusplus
extern "C" {
#endif


void InterruptHandler(long cause);

#ifdef __cplusplus
}
#endif

#pragma e500v2_floatingpoint off

void InterruptHandler(long cause)
{
	register long aMSR;
	#pragma unused (aMSR) /* Do not warn - aMSR used in asm code */
	#pragma unused (cause)
	
	asm
	{
		/*If we are using SPE,  we need to make sure to enable the SPE bit in the  MSR*/
		#if defined(__PPCZen__) || defined(__PPCe500__) || defined(__PPCe500v2__)
			mfmsr  aMSR
			oris   aMSR,aMSR,0x0200
			mtmsr  aMSR
		#endif	
	}
}

#pragma e500v2_floatingpoint reset
