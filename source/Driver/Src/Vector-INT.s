	INCLUDE	Inc\option.inc
	INCLUDE	Inc\memcfg.inc

;****************************************************************************
;存储器空间
;GCS6 64M 16bit(8MB) DRAM/SDRAM(0xc000000-0xc7fffff)
;APP    RAM=0xc000000~0xc7effff 
;44BMON RAM=0xc7f0000-0xc7fffff
;STACK	   =0xc7ffa00		   

;****************************************************************************
;中断控制预定义
INTCON		EQU	0x01e00000
INTPND	    EQU	0x01e00004
INTMOD	    EQU	0x01e00008
INTMSK	    EQU	0x01e0000c
I_ISPR	    EQU	0x01e00020
I_CMST	    EQU	0x01e0001c
I_ISPC	EQU	0x01e00024			

;****************************************************************************
;看门狗定时器预定义
WTCON	    EQU	0x01d30000

;****************************************************************************
;系统时钟预定义
PLLCON	    EQU	0x01d80000
CLKCON	    EQU	0x01d80004
LOCKTIME    EQU	0x01d8000c
	
;****************************************************************************
;存储器控制预定义
REFRESH	    EQU 0x01c80024

;****************************************************************************
;BDMA目的寄存器
BDIDES0     EQU 0x1f80008
BDIDES1     EQU 0x1f80028

;****************************************************************************
;预定义常数（常量）
USERMODE    EQU	0x10
FIQMODE	    EQU	0x11
IRQMODE	    EQU	0x12
SVCMODE	    EQU	0x13
ABORTMODE   EQU	0x17
UNDEFMODE   EQU	0x1b
MODEMASK    EQU	0x1f
NOINT	    EQU	0xc0

ModeMask 		EQU	0x1F
SVC32Mode		EQU	0x13
IRQ32Mode		EQU	0x12
FIQ32Mode		EQU	0x11
User32Mode		EQU	0x10
Abort32Mode		EQU	0x17
Undef32Mode		EQU	0x1B
IRQ_BIT			EQU	0x80
FIQ_BIT			EQU	0x40


