########################################################################################
# Initialization file for P1025TWR board - SRAM
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


	##################################################################################
	# configure internal SRAM at 0x00000000

	# L2CTL
	# bit 0 = 0 		- L2E: L2 SRAM disabled
	# bit 2-3 = 01 		- L2SIZ: = 256K
	# bit 13-15 = 001 	- L2SRAM: Entire array is a single SRAM (265KB)
	mem [CCSR 0x20000] = 0x10010000


	# L2SRBAR0
	# bit 0-17 = BASE addr: 0x00000000
	mem [CCSR 0x20100] = 0x00000000

	# L2SRBAREA0
	# bit 28-31 = EXTENTED BASE addr: 0x00000000
	mem [CCSR 0x20104] = 0x00000000

	# L2CTL
	# bit 0 = 1 		- L2E: L2 SRAM enable
	mem [CCSR 0x20000] = 0x90010000
	
	
	# configure local access windows

	# LAWBAR0 - Local Bus NOR
	# bit 8 - 31 = 0xFC000000 - base addr
	mem [CCSR 0xc08] = 0x000FC000

	# LAWAR0
	# bit 0 = 1 - enable window
	# bit 7-11 = 00100 - Local Bus
	# bit 26 - 31 =  011001 64M - size
	mem [CCSR 0xc10] = 0x80400019


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
	
	variable CAM_GROUP "regPPCTLB1/"
	variable SPR_GROUP "e500 Special Purpose Registers/"
	variable GPR_GROUP "General Purpose Registers/"
	variable SSP_GROUP "Standard Special Purpose Registers/"
	

	##################################################################################
	# move CCSR at 0xE0000000

	# CCSRBAR
	# bit 8 - 23 - BASE_ADDR
	mem [CCSR 0x0] = 0x000e0000
	set CCSRBAR 0xe0000000

	#######################################################################
	# invalidate BR0
	mem [CCSR 0x5000] = 0x00001000	
	
	# ABIST off
	# L2ERRDIS[TPARDIS]=1  L2ERRDIS[MBECCDIS]=1 L2ERRDIS[SBECCDIS]=1 L2ERRDIS[L2CFGDIS]=1
	mem [CCSR 0x20e44] = 0x0000001D		

	# activate debug interrupt and enable SPU
	reg MSR = 0x02000200
	
	##################################################################################
	#	
	#	Memory Map
	#
	#   0x00000000  0x0003FFFF 	SRAM	            256K 	
	#   0xE0000000  0xE00FFFFF  CCSR   				1M
	#   0xFC000000	0xFFFFFFFF  LocalBus NOR FLASH	64M
	#
	##################################################################################
	
	##################################################################################
	# MMU initialization
	
	# define 64MB   TLB1 entry 0: 0xFC000000 - 0xFFFFFFFF; for Local Bus, cache inhibited, guarded
	reg ${CAM_GROUP}L2MMU_CAM0 = 0x80001FCAFC080000FC000000FC000001

	# define 1MB	TLB1 entry 1: 0xE0000000 - 0xE00FFFFF; CCSR Space, non cacheable, guarded
	reg ${CAM_GROUP}L2MMU_CAM1 = 0x500003CAFC080000E0000000E0000001
	
	# define 256KB  TLB1 entry 2: 0x00000000 - 0x0003FFFF; for internal chip SRAM
	reg ${CAM_GROUP}L2MMU_CAM2 = 0x400001C0FC0800000000000000000001
	

	##################################################################################
	# disable Boot Page Translation Register
	mem [CCSR 0x20] = 0x00000000 

	# Invalidate again BR0 to prevent flash data damage in case 
	# the boot sequencer re-enables CS0 access
	mem [CCSR 0x5000] = 0x00001000 
	
	init_board
    
	##################################################################################
	# Interrupt vectors initialization 
	# interrupt vectors in RAM at 0x00000000
	# IVPR (default reset value) 
	reg ${SPR_GROUP}IVPR = 0x00000000
	
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

    
   	##################################################################################
	# Debugger settings
	
	# infinite loop at program exception to prevent taking the exception again
	mem v:0x00000700 = 0x48000000
	mem v:0x00001500 = 0x48000000
	
	##################################################################################
	apply_e500v2_workaround	
	
	# enable floating point
	reg ${SSP_GROUP}MSR = 0x02001200
	
	# time base enable
	reg ${SPR_GROUP}HID0 = 0x00004000
	
	
	#Trap debug event enable
	reg ${SPR_GROUP}DBCR0 = 0x41000000

	# for debugging starting at program entry point when stack is not initialized
	reg ${GPR_GROUP}SP = 0x0000000F

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
