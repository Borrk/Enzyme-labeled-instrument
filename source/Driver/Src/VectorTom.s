;    GET option.a
;    GET memcfg.a

;;;  the vesion is from tom

_ISR_STARTADDRESS   EQU   0xc7fff00 

;Bank 0 parameter
;B0_Tacs		EQU	0x0	;0clk   //66mHz
;B0_Tcos		EQU	0x0	;0clk
;B0_Tacc		EQU	0x3	;4clk
;B0_Tcoh		EQU	0x0	;0clk
;B0_Tah		EQU	0x0	;0clk
;B0_Tacp		EQU	0x0	;0clk
;B0_PMC		EQU	0x0	;normal(1data)

B0_Tacs		EQU	0x0	;0clk  //50mHZ
B0_Tcos		EQU	0x0	;0clk
B0_Tacc		EQU	0x2	;10clk
B0_Tcoh		EQU	0x0	;0clk
B0_Tah		EQU	0x0	;0clk
B0_Tacp		EQU	0x0	;0clk
B0_PMC		EQU	0x0	;normal(1data)

;Bank 1 parameter
B1_Tacs		EQU	0x3	;4clk
B1_Tcos		EQU	0x3	;4clk
B1_Tacc		EQU	0x7	;14clk
B1_Tcoh		EQU	0x3	;4clk
B1_Tah		EQU	0x3	;4clk
B1_Tacp		EQU	0x3	;6clk
B1_PMC		EQU	0x0	;normal(1data)

;Bank 2 parameter
B2_Tacs		EQU	0x3	;4clk
B2_Tcos		EQU	0x3	;4clk
B2_Tacc		EQU	0x7	;14clk
B2_Tcoh		EQU	0x3	;4clk
B2_Tah		EQU	0x3	;4clk
B2_Tacp		EQU	0x3	;6clk
B2_PMC		EQU	0x0	;normal(1data)

;Bank 3 parameter
B3_Tacs		EQU	0x3	;4clk
B3_Tcos		EQU	0x3	;4clk
B3_Tacc		EQU	0x7	;14clk
B3_Tcoh		EQU	0x3	;4clk
B3_Tah		EQU	0x3	;4clk
B3_Tacp		EQU	0x3	;6clk
B3_PMC		EQU	0x0	;normal(1data)

;Bank 4 parameter    //NAND FLASH
B4_Tacs		EQU	0x3	;4clk
B4_Tcos		EQU	0x3	;4clk
B4_Tacc		EQU	0x7	;14clk
B4_Tcoh		EQU	0x3	;4clk
B4_Tah		EQU	0x3	;4clk
B4_Tacp		EQU	0x3	;6clk
B4_PMC		EQU	0x0	;normal(1data)

;Bank 5 parameter
B5_Tacs		EQU	0x3	;4clk
B5_Tcos		EQU	0x3	;4clk
B5_Tacc		EQU	0x7	;14clk
B5_Tcoh		EQU	0x3	;4clk
B5_Tah		EQU	0x3	;4clk
B5_Tacp		EQU	0x3	;6clk
B5_PMC		EQU	0x0	;normal(1data)

;Bank 6(if SROM) parameter
B6_Tacs		EQU	0x3	;4clk
B6_Tcos		EQU	0x3	;4clk
B6_Tacc		EQU	0x7	;14clk
B6_Tcoh		EQU	0x3	;4clk
B6_Tah		EQU	0x3	;4clk
B6_Tacp		EQU	0x3	;6clk
B6_PMC		EQU	0x0	;normal(1data)

;Bank 7(if SROM) parameter
B7_Tacs		EQU	0x3	;4clk
B7_Tcos		EQU	0x3	;4clk
B7_Tacc		EQU	0x7	;14clk
B7_Tcoh		EQU	0x3	;4clk
B7_Tah		EQU	0x3	;4clk
B7_Tacp		EQU	0x3	;6clk
B7_PMC		EQU	0x0	;normal(1data)

