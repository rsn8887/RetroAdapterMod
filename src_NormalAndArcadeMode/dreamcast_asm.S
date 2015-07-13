#define __SFR_ASM_COMPAT	1
#define __SFR_OFFSET		0

#include <avr/io.h>

;	B5	-	data 0
;	B4	-	data 1
;	B3	-	sense

#define	DC0		5
#define	DC1		4
#define	DCS		3

#define debug

#define	temp0	R16
#define	temp1	R17
;#define	shadow	R18

.extern	dcbuffer

; Timing macro
		.MACRO __DELAY loops
		ldi		timer, \loops
__DELAY_LOOP_\@:
		dec		timer
		brne	__DELAY_LOOP_\@
		.ENDM

; main function
.global dreamcast

dreamcast:
		push	temp0
		push	temp1
		;push	shadow
		push	ZH
		push	ZL
		push	XH
		push	XL

		mov		ZH, r25				; pointer to reportBuffer[]
		mov		ZL, r24

		; prepare command
		ldi		temp0, 0x09			; get condition
		sts		dcbuffer+0, temp0
		ldi		temp0, 0x20			; controller address
		sts		dcbuffer+1, temp0
		ldi		temp0, 0x00			; Retro Adapter (Dreamcast) address
		sts		dcbuffer+2, temp0
		ldi		temp0, 0x01			; 1 additional word to follow
		sts		dcbuffer+3, temp0
		
		ldi		temp0, 0x01			; controller function code
		sts		dcbuffer+4, temp0
		ldi		temp0, 0x00			; upper function code bits and null data
		sts		dcbuffer+5, temp0
		sts		dcbuffer+6, temp0
		sts		dcbuffer+7, temp0

		ldi		temp0, 0x09^0x20^0x01^0x01	; checksum
		sts		dcbuffer+8, temp0

		; send sync
		rcall	dc_write

		pop		XH
		pop		XL
		pop		ZL
		pop		ZH
		;pop		shadow
		pop		temp1
		pop		temp0

		ret

; write 9 bytes in dcbuffer to Maple bus
dc_write:
		ldi		XL, hi8(dcbuffer)
		ldi		XH, lo8(dcbuffer)

		ldi		temp0, 9			; byte counter
		;in		shadow, PORTB		; port shadow

; produce the protocol sync mark
		cbi		PORTB, DC0			; 0
		; 292ns
		nop
		nop
		nop
		cbi		PORTB, DC1			; 1
		; 375ns
		nop
		nop
		nop
		sbi		PORTB, DC1			; 2
		; 292ns
		nop
		nop
		cbi		PORTB, DC1			; 3
		; 333ns
		nop
		nop
		nop
		sbi		PORTB, DC1			; 4
		; 292ns
		nop
		nop
		nop
		cbi		PORTB, DC1			; 5
		; 333ns
		nop
		nop
		nop
		sbi		PORTB, DC1			; 6
		; 292ns
		nop
		nop
		nop
		cbi		PORTB, DC1			; 7
		; 333ns
		nop
		nop
		nop
		sbi		PORTB, DC1			; 8
		; 292ns
		nop
		nop
		nop
		nop
		sbi		PORTB, DC0

; write 8 bytes
dc_write_loop:		
		ld		temp1, X+

		; bit 7 on DC1
		sbrc	temp1, 7
		sbi		PORTB, DC1
		sbrs	temp1, 7
		cbi		PORTB, DC1
		cbi		PORTB, DC0			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC1

		; bit 6 on DC0
		sbrc	temp1, 6
		sbi		PORTB, DC0
		sbrs	temp1, 6
		cbi		PORTB, DC0
		sbi		PORTB, DC1			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC0

		; bit 5 on DC1
		sbrc	temp1, 5
		sbi		PORTB, DC1
		sbrs	temp1, 5
		cbi		PORTB, DC1
		sbi		PORTB, DC0			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC1

		; bit 4 on DC0
		sbrc	temp1, 4
		sbi		PORTB, DC0
		sbrs	temp1, 4
		cbi		PORTB, DC0
		sbi		PORTB, DC1			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC0

		; bit 3 on DC1
		sbrc	temp1, 5
		sbi		PORTB, DC1
		sbrs	temp1, 5
		cbi		PORTB, DC1
		sbi		PORTB, DC0			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC1

		; bit 2 on DC0
		sbrc	temp1, 4
		sbi		PORTB, DC0
		sbrs	temp1, 4
		cbi		PORTB, DC0
		sbi		PORTB, DC1			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC0

		; bit 1 on DC1
		sbrc	temp1, 5
		sbi		PORTB, DC1
		sbrs	temp1, 5
		cbi		PORTB, DC1
		sbi		PORTB, DC0			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC1

		; bit 0 on DC0
		sbrc	temp1, 4
		sbi		PORTB, DC0
		sbrs	temp1, 4
		cbi		PORTB, DC0
		sbi		PORTB, DC1			; clock bit out
		nop							; >250ns
		nop
		sbi		PORTB, DC0

		dec		temp0
		;brne	dc_write_loop
		
		sbi		PORTB, DC0
		sbi		PORTB, DC1
		ret

