########################################################################################
# Initialization file for P1025TWR board 
# Clock Configuration:
#       CPU0: 384 MHz, CPU1: 384 MHz, CCB: 384 MHz,
#       DDR: 666 MHz, SYSCLK: 64 MHz
########################################################################################

variable CCSRBAR 0xff700000

proc CCSR {reg_off} {
	global CCSRBAR
	
	return p:0x[format %x [expr {$CCSRBAR + $reg_off}]]
}

proc init_board {} {

	global CCSRBAR

	# configure local access windows
	
	# LAWBAR0 - Local Bus NOR
	# bit 8 - 31 = 0xFC000000 - base addr
	mem [CCSR 0xc08] = 0x000FC000

	# LAWAR0
	# bit 0 = 1 - enable window
	# bit 7-11 = 00100 - Local Bus
	# bit 26 - 31 =  011001 64M - size
	mem [CCSR 0xc10] = 0x80400019

	# LAWBAR1 - PEX1
	# bit 8 - 31 = 0x80000000 - base addr
	mem [CCSR 0xc28] = 0x00080000

	# LAWAR1
	# bit 0 = 1 - enable window
	# bit 7-11 = 00010 - PEX1
	# bit 26 - 31 =  011100 512M - size
	mem [CCSR 0xc30] = 0x8020001c

	# LAWBAR2 - PEX 1
	# bit 8 - 31 = 0xEFC20000 - base addr
	mem [CCSR 0xc48] = 0x000efc00

	# LAWAR2
	# bit 0 = 1 - enable window
	# bit 7-11 = 00010 - PEX1
	# bit 26-31 = 001111 64k - size
	mem [CCSR 0xc50] = 0x8020000f


	# LAWBAR3 - PEX2
	# bit 8 - 31 = 0xA0000000 - base addr
	mem [CCSR 0xc68] = 0x000a0000

	# LAWAR3
	# bit 0 = 1 - enable window
	# bit 7-11 = 00001 - PEX2
	# bit 26 - 31 =  011100 512M - size
	mem [CCSR 0xc70] = 0x8010001c

	# LAWBAR4 - PEX 2
	# bit 8 - 31 = 0xEFC10000 - base addr
	mem [CCSR 0xc88] = 0x000efc10

	# LAWAR4
	# bit 0 = 1 - enable window
	# bit 7-11 = 00001 - PEX2
	# bit 26-31 = 001111 64k - size
	mem [CCSR 0xc90] = 0x8010000f

	# LAWBAR11 - DDR
	# bit 8 - 31 = 0x00000000 - base addr
	mem [CCSR 0xd68] = 0x00000000

	# LAWAR11
	# bit 0 = 1 - enable window
	# bit 7-11 = 01111 - DDR
	# bit 26 - 31 =  011100 512MB - size
	mem [CCSR 0xd70] = 0x80f0001c
	
	##################################################################################
	# DDR Controllers Setup

	# DDR_SDRAM_CFG
	mem [CCSR 0x2110] = 0x470c0000

	#CS0_BNDS
	mem [CCSR 0x2000] = 0x0000001f

	#CS1_BNDS
	mem [CCSR 0x2008] = 0x00000000

	#CS0_CNFG
	mem [CCSR 0x2080] = 0x80014202

	#CS1_CNFG
	mem [CCSR 0x2084] = 0x00000000

	# TIMING_CFG_0
	mem [CCSR 0x2104] = 0x00220004

	# TIMING_CFG_1
	mem [CCSR 0x2108] = 0x5c5b6544

	# TIMING_CFG_2
	mem [CCSR 0x210c]= 0x0fa880de

	# TIMING_CFG_3
	mem [CCSR 0x2100] = 0x00020000

	# DDR_SDRAM_CFG_2
	mem [CCSR 0x2114] = 0x04401050

	# DDR_SDRAM_MODE
	mem [CCSR 0x2118] = 0x80461320

	# DDR_SDRAM_MODE_2
	mem [CCSR 0x211c] = 0x00008000

	# DDR_SDRAM_MD_CNTL
	mem [CCSR 0x2120] = 0x00000000

	# DDR_SDRAM_INTERVAL
	mem [CCSR 0x2124] = 0x09480000

	# DDR_DATA_INIT
	mem [CCSR 0x2128] = 0xdeadbeef	
	
	# DDR_SDRAM_CLK_CNTL
	mem [CCSR 0x2130] = 0x03000000	

	# TIMING_CFG_4
	mem [CCSR 0x2160] = 0x00220001

	# TIMING_CFG_5
	mem [CCSR 0x2164] = 0x03402400

	# DDR_ZQ_CNTL
	mem [CCSR 0x2170] = 0x89080600

	# DDR_WRLVL_CNTL
	mem [CCSR 0x2174] = 0x8655a608

	# DDR_CDR_1 
	mem [CCSR 0x2b28] = 0x00000000
	
	# DDR_CDR_2
	mem [CCSR 0x2b2c] = 0x00000000

	#delay before enable
	wait 500

	# DDR_SDRAM_CFG
	mem [CCSR 0x2110] = 0xc70c0000
	
	#wait for DRAM data initialization
	wait 2000


	##################################################################################
	# configure Local Bus memory controller 

	# CS0
	
	# BR0 base address at 0xFC000000, port size 16 bit, GPCM, Data error checking disabled, but normal parity generation for GPCM and UPM.
	mem [CCSR 0x5000] = 0xFC001001
	# OR0 64MB flash size
	mem [CCSR 0x5004] = 0xfc0000b1
		
		
	# LBCR
	mem [CCSR 0x50d0] = 0x4000000f
	# LCRR
	mem [CCSR 0x50d4] = 0x80000008
	
	# L2CTL
	# bit 0 = 0 		- L2E: L2 disabled
	# bit 1 = 1			- L2I: L2 flash invalidate
	mem [CCSR 0x20000] = 0x[format %x [expr {[mem [CCSR 0x20000] -np] & 0x7FFFFFFF | 0x40000000}]]	
}

