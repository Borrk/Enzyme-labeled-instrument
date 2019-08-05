; ***********************************************
; * NAME    : NAND FLASH Subroutine		*
; * Version : 23.JUL.1998			*
; ***********************************************


	AREA |C$$code|, CODE, READONLY

	EXPORT	__WrPage528
__WrPage528
	;input:a1(r0)=pPage
	stmfd	sp!,{r1-r11}
	; Hyq modify to 2000000 
	;mov a2,#0x6000000
	mov a2,#0x2000000
	mov	a3,#0x200
0	ldmia	a1!,{r4-r11}
	stmia	a2,{r4-r11}
	subs	a3,a3,#32
	bne	%B0
	ldmia	a1!,{r4-r7}
	stmia	a2,{r4-r7}

	ldmfd	sp!,{r1-r11}
	mov	pc,lr




	EXPORT	__RdPage528
__RdPage528
	;input:a1(r0)=pPage
	stmfd	sp!,{r1-r11}
	; Hyq modify to 2000000 
	;mov a2,#0x6000000
	mov a2,#0x2000000
	mov	a3,#0x200
0	ldmia	a2,{r4-r11}
	stmia	a1!,{r4-r11}
	subs	a3,a3,#32
	bne	%B0
	ldmia	a2,{r4-r7}
	stmia	a1!,{r4-r7}

	ldmfd	sp!,{r1-r11}
	mov	pc,lr

	END
