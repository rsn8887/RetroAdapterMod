#define __SFR_ASM_COMPAT	1
#define __SFR_OFFSET		0

#include <avr/io.h>

;	B5	-	Y1		PCINT5
;	B3	-	Y2		PCINT3
;	B4	-	X1		PCINT4
;	B2	-	X2		PCINT2

#define	temp0	r18
#define	temp1	r19
#define	temp2	r20
#define	temp3	r21
#define	sregsav	r22

; X1/X2 and Y1/Y2 reversed for Amiga mice
#define	X1		(1<<4)
#define	X2		(1<<2)
#define	Y1		(1<<5)
#define	Y2		(1<<3)

.extern	xdelta
.extern	ydelta
.extern	pdstate
.extern	bounce1

.global PCINT0_vect

PCINT0_vect:
		sei								; required for V-USB

		push	sregsav
		in		sregsav, SREG

		push	temp0
		push	temp1
		push	temp2
		push	temp3

		; read X/Y lines
		in		temp3, PINB
		andi	temp3, X1|X2|Y1|Y2

/* ----- X axis decoder section ---------------------------------------------------------------- */
		mov		temp2, temp3
		andi	temp2, X1|X2
		lds		temp1, pdstate
		andi	temp1, X1|X2
		lds		temp0, xdelta

		; Gray code decoder X
		cpi		temp2, 0
		breq	gray_x00
		cpi		temp2, X1
		breq	gray_x01
		cpi		temp2, X1|X2
		breq	gray_x11
gray_x10:
		cpi		temp1, 0
		breq	gray_dec_x
		cpi		temp1, X1|X2
		breq	gray_inc_x
		rjmp	gray_x_done
gray_x00:
		cpi		temp1, X1
		breq	gray_dec_x
		cpi		temp1, X2
		breq	gray_inc_x
		rjmp	gray_x_done
gray_x01:
		cpi		temp1, X1|X2
		breq	gray_dec_x
		cpi		temp1, 0
		breq	gray_inc_x
		rjmp	gray_x_done
gray_x11:
		cpi		temp1, X2
		breq	gray_dec_x
		cpi		temp1, X1
		breq	gray_inc_x
		rjmp	gray_x_done

gray_inc_x:
		inc		temp0
		rjmp	gray_x_done
gray_dec_x:
		dec		temp0

gray_x_done:
		sts		xdelta, temp0


/* ----- Y axis decoder section ---------------------------------------------------------------- */
		mov		temp2, temp3
		andi	temp2, Y1|Y2
		lds		temp1, pdstate
		andi	temp1, Y1|Y2
		lds		temp0, ydelta

		; Gray code decoder Y
		cpi		temp2, 0
		breq	gray_y00
		cpi		temp2, Y1
		breq	gray_y01
		cpi		temp2, Y1|Y2
		breq	gray_y11
gray_y10:
		cpi		temp1, 0
		breq	gray_dec_y
		cpi		temp1, Y1|Y2
		breq	gray_inc_y
		rjmp	gray_y_done
gray_y00:
		cpi		temp1, Y1
		breq	gray_dec_y
		cpi		temp1, Y2
		breq	gray_inc_y
		rjmp	gray_y_done
gray_y01:
		cpi		temp1, Y1|Y2
		breq	gray_dec_y
		cpi		temp1, 0
		breq	gray_inc_y
		rjmp	gray_y_done
gray_y11:
		cpi		temp1, Y2
		breq	gray_dec_y
		cpi		temp1, Y1
		breq	gray_inc_y
		rjmp	gray_y_done

gray_inc_y:
		inc		temp0
		rjmp	gray_y_done
gray_dec_y:
		dec		temp0

gray_y_done:
		sts		ydelta, temp0

		sts		pdstate, temp3

int_exit:
		pop		temp3
		pop		temp2
		pop		temp1
		pop		temp0

		out		SREG, sregsav
		pop		sregsav
		reti

