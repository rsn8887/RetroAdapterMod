#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "dualdb9.h"

/*	b1:	0	1	2	3	4	5	6	7
		A	B	C	X	Y	Z	St	Sel
*/

void ReadDualDB9(report_t *reportBuffer)
{
	DDRB	&= 0b11000001;				// DB15 inputs
	PORTB	|= 0b00111110;				// Pull-ups
	
	DDRC	|= (1<<2);					// PC2 (Select) output
	DDRC	&= ~(1<<1);					// PC1 as input
	PORTC	|= (1<<1);					// Pull-up
	PORTC	&= ~(1<<2);					// Select starts low

	switch (PINB & 0b00111000)
	{
		case (0):						// Famicom
			Read2ndFamicom(reportBuffer);
			break;
		default:						// Atari / Amiga / Sega
			Read2ndDirect(reportBuffer);
			break;
	}
}



void Read2ndDirect(report_t *reportBuffer)
{
	uchar	megadrive = 0;

	_delay_us(14);						// The custom chip in Megadrive 6 button controllers is
										// a bit slower than CMOS logic

	if ((PINB & 0b00001100) == 0)		// if L/R low then Megadrive pad detected
	{
		megadrive = 1;
		if (!(PINB & (1<<1))) reportBuffer->b1 |= (1<<0);	// Megadrive A
		if (!(PINC & (1<<1))) reportBuffer->b2 |= (1<<1);	// Megadrive Start
	}

	PORTC	|= (1<<2);					// Select high for Megadrive

	_delay_us(14);

	if (!(PINB & (1<<5))) reportBuffer->y = -128;		// up
	if (!(PINB & (1<<4))) reportBuffer->y = 127;		// down
	if (!(PINB & (1<<3))) reportBuffer->x = -128;		// left
	if (!(PINB & (1<<2))) reportBuffer->x = 127;		// right

	if (megadrive == 0)
	{
		if (!(PINB & (1<<1))) reportBuffer->b1 |= (1<<0);	// Fire 1
		if (!(PINC & (1<<1))) reportBuffer->b1 |= (1<<1);	// Fire 2
	}
	else
	{
		if (!(PINB & (1<<1))) reportBuffer->b1 |= (1<<1);	// Fire 1 / Megadrive B
		if (!(PINC & (1<<1))) reportBuffer->b1 |= (1<<2);	// Fire 2 / Megadrive C
				
		// Megadrive six button controller detection //

		PORTC	&= ~(1<<2);					// select low
		_delay_us(14);
		PORTC	|= (1<<2);					// select high
		_delay_us(14);
		PORTC	&= ~(1<<2);					// select low
		_delay_us(14);

		// U/D/L/R are low if six button controller
		if ((PINB & 0b00111100) == 0)
		{
			PORTC	|= (1<<2);					// select high
			_delay_us(14);

			if (!(PINB & (1<<5))) reportBuffer->b1 |= (1<<7);	// Z
			if (!(PINB & (1<<4))) reportBuffer->b1 |= (1<<6);	// Y
			if (!(PINB & (1<<3))) reportBuffer->b1 |= (1<<3);	// X
			if (!(PINB & (1<<2))) reportBuffer->b2 |= (1<<0);	// Mode
		}
	}
}


void Read2ndFamicom(report_t *reportBuffer)
{
	uchar	temp;

	DDRB	&= ~(FAM2_DAT);			// Data as input
	PORTB	|= FAM2_DAT;			// with pull-up

	DDRC	|= FAM2_LAT;			// Latch as output
	PORTC	|= FAM2_LAT;			// starting high

	DDRC	|= FAM2_CLK;			// Clock as output
	PORTC	|= FAM2_CLK;			// starting high

	_delay_us(FAMDELAY * 2);		// latch pulse
	PORTC	&= ~(FAM2_LAT);			// latch low again
	_delay_us(FAMDELAY);			// settle time

	// button IDs in brackers are Super Famicom
	if (Famicom2Read()) reportBuffer->b1 |= (1<<1);	// A (snes B)
	if (Famicom2Read()) reportBuffer->b1 |= (1<<0);	// B (snes Y)
	if (Famicom2Read()) reportBuffer->b2 |= (1<<0);	// Select 
	if (Famicom2Read()) reportBuffer->b2 |= (1<<1);	// Start
	if (Famicom2Read()) reportBuffer->y = -128;		// Up
	if (Famicom2Read()) reportBuffer->y = 127;		// Down
	if (Famicom2Read()) reportBuffer->x = -128;		// Left
	if (Famicom2Read()) reportBuffer->x = 127;		// Right

	// Super Famicom only bits
	if (Famicom2Read()) reportBuffer->b1 |= (1<<2);	// (A)
	if (Famicom2Read()) reportBuffer->b1 |= (1<<3);	// (X)
	if (Famicom2Read()) reportBuffer->b1 |= (1<<6);	// (L)
	if (Famicom2Read()) reportBuffer->b1 |= (1<<7);	// (R)
	// If the next bit is high, Super Famicom detected so need to remap buttons.
	// If not, mask out above bits which are always low on the Famicom
	if (Famicom2Read())
	{
		reportBuffer->b1 &= 0b00000011;	// mask out non-Famicom buttons
	} 
	else {
		temp = reportBuffer->b1;
		reportBuffer->b1 &= 0b11110000;	// clear buttons to be remapped
		if (temp & (1<<0)) reportBuffer->b1 |= (1<<2);	// Snes Y
		if (temp & (1<<1)) reportBuffer->b1 |= (1<<0);	// Snes B
		if (temp & (1<<2)) reportBuffer->b1 |= (1<<1);	// Snes A
		if (temp & (1<<3)) reportBuffer->b1 |= (1<<3);	// Snes X
	}
}

uchar Famicom2Read(void)
{
	uchar i = 0;

	if (!(PINB & FAM2_DAT)) i = 1;	// button pressed
	PORTC &= ~(FAM2_CLK);			// clock low
	_delay_us(FAMDELAY);
	PORTC |= FAM2_CLK;				// clock high
	_delay_us(FAMDELAY);
	return i;
}