;Bank 6 parameter
B6_MT		EQU	0x3	;SDRAM
B6_Trcd		EQU	0x0	;2clk
B6_SCAN		EQU 0x1  ;hy57v64820hg 9bit ;0x0	;8bit

;Bank 7 parameter
B7_MT		EQU	0x3	;SDRAM
B7_Trcd		EQU	0x0;   2	;2clk
B7_SCAN		EQU	0x1	;9bit

;REFRESH parameter
REFEN		EQU	0x1	;Refresh enable
TREFMD		EQU	0x0	;CBR(CAS before RAS)/Auto refresh
Trp		    EQU	0x11	;0clk
Trc		    EQU	0x0	;5clk
Tchr		EQU	0x0	;3clk
REFCNT		EQU	0x0	;period=62.5s, MCLK=60Mhz


;Interrupt Control
INTPND	    EQU	0x01e00004
INTMOD	    EQU	0x01e00008
INTMSK	    EQU	0x01e0000c
I_ISPR	    EQU	0x01e00020
I_CMST	    EQU	0x01e0001c

;Watchdog timer
WTCON	    EQU	0x01d30000

;Clock Controller
PLLCON	    EQU	0x01d80000
CLKCON	    EQU	0x01d80004
LOCKTIME    EQU	0x01d8000c
CLKSLOW	    EQU 0x01d80008
;port pdate

PDATG       EQU 0x01d20044

PDATF       EQU 0x01d20038
PCONF       EQU 0x01d20034
PUPF        EQU 0x01d2003c
; SYSCFG
SYSCFG      EQU 0x01c00000


;Memory Controller
REFRESH	    EQU 0x01c80024

;BDMA destination register
BDIDES0     EQU 0x1f80008
BDIDES1     EQU 0x1f80028

;Pre-defined constants
USERMODE    EQU	0x10
FIQMODE	    EQU	0x11
IRQMODE	    EQU	0x12
SVCMODE	    EQU	0x13
ABORTMODE   EQU	0x17
UNDEFMODE   EQU	0x1b
MODEMASK    EQU	0x1f
NOINT	    EQU	0xc0

;check if tasm.exe is used.
    GBLL    THUMBCODE
    [ {CONFIG} = 16	
THUMBCODE SETL	{TRUE}
    CODE32
    |   
THUMBCODE SETL	{FALSE}
    ]

    [ THUMBCODE
    CODE32   ;for start-up code for Thumb mode
    ]

    MACRO
$HandlerLabel HANDLER $HandleLabel


