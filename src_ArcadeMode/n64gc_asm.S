#define __SFR_ASM_COMPAT	1
#define __SFR_OFFSET		0

#include <avr/io.h>

;	PB0		-	data

#define	data1	r18
#define	data2	r19
#define	data3	r20
#define	data4	r21
#define	data5	r22
#define data6	r23
#define data7	r24
#define	data8	r25

#define	b1		r20
#define b2		r21

#define	count	r26
#define	timer	r27

#define	MSD3	13
#define	MSD2	9
#define	MSD1	3

#define debug

; Timing macro
		.MACRO __DELAY loops
		ldi		timer, \loops
__DELAY_LOOP_\@:
		dec		timer
		brne	__DELAY_LOOP_\@
		.ENDM

; main function
.global readnintendo

readnintendo:
		mov		ZH, r25		; pointer to reportBuffer[]
		mov		ZL, r24
		ldi		data1, 1	; add 1 to skip report ID
		ldi		data2, 0
		add		ZL, data1
		adc		ZH, data2

		clr		data1
		clr		data2
		clr		data3
		clr		data4
		clr		data5
		clr		data6
		clr		data7
		clr		data8

		; send 0x00 to identify controller
		ldi		data8, 0x00
		rcall	writebyte
		rcall	writestopbit
		
		; read three bytes for id
		rcall	readbyte
		mov		data1, data8
		rcall	readbyte
		mov		data2, data8
		rcall	readbyte
		mov		data3, data8

		cpi		data1, 0x05		; N64 pad
		breq	readn64

		cpi		data1, 0x09		; GameCube pad
		breq	readgc

		ldi		r24, 0			; return
		ret

; --- read N64 ------------------------------------------------------------------------------------

readn64:
		; wait for controller to be ready again
		__DELAY 255
		__DELAY 200

		; send 0x01 to read controller status words
		ldi		data8, 0x01
		rcall	writebyte
		rcall	writestopbit
		
		; read three bytes for id
		rcall	readbyte
		mov		data1, data8
		rcall	readbyte
		mov		data2, data8
		rcall	readbyte
		mov		data3, data8
		rcall	readbyte
		mov		data4, data8

		st		Z, data3		; x
		neg		data4			; for some reason y is inverted
		std		Z+1, data4		; y

		; data3 - b1
		; data4 - b2
		ldi		data3, 0
		ldi		data4, 0

		; map:	0 - A
		;		1 - B
		;		2 - C-Up
		;		3 - C-Down
		;		4 - C-Left
		;		5 - C-Right
		;		6 - L trigger
		;		7 - R trigger
		;		0 - Z
		;		1 - Start

		; 1:	0 - Right
		;		1 - Left
		;		2 - Down
		;		3 - Up
		;		4 - Start
		;		5 - Z
		;		6 - B
		;		7 - A

		; d-pad to hat switch is decoding done by n64gc.c, returned in r24
		sbrc	data1, 7		; A
		ori		b1, (1<<0)
		sbrc	data1, 6		; B
		ori		b1, (1<<1)
		sbrc	data1, 5		; Z
		ori		b2, (1<<0)
		sbrc	data1, 4		; Start
		ori		b2, (1<<1)
		andi	data1, 0x0f		; d-pad
		
		; 2:	0 - C-Right
		;		1 - C-Left
		;		2 - C-Down
		;		3 - C-Up
		;		4 - R
		;		5 - L

		; C buttons mapped as buttons because you can press opposite directions at the
		; same time so they are not suitable for mapping to an axis/hat.
		sbrc	data2, 5		; L
		ori		b1, (1<<6)
		sbrc	data2, 4		; R
		ori		b1, (1<<7)
		sbrc	data2, 3		; C-Up
		ori		b1, (1<<2)
		sbrc	data2, 2		; C-Down
		ori		b1, (1<<3)
		sbrc	data2, 1		; C-Left
		ori		b1, (1<<4)
		sbrc	data2, 0		; C-Right
		ori		b1, (1<<5)

		std		Z+5, data3		; b1
		std		Z+6, data4		; b2

		mov		r24, data1		; return d-pad
		ret

; --- read GameCube -------------------------------------------------------------------------------

