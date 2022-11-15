/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.08.01 (2021-12-15)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.79.01 (2021-11-10)                                     **
**    ABCC Driver    5.08.01 (2021-12-09)                                     **
**                                                                            */
#pragma section code_type ".init"

#ifdef __cplusplus
extern "C" {
#endif

asm void __reset(void);
asm void usr_init();
extern void __start();
extern unsigned long gInterruptVectorTable;
extern unsigned long gInterruptVectorTableEnd;

#ifdef __cplusplus
}
#endif


asm void __reset(void)
{
	//
	//   Enable machine check exceptions, SPE, debug interrupts
	//
	lis		r3, 0x0200
	ori		r3, r3, 0x1200
	mtmsr	r3

	b		__start
}
#define NO_MMU_ENTRIES	6
#define NO_MMU_REGS		4
__declspec(section ".init") const unsigned long int Tlb1Entries[NO_MMU_ENTRIES * NO_MMU_REGS] =
{
		0x10000000, 0x80000800, 0xfc00000a, 0xfc00003f, /* TLB1_0 FLASH */
		0x10010000, 0x80000a00, 0x8000000a, 0x8000003f, /* TLB1_1 PCI 1&2 */
		0x10020000, 0x80000500, 0xe000000a, 0xe000003f, /* TLB1_2 CCSR */
		0x10030000, 0x80000400, 0xefc0000a, 0xefc0003f, /* TLB1_3 PEX IO */
		0x10040000, 0x80000900, 0x00000004, 0x0000003f, /* TLB1_4 DDR */
		0x10050000, 0x80000900, 0x10000004, 0x1000003f, /* TLB1_5 DDR */
};

#define NO_LAW_ENTRIES	12
#define LINE_LEN		2
__declspec(section ".init")  const unsigned long LawArray[NO_LAW_ENTRIES * LINE_LEN] =
{
		0xE0000C08, 0x000FC000, /* LAWBAR 0 */
		0xE0000C10, 0x80400019, /* LAWAR 0 */
		0xE0000C28, 0x00080000, /* LAWBAR 1 */
		0xE0000C30, 0x8020001c, /* LAWAR 1 */
		0xE0000C48, 0x000efc00, /* LAWBAR 2 */
		0xE0000C50, 0x8020000f, /* LAWAR 2*/
		0xE0000C68, 0x000a0000, /* LAWBAR 3 */
		0xE0000C70, 0x8010001c, /* LAWAR 3 */
		0xE0000C88, 0x000efc10, /* LAWBAR 4 */
		0xE0000C90, 0x8010000f, /* LAWAR 4 */
		0xE0000D68, 0x00000000, /* LAWBAR 11 */
		0xE0000D70, 0x80f0001c /* LAWAR 11 */
};

#define NO_DDR_ENTRIES		22

__declspec(section ".init")  extern const unsigned long DdrCtrlArray[NO_DDR_ENTRIES * LINE_LEN] =
{
		0xe0002110, 0x470c0000,
		0xe0002000, 0x0000001f,
		0xe0002008, 0x00000000,
		0xe0002080, 0x80014202,
		0xe0002084, 0x00000000,
		0xe0002104, 0x00220004,
		0xe0002108, 0x5c5b6544,
		0xe000210c, 0x0fa880de,
		0xe0002100, 0x00020000,
		0xe0002114, 0x04401050,
		0xe0002118, 0x80461320,
		0xe000211c, 0x00008000,
		0xe0002120, 0x00000000,
		0xe0002124, 0x09480000,
		0xe0002128, 0xdeadbeef,
		0xe0002130, 0x03000000,
		0xe0002160, 0x00220001,
		0xe0002164, 0x03402400,
		0xe0002170, 0x89080600,
		0xe0002174, 0x8655a608,
		0xe0002b28, 0x00000000,
		0xe0002b2c, 0x00000000
};
#define NO_LB_ENTRIES		4
__declspec(section ".init")  const unsigned long LBArray[NO_LB_ENTRIES * LINE_LEN] =
{
		0xe0005000, 0xFC001001,
		0xe0005004, 0xfc000011,
		0xe00050d0, 0x4000000f,
		0xe00050d4, 0x80000008
};