$HandlerLabel
    sub	    sp,sp,#4	    ;decrement sp(to store jump address)
    stmfd   sp!,{r0}	    ;PUSH the work register to stack(lr does not push because it return to original address)
    ldr	    r0,=$HandleLabel;load the address of HandleXXX to r0
    ldr	    r0,[r0]	    ;load the contents(service routine start address) of HandleXXX
    str	    r0,[sp,#4]	    ;store the contents(ISR) of HandleXXX to stack
    ldmfd   sp!,{r0,pc}	    ;POP the work register and pc(jump to ISR)
    MEND

    IMPORT	|Image$$RO$$Limit|  ; End of ROM code (=start of ROM data)
    IMPORT	|Image$$RW$$Base|   ; Base of RAM to initialise
    IMPORT	|Image$$ZI$$Base|   ; Base and limit of area
    IMPORT	|Image$$ZI$$Limit|  ; to zero initialise

    ;IMPORT __main    ; The main entry of mon program 

    AREA    Selfboot,CODE,READONLY
    b ResetHandler  ;for debug
    b HandlerUndef  ;handlerUndef
    b HandlerSWI    ;SWI interrupt handler
    b HandlerPabort ;handlerPAbort
    b HandlerDabort ;handlerDAbort
    b .		        ;handlerReserved
    subs pc,lr,#4   ;(old=) b HandlerIRQ
    subs pc,lr,#4   ;(old=) b HandlerFIQ
	;***IMPORTANT NOTE***
	;If the H/W vectored interrutp mode is enabled, The above two instructions should
	;be changed like below, to work-around with H/W bug of S3C44B0X interrupt controller. 
	; b HandlerIRQ  ->  subs pc,lr,#4
	; b HandlerIRQ  ->  subs pc,lr,#4

VECTOR_BRANCH
    ldr pc,=HandlerEINT0    ;mGA    H/W interrupt vector table
    ldr pc,=HandlerEINT1    ;	
    ldr pc,=HandlerEINT2    ;
    ldr pc,=HandlerEINT3    ;
    ldr pc,=HandlerEINT4567 ;
    ldr pc,=HandlerTICK	    ;mGA
    b .
    b .
    ldr pc,=HandlerZDMA0    ;mGB
    ldr pc,=HandlerZDMA1    ;
    ldr pc,=HandlerBDMA0    ;
    ldr pc,=HandlerBDMA1    ;
    ldr pc,=HandlerWDT	    ;
    ldr pc,=HandlerUERR01   ;mGB
    b .
    b .
    ldr pc,=HandlerTIMER0   ;mGC
    ldr pc,=HandlerTIMER1   ;
    ldr pc,=HandlerTIMER2   ;
    ldr pc,=HandlerTIMER3   ;
    ldr pc,=HandlerTIMER4   ;
    ldr pc,=HandlerTIMER5   ;mGC
    b .
    b .
    ldr pc,=HandlerURXD0    ;mGD
    ldr pc,=HandlerURXD1    ;
    ldr pc,=HandlerIIC	    ;
    ldr pc,=HandlerSIO	    ;
    ldr pc,=HandlerUTXD0    ;
    ldr pc,=HandlerUTXD1    ;mGD
    b .
    b .
    ldr pc,=HandlerRTC	    ;mGKA
    b .						;
    b .						;
    b .						;
    b .						;
    b .						;mGKA
    b .
    b .
    ldr pc,=HandlerADC	    ;mGKB
    b .						;
    b .						;
    b .						;
    b .						;
    b .						;mGKB
    b .
    b .
;0xe0=EnterPWDN
    ldr pc,=EnterPWDN

    LTORG	

HandlerFIQ	HANDLER HandleFIQ
HandlerIRQ	HANDLER HandleIRQ
HandlerUndef	HANDLER HandleUndef
HandlerSWI	HANDLER HandleSWI
HandlerDabort	HANDLER HandleDabort
HandlerPabort	HANDLER HandlePabort

HandlerADC	HANDLER HandleADC
HandlerRTC	HANDLER HandleRTC
HandlerUTXD1	HANDLER HandleUTXD1
HandlerUTXD0	HANDLER HandleUTXD0
HandlerSIO	HANDLER HandleSIO
HandlerIIC	HANDLER HandleIIC
HandlerURXD1	HANDLER HandleURXD1
HandlerURXD0	HANDLER HandleURXD0
HandlerTIMER5	HANDLER HandleTIMER5
HandlerTIMER4	HANDLER HandleTIMER4
HandlerTIMER3	HANDLER HandleTIMER3
HandlerTIMER2	HANDLER HandleTIMER2
HandlerTIMER1	HANDLER HandleTIMER1
HandlerTIMER0	HANDLER HandleTIMER0
HandlerUERR01	HANDLER HandleUERR01
HandlerWDT	HANDLER HandleWDT
HandlerBDMA1	HANDLER HandleBDMA1
HandlerBDMA0	HANDLER HandleBDMA0
HandlerZDMA1	HANDLER HandleZDMA1
HandlerZDMA0	HANDLER HandleZDMA0
HandlerTICK	HANDLER HandleTICK
HandlerEINT4567	HANDLER HandleEINT4567
HandlerEINT3	HANDLER HandleEINT3
HandlerEINT2	HANDLER HandleEINT2
HandlerEINT1	HANDLER HandleEINT1
HandlerEINT0	HANDLER HandleEINT0


;****************************************************
;*	START											*
;****************************************************
ResetHandler
    ldr	    r0,=WTCON	    ;watch dog disable 
    ldr	    r1,=0x0 		
    str	    r1,[r0]

    ldr	    r0,=INTMSK
    ldr	    r1,=0x07ffffff  ;all interrupt disable
    str	    r1,[r0]

    ;****************************************************
    ;*	Set clock control registers						*
    ;****************************************************
    ldr	    r0,=CLKSLOW		 
    ldr	    r1,=0x39	    ;All unit block CLK enable	
    str	    r1,[r0]
                       ;pwr on
;    ldr	    r0,=PCONF		 
;    ldr	    r1,=0x1B6D4A 
;    str	    r1,[r0]

 ;   ldr	    r0,=PUPF		 
 ;   ldr	    r1,=0x003 
 ;   str	    r1,[r0]

  ;  ldr	    r0,=PDATF		 
 ;   ldr	    r1,=0xffe7  
  ;  str	    r1,[r0]
      ; cach on
 ;  ldr	    r0,=SYSCFG		 
 ;  ldr	    r1,=0x000E  
 ;  str	    r1,[r0]
        
    ldr	r0,=LOCKTIME
    ldr	r1,=0xfff
    str	r1,[r0]


    ldr	    r0,=CLKSLOW		 
    ldr	    r1,=0x19	    ;All unit block CLK enable	
    str	    r1,[r0]


    ldr	    r0,=CLKCON		 
    ldr	    r1,=0x1fd8	    ;0x7ff8   All unit block CLK enable	
    str	    r1,[r0]


    ;****************************************
    ;*  change BDMACON reset value for BDMA *   
    ;****************************************
    ldr     r0,=BDIDES0       
    ldr     r1,=0x40000000   ;BDIDESn reset value should be 0x40000000	 
    str     r1,[r0]

    ldr     r0,=BDIDES1      
    ldr     r1,=0x40000000   ;BDIDESn reset value should be 0x40000000	 
    str     r1,[r0]

    ;****************************************************
    ;*	Set memory control registers					* 	
    ;****************************************************
    
    ldr     r0,=PDATG
    ldr     r1,[r0]
    tst     r1,#0x80  ; #0x80  ;=int 7( pg.7 ) ;;   #0x10  ;=int 5 (pg.4 )
    
    ldrne   r0,=SMRDATA  ; 1=16 bit  
    ldreq   r0,=SMRDATA1  ; 0=8 bit
    
    ldmia   r0,{r1-r13}
    ldr	    r0,=0x01c80000  ;BWSCON Address
    stmia   r0,{r1-r13}

    ;****************************************************
    ;*	pll con
    ;****************************************************
;	ldr	r0,=PLLCON			;temporary setting of PLL
;	ldr	r1,=((0x2a<<12)+(3<<4)+1)	;Fin=10MHz,Fout=40MHz
;	str	r1,[r0]


    ;****************************************************
    ;*	Initialize stacks								* 
    ;****************************************************
    ldr	    sp, =SVCStack	;Why?
    bl	    InitStacks

    ;****************************************************
    ;*	Setup IRQ handler								*
    ;****************************************************
 ;   ldr	    r0,=HandleIRQ		;This routine is needed
 ;   ldr	    r1,=IsrIRQ			;if there isn't 'subs pc,lr,#4' at 0x18, 0x1c
 ;   str	    r1,[r0]
    ;********************************************************
    ;*	Copy and paste RW data/zero initialized data	    *
    ;********************************************************
    LDR	    r0, =|Image$$RO$$Limit|	; Get pointer to ROM data
    LDR	    r1, =|Image$$RW$$Base|	; and RAM copy
    LDR	    r3, =|Image$$ZI$$Base|	
	;Zero init base => top of initialised data
			
    CMP	    r0, r1	    ; Check that they are different
    BEQ	    %F1
0		
    CMP	    r1, r3	    ; Copy init data
    LDRCC   r2, [r0], #4    ;--> LDRCC r2, [r0] + ADD r0, r0, #4		 
    STRCC   r2, [r1], #4    ;--> STRCC r2, [r1] + ADD r1, r1, #4
    BCC	    %B0
1		
    LDR	    r1, =|Image$$ZI$$Limit| ; Top of zero init segment
    MOV	    r2, #0
2		
    CMP	    r3, r1	    ; Zero init
    STRCC   r2, [r3], #4
    BCC	    %B2

  
    ldr	    r0,=CLKSLOW		 
    ldr	    r1,=0x09	    ;All unit block CLK enable	
    str	    r1,[r0]

    IMPORT Main__Fv    ; The main entry of mon program 


    [ :LNOT:THUMBCODE
	BL		Main__Fv	    ;从汇编进入C语言代码空间，不要使用main()
	B	.						
    ]

    [ THUMBCODE		    ;for start-up code for Thumb mode
	orr     lr,pc,#1
	bx      lr
	CODE16
	bl      Main__Fv	    ;从汇编进入C语言代码空间，不要使用main()
	b       .
	CODE32
    ]

;****************************************************
;*	The function for initializing stack				*
;****************************************************
InitStacks
	;Don't use DRAM,such as stmfd,ldmfd......
	;SVCstack is initialized before
	;Under toolkit ver 2.50, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'

    mrs	    r0,cpsr
    bic	    r0,r0,#MODEMASK
    orr	    r1,r0,#UNDEFMODE|NOINT
    msr     cpsr_cxsf,r1 ; cpsr,r1;        msr	    cpsr_cxsf,r1		;UndefMode
    ldr	    sp,=UndefStack
	
    orr	    r1,r0,#ABORTMODE|NOINT
    msr      cpsr_cxsf,r1 ;cpsr,r1;msr	    cpsr_cxsf,r1 	    	;AbortMode
    ldr	    sp,=AbortStack

    orr	    r1,r0,#IRQMODE|NOINT
    msr      cpsr_cxsf,r1 ;cpsr,r1;//msr	    cpsr_cxsf,r1 	    	;IRQMode
    ldr	    sp,=IRQStack
	
    orr	    r1,r0,#FIQMODE|NOINT
    msr     cpsr_cxsf,r1 ; cpsr,r1;//msr	    cpsr_cxsf,r1 	    	;FIQMode
    ldr	    sp,=FIQStack

    bic	    r0,r0,#MODEMASK|NOINT
    orr	    r1,r0,#SVCMODE
    msr     cpsr_cxsf,r1 ;cpsr,r1;//msr	    cpsr_cxsf,r1 	    	;SVCMode
    ldr	    sp,=SVCStack

	;USER mode is not initialized.
    mov	    pc,lr ;The LR register may be not valid for the mode changes.

;****************************************************
;*	The function for entering power down mode		*
;****************************************************
;void EnterPWDN(int CLKCON);
EnterPWDN
    mov	    r2,r0               ;r0=CLKCON
    ldr	    r0,=REFRESH		
    ldr	    r3,[r0]
    mov	    r1, r3
    orr	    r1, r1, #0x400000   ;self-refresh enable
    str	    r1, [r0]

    nop     ;Wait until self-refresh is issued. May not be needed.
    nop     ;If the other bus master holds the bus, ...
    nop	    ; mov r0, r0
    nop
    nop
    nop
    nop

;enter POWERDN mode
    ldr	    r0,=CLKCON
    str	    r2,[r0]

;wait until enter SL_IDLE,STOP mode and until wake-up
    ldr	    r0,=0x10
0   subs    r0,r0,#1
    bne	    %B0

;exit from DRAM/SDRAM self refresh mode.
    ldr	    r0,=REFRESH
    str	    r3,[r0]
    mov	    pc,lr

    LTORG

SMRDATA DATA
;  sdram  16 bits
;*****************************************************************
;* Memory configuration has to be optimized for best performance *
;* The following parameter is not optimized.                     *
;*****************************************************************

;*** memory access cycle parameter strategy ***
; 1) Even FP-DRAM, EDO setting has more late fetch point by half-clock
; 2) The memory settings,here, are made the safe parameters even at 66Mhz.
; 3) FP-DRAM Parameters:tRCD=3 for tRAC, tcas=2 for pad delay, tcp=2 for bus load.
; 4) DRAM refresh rate is for 40Mhz. 

	DCD 0x11111102	;Bank0=OM[1:0], Bank1~Bank7=8bit
	DCD ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))	;GCS0
	DCD ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))	;GCS1 
	DCD ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))	;GCS2
	DCD ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))	;GCS3
	DCD ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))	;GCS4
	DCD ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))	;GCS5
	DCD 0x00018001; ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))	;GCS6
	DCD 0x00018001; ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))	;GCS7
	DCD 0x00AC0951; ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)	;REFRESH RFEN=1, TREFMD=0, trp=3clk, trc=5clk, tchr=3clk,count=1019
	DCD 0x17 ; 0x10			;SCLK power down mode, BANKSIZE 32M/32M
	DCD 0x20 ; 0x20			;MRSR6 CL=2clk
	DCD 0x20 ; 0x20			;MRSR7