;**********************************************************
;检查是否使用tasm.exe进行编译
	
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
    ldr	    r0,=$HandleLabel ;load the address of HandleXXX to r0
    ldr	    r0,[r0]	    	;load the contents(service routine start address) of HandleXXX
    str	    r0,[sp,#4]	    ;store the contents(ISR) of HandleXXX to stack
    ldmfd   sp!,{r0,pc}	    ;POP the work register and pc(jump to ISR)
    MEND

;******************************************************
	AREA	SelfBoot,	CODE,	READONLY
	
	IMPORT  Main__Fv
	ENTRY	
		
ResetEntry
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
    b .					;mGKA
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


;****************************************************
;*	START											*
;****************************************************
;初始化程序开始
ResetHandler
	mrs	r0, cpsr				;enter svc mode and disable irq,fiq
	bic	r0, r0, #ModeMask
	orr	r0, r0, #(SVC32Mode :OR: IRQ_BIT :OR: FIQ_BIT)
	msr	cpsr_c, r0

	bl	SysInit	
	
	adr	r0, ResetEntry
	ldr	r1,	BaseOfROM
	cmp	r0,	r1
	ldreq	r0, TopOfROM
	beq	InitRamData
			
	ldr	r2,	=CopyProcBeg
	sub	r1, r2, r1
	add	r0, r0, r1	
	ldr	r3,	=CopyProcEnd	
0	
	ldmia	r0!, {r4-r7}
	stmia	r2!, {r4-r7}
	cmp	r2, r3
	bcc	%B0	
	
	ldr	r3, TopOfROM		
	ldr	pc, =CopyProcBeg
	
;***********************************************
CopyProcBeg	
0	
	ldmia	r0!, {r4-r11}
	stmia	r2!, {r4-r11}
	cmp	r2, r3
	bcc	%B0	
CopyProcEnd
	
	sub	r1, r2, r3
	sub	r0, r0, r1		
	
InitRamData	
	ldr	r2, BaseOfBSS
	ldr	r3, BaseOfZero	
0
	cmp	r2, r3
	ldrcc	r1, [r0], #4
	strcc	r1, [r2], #4
	bcc	%B0	

	mov	r0,	#0
	ldr	r3,	EndOfBSS
1	
	cmp	r2,	r3
	strcc	r0, [r2], #4
	bcc	%B1			


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

SysInit	
;禁止看门狗	
	ldr	r0, =WTCON			
	ldr	r1, =0 		
	str	r1,[r0]
;禁止所有中断
	ldr	    r0,=INTMSK
	ldr	    r1,=0x07ffffff		
	str	    r1,[r0]

;设定时钟控制寄存器
	ldr	r0, =LOCKTIME
	ldr	r1, =0xfff
	str	r1, [r0]

    ;[ PLLONSTART
	ldr	r0, =PLLCON			;锁相环倍频设定
	ldr	r1, =((M_DIV<<12)+(P_DIV<<4)+S_DIV)		;设定系统主时钟频率, 倍频为((P_DIV+2)*(2的S_DIV次方))/(M_DIV+8)
	str	r1, [r0]
    ;]

	ldr	r0, =CLKCON		 
	ldr	r1, =0x7ff8	    ;所有功能单元块时钟使能
	str	r1, [r0]
;****************************************************************************
;为BDMA改变BDMACON的复位值
	ldr	r0, =BDIDES0       
	ldr	r1, =0x40000000   ;BDIDESn reset value should be 0x40000000	 
	str	r1, [r0]

	ldr	r0, =BDIDES1      
	ldr	r1, =0x40000000   ;BDIDESn reset value should be 0x40000000	 
	str	r1, [r0]
;****************************************************
;设定存储器控制寄存器			
	adr	r0, SysInit
	ldr	r1, =SysInit
	sub	r0, r1, r0		
	ldr	r1, =SMRDATA
	sub	r0, r1, r0 
	ldmia   r0, {r1-r13}
	ldr	    r0, =0x01c80000			;BWSCON Address
	stmia   r0, {r1-r13}	

;****************************************************	
;初始化堆栈
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

    mov	  pc, lr			;返回	    	

;***********************************************

	IMPORT	|Image$$RO$$Base|	; ROM code start	
	IMPORT	|Image$$RO$$Limit|	; RAM data starts after ROM program
	IMPORT	|Image$$RW$$Base|	; Pre-initialised variables
	IMPORT	|Image$$ZI$$Base|	; uninitialised variables
	IMPORT	|Image$$ZI$$Limit|	; End of variable RAM space

BaseOfROM	DCD	|Image$$RO$$Base|
TopOfROM	DCD	|Image$$RO$$Limit|
BaseOfBSS	DCD	|Image$$RW$$Base|
BaseOfZero	DCD	|Image$$ZI$$Base|
EndOfBSS	DCD	|Image$$ZI$$Limit|

	EXPORT	GetBaseOfROM
	EXPORT	GetEndOfROM
	EXPORT	GetBaseOfBSS
	EXPORT	GetBaseOfZero
	EXPORT	GetEndOfBSS
	
GetBaseOfROM
	ldr	r0, BaseOfROM
	mov	pc, lr	
GetEndOfROM
	ldr	r0, TopOfROM
	mov	pc,	lr
GetBaseOfBSS
	ldr	r0,	BaseOfBSS
	mov	pc,	lr
GetBaseOfZero
	ldr	r0,	BaseOfZero
	mov	pc,	lr
GetEndOfBSS
	ldr	r0,	EndOfBSS
	mov	pc,	lr

HandlerFIQ		HANDLER HandleFIQ
HandlerIRQ		HANDLER HandleIRQ
HandlerUndef	HANDLER HandleUndef
HandlerSWI		HANDLER HandleSWI
HandlerDabort	HANDLER HandleDabort
HandlerPabort	HANDLER HandlePabort

HandlerADC		HANDLER HandleADC
HandlerRTC		HANDLER HandleRTC
HandlerUTXD1	HANDLER HandleUTXD1
HandlerUTXD0	HANDLER HandleUTXD0
HandlerSIO		HANDLER HandleSIO
HandlerIIC		HANDLER HandleIIC
HandlerURXD1	HANDLER HandleURXD1
HandlerURXD0	HANDLER HandleURXD0
HandlerTIMER5	HANDLER HandleTIMER5
HandlerTIMER4	HANDLER HandleTIMER4
HandlerTIMER3	HANDLER HandleTIMER3
HandlerTIMER2	HANDLER HandleTIMER2
HandlerTIMER1	HANDLER HandleTIMER1
HandlerTIMER0	HANDLER HandleTIMER0
HandlerUERR01	HANDLER HandleUERR01
HandlerWDT		HANDLER HandleWDT
HandlerBDMA1	HANDLER HandleBDMA1
HandlerBDMA0	HANDLER HandleBDMA0
HandlerZDMA1	HANDLER HandleZDMA1
HandlerZDMA0	HANDLER HandleZDMA0
HandlerTICK		HANDLER HandleTICK
HandlerEINT4567	HANDLER HandleEINT4567
HandlerEINT3	HANDLER HandleEINT3
HandlerEINT2	HANDLER HandleEINT2
HandlerEINT1	HANDLER HandleEINT1
HandlerEINT0	HANDLER HandleEINT0

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
;本函数用来进入掉电模式
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


;*****************************************************************	
SMRDATA DATA
;*****************************************************************
;存储器最好配置成最优的性能,下面的参数不是最优化的
;*****************************************************************

;*** memory access cycle parameter strategy ***
; 1) Even FP-DRAM, EDO setting has more late fetch point by half-clock
; 2) The memory settings,here, are made the safe parameters even at 66Mhz.
; 3) FP-DRAM Parameters:tRCD=3 for tRAC, tcas=2 for pad delay, tcp=2 for bus load.
; 4) DRAM refresh rate is for 40Mhz. 

