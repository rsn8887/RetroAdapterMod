#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "famicom.h"
#include "hid_modes.h"

/*	B5	-	Y1		PCINT5
	B3	-	Y2		PCINT3
	B4	-	X1		PCINT4
	B2	-	X2		PCINT2
	B1	-	LMB
	C0	-	RMB
*/

extern	uchar	hidMode;

char	xdelta, ydelta;
uchar	pdstate, bounce1;

void ReadAmigaMouse(reportMouse_t *reportBufferMouse)
{
	DDRB	&= 0b11000001;	// inputs with pull-ups
	PORTB	|= 0b00111110;
	DDRC	&= 0b11111110;
	PORTC	|= 0b00000001;

	// set up and enable interrupt
	PCMSK0	= (1<<PCINT5)|(1<<PCINT4)|(1<<PCINT3)|(1<<PCINT2);
	PCICR	= (1<<PCIE0);

	reportBufferMouse->x = xdelta;
	reportBufferMouse->y = ydelta;
	xdelta = 0;
	ydelta = 0;
	if (!(PINB & (1<<1))) reportBufferMouse->b1 |= (1<<0);
	if (!(PINC & (1<<0))) reportBufferMouse->b1 |= (1<<1);
}
