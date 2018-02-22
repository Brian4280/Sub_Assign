;
; DA_1.asm
;
; Created: 2/20/2018 7:44:51 PM
; Author : Lopez
;


; Replace with your application code

	LDI ZL, 0x22
	LDI ZH, 0x02
	LDI R16, 0x02
	LDI R17, 0x22
	LDI R20, 0          ; counter for number of locations stored

storeLp1:
	MOV R18, R16
	ADD R18, R17        ; getting HIGH(STARTADD) + LOW(STARTADD)
	ST Z+, R18          ; storing the results and going to next location
	INC R20 
	CPI R20, 0        ; once count has reached its limit, end current loop
	BREQ Lp1Done
	INC R17             ; increase address number
	CPI R17, 0x00       ; when lower address is 00 (after FF), increment upper half
	BRNE storeLp1
	LDI R17, 0x00       ; and set lower half back to zero
	INC R16
	RJMP storeLp1

Lp1Done:
	LDI R20, 0
storeLp2:               ; exactly the same as first loop, done only 45 times to make
	MOV R18, R16        ; up for the 300 needed.
	ADD R18, R17
	ST Z+, R18
	INC R20
	CPI R20, 46
	BREQ Lp2Done
	INC R17
	CPI R17, 0x00
	BRNE storeLp2
	LDI R17, 0x00
	INC R16
	RJMP storeLp2

Lp2Done:                ; done storing values into STARTADD to STARTADD+300
	LDI R20, 0          ; used for a counter
	LDI R21, 0          ; will contain the current value of Z's pointer
	LDI ZL, 0x22        ; parse through values that were just stored
	LDI ZH, 0x02
	LDI XL, 0x00        ; will store values that are divisible by 5
	LDI XH, 0x04
	LDI YL, 0x00        ; will store everything else
	LDI YH, 0x06
	CLR R16
	CLR R17
	CLR R18
	CLR R19

GoThruMem1:
	LD R21, Z+
	MOV R22, R21
div1:
	SUBI R21, 5         ; will continuously subtract 5 until it either reaches 0 or
	BRMI notdiv1        ; a negative number.
	CPI R21, 0          ; if it reaches 0, then it is divisible by 5
	BREQ isdiv1         ; if it gets to a negative number, then it is not divisible by 5
	RJMP div1
isdiv1:
	ST X+, R22
	ADD R16, R22        ; add num divisible by 5 to r17:r16. First add number to lower 
	ADC R17, R0         ; half, then add the upper half with the carry bit if used
	RJMP divDone1
notdiv1:
	ST Y+, R22
	ADD R18, R22        ; add every other number to r18:r19 word
	ADC R19, R0
divDone1:
	INC R20
	CPI R20, 0        ; checks if r20 is 0 since that is when R20 reaches its max value and resets
	BREQ mem1LpDone
	RJMP GoThruMem1

mem1LpDone:
	CLR R20
GoThruMem2:
	LD R21, Z+
	MOV R22, R21
div2:
	SUBI R21, 5         ; will continuously subtract 5 until it either reaches 0 or
	BRMI notdiv2        ; a negative number.
	CPI R21, 0          ; if it reaches 0, then it is divisible by 5
	BREQ isdiv2         ; if it gets to a negative number, then it is not divisible by 5
	RJMP div2
isdiv2:
	ST X+, R22
	ADD R16, R22        ; add num divisible by 5 to r17:r16. First add number to lower 
	ADC R17, R0         ; half, then add the upper half with the carry bit if used
	RJMP divDone2
notdiv2:
	ST Y+, R22
	ADD R18, R22        ; add every other number to r18:r19 word
	ADC R19, R0
divDone2:
	INC R20
	CPI R20, 46        ; checks if r20 is 46 since that will be when it has gone through
	BREQ mem2LpDone     ; 300 numbers
	RJMP GoThruMem2

mem2LpDone:
	CLR R22

	