readgc:
		; wait for controller to be ready again
		__DELAY 100

		; send 0x400302 to read controller status words
		ldi		data8, 0x40
		rcall	writebyte
		ldi		data8, 0x03
		rcall	writebyte
		ldi		data8, 0x02
		rcall	writebyte
		rcall	writestopbit
		
		; read three bytes for id
		rcall	readbyte
		mov		data1, data8
		rcall	readbyte
		mov		data2, data8
		rcall	readbyte
		mov		data3, data8
		rcall	readbyte
		mov		data4, data8
		rcall	readbyte
		mov		data5, data8
		rcall	readbyte
		mov		data6, data8
		rcall	readbyte
		mov		data7, data8
		rcall	readbyte

		subi	data3, 128
		subi	data4, 128
		neg		data4
		st		Z, data3		; x
		std		Z+1, data4		; y

		subi	data5, 128
		neg		data5
		subi	data6, 128
		neg		data6
		std		Z+2, data5
		std		Z+3, data6

		; data3 - b1
		; data4 - b2
		ldi		data3, 0
		ldi		data4, 0

		; data1:	0 - A
		;			1 - B
		;			2 - X
		;			3 - Y
		;			4 - Start

		sbrc	data1, 0		; A 
		ori		b1, (1<<0)
		sbrc	data1, 1		; B 
		ori		b1, (1<<1)
		sbrc	data1, 2		; X 
		ori		b1, (1<<3)
		sbrc	data1, 3		; Y 
		ori		b1, (1<<2)
		sbrc	data1, 4		; Start 
		ori		b2, (1<<1)

		; data2:	4 - Z
		;			5 - R
		;			6 - L

		sbrc	data2, 4		; Z
		ori		b2, (1<<0)
		sbrc	data2, 5		; R
		ori		b1, (1<<7)
		sbrc	data2, 6		; L
		ori		b1, (1<<6)

		; data7:	L trigger
		cpi		data7, 64
		brlo	gc_no_l
		ori		b1, (1<<6)
gc_no_l:

		; data8:	R trigger
		cpi		data8, 64
		brlo	gc_no_r
		ori		b1, (1<<7)
gc_no_r:
	
		std		Z+5, data3		; b1
		std		Z+6, data4		; b2

		andi	data2, 0x0f		; d-pad
		mov		data1, data2	; swap left/right to match N64
		andi	data2, ~(1<<0)
		sbrc	data2, 1
		ori		data2, (1<<0)
		andi	data2, ~(1<<1)
		sbrc	data1, 0
		ori		data2, (1<<1)
		mov		r24, data2		; return d-pad
		ret

; --- read byte -----------------------------------------------------------------------------------
; byte in data8, timer and count trashed
readbyte:
		ldi		data8, 0
		ldi		timer, 20
		ldi		count, 8
in_byte:
		dec		timer
		breq	timeout
		sbic	PINB, 0
		rjmp	in_byte
		__DELAY	MSD2			;~2us
		lsl		data8
		sbic	PINB, 0
		ori		data8, 0b00000001
		;sbi		PORTD, 7	// debug
		;cbi		PORTD, 7	// debug

		;nop
		;nop
		;nop
		;sbrc	data8, 0
		;sbi		PORTD, 7	// debug
		;cbi		PORTD, 7	// debug

		ldi		timer, 20
in_wait_high:
		dec		timer
		breq	timeout
		sbis	PINB, 0
		rjmp	in_wait_high
		dec		count
		brne	in_byte

timeout:
		ret

; --- write byte ----------------------------------------------------------------------------------
; byte in data8, data8 and count trashed
writebyte:
		ldi		count, 8

out_loop:
		sbrc	data8, 7
		rjmp	out_1
		
		; send a 0
		sbi		DDRB, 0
		__DELAY MSD3
		nop
		cbi		DDRB, 0
		__DELAY MSD1

		;nop
		;nop
		;sbi		PORTD, 7	// debug
		;cbi		PORTD, 7	// debug

		lsl		data8
		dec		count
		brne	out_loop

		ret

out_1:
		; send a 1
		sbi		DDRB, 0
		__DELAY MSD1
		nop
		nop
		cbi		DDRB, 0
		__DELAY	MSD3
		;nop
		lsl		data8
		dec		count
		brne	out_loop

		ret

writestopbit:
		; send stop bit
		;__DELAY	MSD2
		;sbi		PORTD, 7	// debug
		;cbi		PORTD, 7	// debug
		sbi		DDRB, 0
		__DELAY MSD1
		;nop
		nop
		nop
		;sbi		PORTD, 7	// debug
		;cbi		PORTD, 7	// debug
		cbi		DDRB, 0

		ret