SMRDATA1 DATA
; sdram  8 bits 
	DCD 0x10111102	;Bank0=OM[1:0], Bank1~Bank7=8bit
	DCD ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))	;GCS0
	DCD ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))	;GCS1 
	DCD ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))	;GCS2
	DCD ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))	;GCS3
	DCD ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))	;GCS4
	DCD ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))	;GCS5
	DCD 0x00018002; ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))	;GCS6
	DCD 0x00018001; ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))	;GCS7
	DCD 0x00AC0951; ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)	;REFRESH RFEN=1, TREFMD=0, trp=3clk, trc=5clk, tchr=3clk,count=1019
	DCD 0x17 ; 0x10			;SCLK power down mode, BANKSIZE 32M/32M
	DCD 0x20 ; 0x20			;MRSR6 CL=2clk
	DCD 0x20 ; 0x20			;MRSR7

	ALIGN


	AREA RamData, DATA, READWRITE

	^	(_ISR_STARTADDRESS-0xa00)
				
UserStack	#	512	;c1(c7)ffa00
SVCStack	#	512	;c1(c7)ffb00
UndefStack	#	512	;c1(c7)ffc00
AbortStack	#	512	;c1(c7)ffd00
IRQStack	#	512	;c1(c7)ffe00
FIQStack	#	0	;c1(c7)fff00


		^	_ISR_STARTADDRESS
HandleReset	#	4
HandleUndef	#	4
HandleSWI	#	4
HandlePabort	#	4
HandleDabort	#	4
HandleReserved	#	4
HandleIRQ	#	4
HandleFIQ	#	4

;Don't use the label 'IntVectorTable',
;because armasm.exe cann't recognize this label correctly.
;the value is different with an address you think it may be.
;IntVectorTable
HandleADC	#	4
HandleRTC	#	4
HandleUTXD1	#	4
HandleUTXD0	#	4
HandleSIO	#	4
HandleIIC	#	4
HandleURXD1	#	4
HandleURXD0	#	4
HandleTIMER5	#	4
HandleTIMER4	#	4
HandleTIMER3	#	4
HandleTIMER2	#	4
HandleTIMER1	#	4
HandleTIMER0	#	4
HandleUERR01	#	4
HandleWDT	#	4
HandleBDMA1	#	4
HandleBDMA0	#	4
HandleZDMA1	#	4
HandleZDMA0	#	4
HandleTICK	#	4
HandleEINT4567	#	4
HandleEINT3	#	4
HandleEINT2	#	4
HandleEINT1	#	4
HandleEINT0	#	4   ;0xc1(c7)fff84

		END
