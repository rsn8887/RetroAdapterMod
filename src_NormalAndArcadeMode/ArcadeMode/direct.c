#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "direct.h"

/*	NB. Sel starts as output low, all directly connected controllers must be compatible with this.
		After reading the controller is is returned to high again, which allows most Atari/C=
		autofire circuits to work.
*/

void ReadDB9(report_t *reportBuffer)
{
	uchar	megadrive = 0;

	DDRD	&= 0b10000110;				// All inputs except Select, don't touch USB D+/-
	DDRD	|= 0b10000000;				// Select as output
	PORTD	&= 0b01111111;				// Select starts low
	PORTD	|= 0b01111001;				// Pull-ups
	
	DDRB	&= ~(1<<0);					// Pin 9 input
	PORTB	|= (1<<0);					// Pull-up

	/* --- DB9 controllers --------------------------------------------------------------------- */

	_delay_us(14);						// The custom chip in Megadrive 6 button controllers is
										// quite a bit slower than CMOS logic

	if ((PIND & 0b00110000) == 0)		// if L/R low then Megadrive pad detected
	{
		megadrive = 1;
		if (!(PIND & (1<<6))) reportBuffer->b1 |= (1<<0);	// Megadrive A
		if (!(PINB & (1<<0))) reportBuffer->b2 |= (1<<1);	// Megadrive Start
	}

	PORTD	|= (1<<7);					// P1 select high for Megadrive

	_delay_us(14);

	if (!(PIND & (1<<0))) reportBuffer->y = -128;			// up
	if (!(PIND & (1<<3))) reportBuffer->y = 127;			// down
	if (!(PIND & (1<<4))) reportBuffer->x = -128;			// left
	if (!(PIND & (1<<5))) reportBuffer->x = 127;			// right

	if (megadrive == 0)
	{
		if (!(PIND & (1<<6))) reportBuffer->b1 |= (1<<0);	// Fire 1
		if (!(PINB & (1<<0))) reportBuffer->b1 |= (1<<1);	// Fire 2
	}
	else
	{
		if (!(PIND & (1<<6))) reportBuffer->b1 |= (1<<1);	// Fire 1 / Megadrive B
		if (!(PINB & (1<<0))) reportBuffer->b1 |= (1<<2);	// Fire 2 / Megadrive C
				
		// Megadrive six button controller detection //

		PORTD	&= ~(1<<7);					// select low
		_delay_us(14);
		PORTD	|= (1<<7);					// select high
		_delay_us(14);
		PORTD	&= ~(1<<7);					// select low
		_delay_us(14);

		// U/D/L/R are low if six button controller
		if ((PIND & 0b00111001) == 0)
		{
			PORTD	|= (1<<7);					// select high
			_delay_us(14);

			if (!(PIND & (1<<0))) reportBuffer->b1 |= (1<<7);	// Z
			if (!(PIND & (1<<3))) reportBuffer->b1 |= (1<<6);	// Y
			if (!(PIND & (1<<4))) reportBuffer->b1 |= (1<<3);	// X
			if (!(PIND & (1<<5))) reportBuffer->b2 |= (1<<0);	// Mode
		}

	}

	PORTD	|= (1<<7);					// Select high for Megadrive between polls
}
	// Note: Select line remains high to provide +5V power to Amiga joysticks, which used it
	// for auto-fire. Most auto-fire circuits seem to work fine despite the line being breifly
	// pulled low for Megadrive controllers.

	/* --- NeoGeo controller ------------------------------------------------------------------- */
void ReadDB15(report_t *reportBuffer)
{
	DDRB	&= 0b11000001;				// PB1-PB5 inputs
	PORTB	|= 0b00111110;				// Pull-ups

	DDRC	&= 0b11000000;				// PC0-PC5 inputs
	PORTC	|= 0b00111111;				// Pull-ups

	DDRD	&= ~((1<<0)|(1<<4));		// PD0/PD4 inputs
	PORTD	|= (1<<0)|(1<<4);

/*
	if (!(PINB & (1<<5))) reportBuffer->b2 |= (1<<2);	// Select
	if (!(PINB & (1<<4))) reportBuffer->b1 |= (1<<3);	// D
	if (!(PINB & (1<<3))) reportBuffer->b1 |= (1<<1);	// B
	if (!(PINB & (1<<2))) reportBuffer->x = 127;		// Right
	if (!(PINB & (1<<1))) reportBuffer->y = 127;		// Down
	if (!(PINC & (1<<2))) reportBuffer->b2 |= (1<<3);	// Start
	if (!(PINC & (1<<1))) reportBuffer->b1 |= (1<<2);	// C
	if (!(PINC & (1<<0))) reportBuffer->b1 |= (1<<0);	// A
	// Up is shared with the DB9 so has already been checked
*/
// Change by rsn8887 06/17/2015 - Arcade mode instead of NeoGeo
	if (!(PINB & (1<<5))) reportBuffer->b1 |= (1<<6);	// Button 7
	if (!(PINB & (1<<4))) reportBuffer->b1 |= (1<<3);	// Button 4
	if (!(PINB & (1<<3))) reportBuffer->b1 |= (1<<5);	// Button 6
	if (!(PINB & (1<<2))) reportBuffer->b2 |= (1<<0);	// Button 9
	if (!(PINB & (1<<1))) reportBuffer->b2 |= (1<<1);	// Button 10
	if (!(PINC & (1<<2))) reportBuffer->b1 |= (1<<7);	// Button 8
	if (!(PINC & (1<<1))) reportBuffer->b1 |= (1<<2);	// Button 3
	if (!(PINC & (1<<0))) reportBuffer->b1 |= (1<<4);	// Button 5
}
