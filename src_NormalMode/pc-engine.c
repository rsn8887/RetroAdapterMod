#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "pc-engine.h"

/*	B1	DS	Data Select
	B5	UP	Up/I
	B4	RT	Right/II
	B3	DN	Down/Sel
	B2	LF	Left/Run
	D4	OE	Output Enable

	OE is also used by the auto-fire circuit to count the number of times that the
	controller is read, so that on the 4th or 8th read it can pulse the fire buttons.

	PC-Engine Avenue Pad 6 detection: Every time the OE line is toggled the pad
	switches between UDLR/RS12 and ----3456, where - is held low. There is no way of
	knowing which set you will get each time, but since UDLR cannot simultaniously
	be held it acts as a key for detection.
*/

void ReadPCE(report_t *reportBuffer)
{
	uchar	i;

	DDRB	&= ~(PCE_UP|PCE_RT|PCE_DN|PCE_LF);		// Multiplexed data lines as input
	DDRB	|= PCE_DS;								// DS as output
	PORTB	|= PCE_DS|PCE_UP|PCE_RT|PCE_DN|PCE_LF;	// DS high, pull-ups On inputs

	DDRD	|= PCE_OE;								// OE as output
	PORTD	&= ~PCE_OE;								// OE starts low (enabled)
	
	for (i = 2; i; --i)
	{
		PORTD &= ~PCE_OE;							// OE low for reading pad

		asm("nop");									// Delay for CMOS logic
		asm("nop");

		// Check for Avanue Pad 6
		if (PINB & (PCE_UP|PCE_DN|PCE_LF|PCE_RT))
		{
			// Normal pad or part 1 of double poll
			if (!(PINB & PCE_UP)) reportBuffer->y = -128;		// Up
			if (!(PINB & PCE_RT)) reportBuffer->x = 127;		// Right
			if (!(PINB & PCE_DN)) reportBuffer->y = 127;		// Down
			if (!(PINB & PCE_LF)) reportBuffer->x = -128;		// Left

			PORTB &= ~PCE_DS;						// DS low
			asm("nop");
			asm("nop");

			if (!(PINB & PCE_UP)) reportBuffer->b1 |= (1<<0);	// I / A
			if (!(PINB & PCE_RT)) reportBuffer->b1 |= (1<<1);	// II / B
			if (!(PINB & PCE_DN)) reportBuffer->b2 |= (1<<1);	// Select
			if (!(PINB & PCE_LF)) reportBuffer->b2 |= (1<<0);	// Run / Start
		}
		else
		{
			// Part 2 of double poll

			PORTB &= ~PCE_DS;						// DS low
			asm("nop");
			asm("nop");

			if (!(PINB & PCE_UP)) reportBuffer->b1 |= (1<<2);	// III
			if (!(PINB & PCE_RT)) reportBuffer->b1 |= (1<<3);	// IV
			if (!(PINB & PCE_DN)) reportBuffer->b1 |= (1<<4);	// V
			if (!(PINB & PCE_LF)) reportBuffer->b1 |= (1<<5);	// VI
		}

		PORTD |= PCE_OE;							// OE and DS high again
		PORTB |= PCE_DS;
	}
}