asm void usr_init()
{
	nofralloc


#ifdef ROM_VERSION

	/* MMU initialization */
	li		r3,NO_MMU_ENTRIES@l
	mtctr	r3

	lis		r4,Tlb1Entries@h
	ori		r4,r4,Tlb1Entries@l

mmu_loop:
	lwz		r3,0(r4)
	lwz		r5,4(r4)
	mtspr 	624, r3
	mtspr 	625, r5
	lwz		r3,8(r4)
	lwz		r5,12(r4)
	mtspr 	626, r3
	mtspr 	627, r5

	tlbwe	/* write TLB */
	msync
	isync
	addi	r4,r4,16 /* get next entry */
	bdnz	mmu_loop

	/* relocate CCSRBAR at 0xe0000000 */
	/* read current value (forces all accesses to configuration space to complete) */
	lis		r5, 0xff70
	ori		r5, r5, 0x0000
	lwz		r4, 0(r5)
	isync

	/* Write the new value to CCSRBAR */
	lis		r5, 0x000e
	ori		r5, r5, 0x0000
	lis		r4, 0xFF700000@h
	stw		r5, 0xFF700000@l(r4)

	/* Perform a load of flash address */
	lis		r5, 0xffff
	ori		r5, r5, 0xfffc
	lwz		r4, 0(r5)
	isync

	/* read new CCSRBAR value */
	lis		r5, 0xe000
	ori		r5, r5, 0x0000
	lwz		r4, 0(r5)
	isync

	/* LAW initialization */
	li		r3,NO_LAW_ENTRIES@l
	mtctr	r3

	lis		r4,LawArray@h
	ori		r4,r4,LawArray@l
law_loop:
	lwz		r3,0(r4)
	lwz		r5,4(r4)
	addi	r4,r4,8
	stw		r5,0(r3)
	bdnz	law_loop

	/* DDR controller initialization */
	li		r3,NO_DDR_ENTRIES@l
	mtctr	r3

	lis		r4,DdrCtrlArray@h
	ori		r4,r4,DdrCtrlArray@l
ddr_loop:
	lwz		r3,0(r4)
	lwz		r5,4(r4)
	addi	r4,r4,8
	stw		r5,0(r3)
	bdnz	ddr_loop

	/* delay */
	lis		r5,0x1
	mtspr 	CTR, r5
wait_loop_c1:
	bdnz	wait_loop_c1

	/* enable controller */
	lis		r4,DdrCtrlArray@h
	ori		r4,r4,DdrCtrlArray@l
	lwz		r3,0(r4)
	lwz		r5,4(r4)
	addis	r5,r5,0x8000
	stw		r5,0(r3)

	lis		r5,0x1
	mtspr 	CTR, r5
wait_loop_c2:
	bdnz	wait_loop_c2

	/* Local Bus initialization */
	li		r3,NO_LB_ENTRIES@l
	mtctr	r3

	lis		r4,LBArray@h
	ori		r4,r4,LBArray@l
lb_loop:
	lwz		r3,0(r4)
	lwz		r5,4(r4)
	addi	r4,r4,8
	stw		r5,0(r3)
	bdnz	lb_loop

	//####################################################################
	//# interrupt vectors initialization
	lis		r4, 0x0000
	lis		r5, 0x0000
	ori		r5, r5, 0x0000
	mtspr	IVPR, r5

	ori		r4, r5, 0x0100
	mtspr	IVOR0, r4
	ori		r4, r5, 0x0200
	mtspr	IVOR1, r4
	ori		r4, r5, 0x0300
	mtspr	IVOR2, r4
	ori		r4, r5, 0x0400
	mtspr	IVOR3, r4
	ori		r4, r5, 0x0500
	mtspr	IVOR4, r4
	ori		r4, r5, 0x0600
	mtspr	IVOR5, r4
	ori		r4, r5, 0x0700
	mtspr	IVOR6, r4
	ori		r4, r5, 0x0c00
	mtspr	IVOR8, r4
	ori		r4, r5, 0x0900
	mtspr	IVOR10, r4
	ori		r4, r5, 0x0f00
	mtspr	IVOR11, r4
	ori		r4, r5, 0x0b00
	mtspr	IVOR12, r4
	ori		r4, r5, 0x1100
	mtspr	IVOR13, r4
	ori		r4, r5, 0x1000
	mtspr	IVOR14, r4
	ori		r4, r5, 0x1500
	mtspr	IVOR15, r4
	ori		r4, r5, 0x1600
	mtspr	IVOR32, r4
	ori		r4, r5, 0x1700
	mtspr	IVOR33, r4
	ori		r4, r5, 0x1800
	mtspr	IVOR34, r4
	ori		r4, r5, 0x1900
	mtspr	IVOR35, r4

	// activate debug interrupt and enable SPU
	lis		r4, 0x0200
	ori		r4, r4, 0x1200
	mtmsr	r4

	// time base enable
	lis		r4, 0x0000
	ori		r4, r4, 0x4000
	mtspr	1008, r4

	//enable trap debug event in DBCR0
	mfspr	r5, 308
	oris	r5, r5, 0x4100
	ori		r5, r5, 0x0000
	mtspr	308, r5


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copy the exception vectors from ROM to RAM
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	lis			r3, gInterruptVectorTable@h
	ori			r3, r3, gInterruptVectorTable@l
	subi		r3,r3,0x0004

	lis			r4, gInterruptVectorTableEnd@h
	ori			r4, r4, gInterruptVectorTableEnd@l

	lis			r5, 0xFFFF
	ori			r5,r5,0xFFFC

loop:
	lwzu		r6, 4(r3)
	stwu		r6, 4(r5)

	cmpw		r3,r4
	blt			loop
#endif

#ifdef CACHE_VERSION
	mfspr	r3,L1CSR0	/* enable d-cache */
	ori		r3, r3, 0x0001
	mtspr	L1CSR0,r3

	mfspr	r3,L1CSR1	/* enable i-cache */
	ori		r3, r3, 0x0001
	mtspr	L1CSR1,r3

	mfspr	r3,BUCSR 	/* enable branch prediction in BUCSR */
	ori		r3, r3, 0x0001
	mtspr	BUCSR,r3

	isync
	msync
#endif

   blr
}