proc apply_e500v2_workaround {} {
	# After reset, e500 cores need to run before being able to enter debug mode.
	# Work-around: set a HW BP at reset address and run the core; after the core hits the BP, it enters debug mode
	# e500 cores need to have valid opcode at the interrupt vector

	variable SPR_GROUP "e500 Special Purpose Registers/"	

	#######################################################################
	# Set a breakpoint at the reset address 
	reg ${SPR_GROUP}IAC1 = 0xfffffffc
	reg ${SPR_GROUP}DBCR0 = 0x40800000
	reg ${SPR_GROUP}DBCR1 = 0x00000000

	# Run the core
	config runcontrolsync off
	go
	wait 50   
	config runcontrolsync on
	stop

	# Clear affected registers 	
	reg ${SPR_GROUP}DBSR  = 0x01CF0000
	reg ${SPR_GROUP}DBCR0 = 0x41000000
	reg ${SPR_GROUP}IAC1  = 0x00000000
	reg ${SPR_GROUP}CSRR0 = 0x00000000
	reg ${SPR_GROUP}CSRR1 = 0x00000000
	
}

proc P1025TWR_init_core {} {

	global CCSRBAR
	
	variable proc_id [expr {[reg PIR %d -np]} ]
	variable CAM_GROUP "regPPCTLB1/"
	variable SPR_GROUP "e500 Special Purpose Registers/"
	variable GPR_GROUP "General Purpose Registers/"
	variable SSP_GROUP "Standard Special Purpose Registers/"
	variable e500_CM_GROUP "e500 Coherency Module/"
	
	
	if {$proc_id == 0} {
		
		##################################################################################
		# move CCSR at 0xE0000000
	
		# CCSRBAR
		# bit 8 - 23 - BASE_ADDR
		mem [CCSR 0x0] = 0x000e0000
		set CCSRBAR 0xe0000000
	
		###################################################################################
		# invalidate BR0
		mem [CCSR 0x5000] = 0x00001000	
		
		# ABIST off
		# L2ERRDIS[TPARDIS]=1  L2ERRDIS[MBECCDIS]=1 L2ERRDIS[SBECCDIS]=1 L2ERRDIS[L2CFGDIS]=1
		mem [CCSR 0x20e44] = 0x0000001D	
	}
	

	# activate debug interrupt and enable SPU
	reg	${SSP_GROUP}MSR = 0x02000200
	
	
	##################################################################################
	#	
	#	Memory Map
	#
	#   0x00000000  0x1FFFFFFF  DDR		  			512MB
	#   0x80000000  0x9FFFFFFF  PEX1				512MB
	#   0xA0000000  0xBFFFFFFF  PEX2				512MB
	#   0xE0000000  0xE01FFFFF  CCSR   				2M
	#   0xEFC00000  0xEFC1FFFF  PEX I/O     		128k
	#   0xFC000000	0xFFFFFFFF  LocalBus NOR FLASH	64M
	#
	##################################################################################
	
	##################################################################################
	# MMU initialization

	# define 64MB   TLB1 entry 0: 0xFC000000 - 0xFFFFFFFF; for Local Bus, cache inhibited, guarded
	reg ${CAM_GROUP}L2MMU_CAM0 = 0x80001FCAFC080000FC000000FC000001
	
	# define 1GB    TLB entry 1: 0x80000000 - 0xBFFFFFFF; for PCI Express 1&2, cache inhibited, guarded
	reg ${CAM_GROUP}L2MMU_CAM1 = 0xA0007FCAFC0800008000000080000001

	# define 1MB	TLB1 entry 2: 0xE0000000 - 0xE00FFFFF; CCSR Space, non cacheable, guarded
	reg ${CAM_GROUP}L2MMU_CAM2 = 0x500003CAFC080000E0000000E0000001

	# define 256KB  TLB entry 3: 0xEFC00000 - 0xEFCFFFFF; for PCI Express I/O, cache inhibited, guarded
	reg ${CAM_GROUP}L2MMU_CAM3 = 0x400001CAFC080000EFC00000EFC00001
	
	# define 256MB	TLB1 entry 4: 0x00000000 - 0x0FFFFFFF; DDR, cacheable, not guarded
	reg ${CAM_GROUP}L2MMU_CAM4 = 0x90003FC41C0800000000000000000001  
	
	# define 256MB	TLB1 entry 5: 0x10000000 - 0x1FFFFFFF; DDR, cacheable, not guarded
	reg ${CAM_GROUP}L2MMU_CAM5 = 0x90003FC41C0800001000000010000001  	
	
	# disable & invalidate all core caches
	reg ${SPR_GROUP}L1CSR0 = 0x2
	reg ${SPR_GROUP}L1CSR1 = 0x0
	reg ${SPR_GROUP}L1CSR1 = 0x2	
	
	# init board, only when the init is run for core 0
	if {$proc_id == 0} {
        init_board
    }
    
	##################################################################################
	# Interrupt vectors initialization 
	# interrupt vectors in RAM at 0x00000000
	# IVPR (default reset value) 
	reg ${SPR_GROUP}IVPR = 0x${proc_id}0000000
	
	# interrupt vector offset registers 
	# IVOR0 - critical input
	reg ${SPR_GROUP}IVOR0 = 0x00000100	
	# IVOR1 - machine check
	reg ${SPR_GROUP}IVOR1 = 0x00000200	
	# IVOR2 - data storage
	reg ${SPR_GROUP}IVOR2 = 0x00000300	
	# IVOR3 - instruction storage
	reg ${SPR_GROUP}IVOR3 = 0x00000400	
	# IVOR4 - external input
	reg ${SPR_GROUP}IVOR4 = 0x00000500	
	# IVOR5 - alignment
	reg ${SPR_GROUP}IVOR5 = 0x00000600	
	# IVOR6 - program
	reg ${SPR_GROUP}IVOR6 = 0x00000700
	# IVOR8 - system call
	reg ${SPR_GROUP}IVOR8 = 0x00000c00	
	# IVOR10 - decrementer
	reg ${SPR_GROUP}IVOR10 = 0x00000900	
	# IVOR11 - fixed-interval timer interrupt
	reg ${SPR_GROUP}IVOR11 = 0x00000f00	
	# IVOR12 - watchdog timer interrupt
	reg ${SPR_GROUP}IVOR12 = 0x00000b00	
	# IVOR13 - data TLB errror
	reg ${SPR_GROUP}IVOR13 = 0x00001100	
	# IVOR14 - instruction TLB error
	reg ${SPR_GROUP}IVOR14 = 0x00001000	
	# IVOR15 - debug
	reg ${SPR_GROUP}IVOR15 = 0x00001500	
	# IVOR32 - SPE-APU unavailable
	reg ${SPR_GROUP}IVOR32 = 0x00001600	
	# IVOR33 - SPE-floating point data exception
	reg ${SPR_GROUP}IVOR33 = 0x00001700
	# IVOR34 - SPE-floating point round exception
	reg ${SPR_GROUP}IVOR34 = 0x00001800
	# IVOR35 - performance monitor
	reg ${SPR_GROUP}IVOR35 = 0x00001900	

	# Put a valid opcode at debug and program exception vector addresses
    mem v:0x${proc_id}0000700 = 0x48000000
 	mem v:0x${proc_id}0001500 = 0x48000000
	
	##################################################################################
	apply_e500v2_workaround
    
   	##################################################################################
	# Debugger settings
	
	# enable floating point
	reg ${SSP_GROUP}MSR = 0x02001200
	
	# time base enable
	reg ${SPR_GROUP}HID0 = 0x00004000
	
	
	#Trap debug event enable
	reg ${SPR_GROUP}DBCR0 = 0x41000000

	# for debugging starting at program entry point when stack is not initialized
	reg ${GPR_GROUP}SP = 0x0000000F

	if {$proc_id == 0} {
		# BRRL - enable all cores
		mem [CCSR 0x1010] = 0x03000000
	}
}

proc envsetup {} {
	# Environment Setup
	radix x 
	config hexprefix 0x
	config MemIdentifier v
	config MemWidth 32 
	config MemAccess 32 
	config MemSwap off
}

#-------------------------------------------------------------------------------
# Main                                                                          
#-------------------------------------------------------------------------------

  envsetup
  
  P1025TWR_init_core