;bank0	16bit BOOT ROM
;bank1	8bit NandFlash
;bank2	16bit IDE
;bank3	8bit UDB
;bank4	rtl8019
;bank5	ext
;bank6	16bit SDRAM
;bank7	16bit SDRAM
    
    [ BUSWIDTH=16			
	DCD 0x11110101	;Bank0=16bit BootRom(AT29C010A*2) :0x0
    | ;BUSWIDTH=32
	DCD 0x22222220	;Bank0=OM[1:0], Bank1~Bank7=32bit
    ]
	
	DCD 	((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))	;GCS0
	DCD 	((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))	;GCS1 
	DCD 	((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))	;GCS2
	DCD 	((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))	;GCS3
	DCD 	((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))	;GCS4
	DCD 	((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))	;GCS5
	
	[ BDRAMTYPE="DRAM" 
	DCD 	((B6_MT<<15)+(B6_Trcd<<4)+(B6_Tcas<<3)+(B6_Tcp<<2)+(B6_CAN))	;GCS6 check the MT value in parameter.a
	DCD 	((B7_MT<<15)+(B7_Trcd<<4)+(B7_Tcas<<3)+(B7_Tcp<<2)+(B7_CAN))	;GCS7
	| ;"SDRAM"
	DCD 	((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))	;GCS6
	DCD 	((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))	;GCS7
	]
	
	DCD 	((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Trc<<18)+(Tchr<<16)+REFCNT)	;REFRESH RFEN=1, TREFMD=0, trp=3clk, trc=5clk, tchr=3clk,count=1019
	DCD 	0x10			;SCLK power down mode, BANKSIZE 32M/32M
	DCD 	0x20			;MRSR6 CL=2clk
	DCD 	0x20			;MRSR7

	ALIGN	

	AREA RamData, DATA, READWRITE	

	^	(_ISR_STARTADDRESS-0x500)				
UserStack	#	2048	;c1(c7)ffa00
SVCStack	#	1024	;c1(c7)ffb00
UndefStack	#	1024	;c1(c7)ffc00
AbortStack	#	1024	;c1(c7)ffd00
IRQStack	#	1024	;c1(c7)ffe00
FIQStack	#	1024	;c1(c7)fff00


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
			
HandleADC		#	4
HandleRTC		#	4
HandleUTXD1	#	4
HandleUTXD0	#	4
HandleSIO		#	4
HandleIIC		#	4
HandleURXD1	#	4
HandleURXD0	#	4
HandleTIMER5	#	4
HandleTIMER4	#	4
HandleTIMER3	#	4
HandleTIMER2	#	4
HandleTIMER1	#	4
HandleTIMER0	#	4
HandleUERR01	#	4
HandleWDT		#	4
HandleBDMA1	#	4
HandleBDMA0	#	4
HandleZDMA1	#	4
HandleZDMA0	#	4
HandleTICK		#	4
HandleEINT4567	#	4
HandleEINT3		#	4
HandleEINT2		#	4
HandleEINT1		#	4
HandleEINT0		#	4   ;0xc1(c7)fff84 	


	END		
		