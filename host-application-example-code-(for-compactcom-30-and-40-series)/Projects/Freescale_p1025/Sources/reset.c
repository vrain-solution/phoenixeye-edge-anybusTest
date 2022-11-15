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

extern void __reset(void);

#pragma section code_type ".reset"
 
#pragma force_active on
__declspec(section ".reset") 

asm void reset(void);

asm void reset(void)
{
	nofralloc
 	b	__reset
}
#pragma force_active reset
 
#ifdef __cplusplus
}
#endif
