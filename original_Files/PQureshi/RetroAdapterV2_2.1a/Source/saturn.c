#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "report.h"
#include "saturn.h"

/*	Pin 2 - B5 - D1 (Down)
	Pin 3 - B4 - D0 (Up)
	Pin 4 - B3 - D6 (TH)
	Pin 5 - B2 - D5 (TR)
	Pin 6 - B1 - D4 (TL)
	Pin 7 - D4 - D3 (Right)
	Pin 8 - D0 - D2 (Left)

	The analogue pad sets the trigger down bit even in analogue mode. Kept this behaviour for
	compatibility with emulators.
*/

extern uchar hat_lut[];

void ReadSaturn(report_t *reportBuffer)
{
	uchar 	temp = 0;
	uchar	satmode = 0;

	DDRB	&= ~(SAT_UP|SAT_DN|SAT_TL);					// UP/DN/TL as input
	DDRB	|= (SAT_TH|SAT_TR);							// TH/TR as outputs
	PORTB	|= (SAT_UP|SAT_DN|SAT_TL|SAT_TR|SAT_TH);	// Pull-ups, TR/TH high
	
	DDRD	&= ~(SAT_RT|SAT_LF);						// inputs
	PORTD	|= (SAT_RT|SAT_LF);							// pull-ups

	_delay_us(2);

	// TH = 1, TR = 1
	if (PIND & SAT_LF)		// Standard digital pad
	{
		if (!(PIND & SAT_RT)) reportBuffer->b1 |= (1<<6);		// L

		// TH = 0, TR = 1
		PORTB &= ~(SAT_TH);

		_delay_us(2);

		if (!(PINB & SAT_UP)) reportBuffer->y = -127;			// Up
		if (!(PINB & SAT_DN)) reportBuffer->y = 127;			// Down
		if (!(PIND & SAT_LF)) reportBuffer->x = -127;			// Left
		if (!(PIND & SAT_RT)) reportBuffer->x = 127;			// Right

		// TH = 1, TR = 0
		PORTB = (PORTB & ~(SAT_TR)) | (SAT_TH);

		_delay_us(2);

		if (!(PINB & SAT_UP)) reportBuffer->b1 |= (1<<1);		// B
		if (!(PINB & SAT_DN)) reportBuffer->b1 |= (1<<2);		// C
		if (!(PIND & SAT_LF)) reportBuffer->b1 |= (1<<0);		// A
		if (!(PIND & SAT_RT)) reportBuffer->b2 |= (1<<0);		// Start

		// TH = 0, TR = 0
		PORTB &= ~(SAT_TH|SAT_TR);

		_delay_us(2);

		if (!(PINB & SAT_UP)) reportBuffer->b1 |= (1<<5);		// Z
		if (!(PINB & SAT_DN)) reportBuffer->b1 |= (1<<4);		// Y
		if (!(PIND & SAT_LF)) reportBuffer->b1 |= (1<<3);		// X
		if (!(PIND & SAT_RT)) reportBuffer->b1 |= (1<<7);		// R
	}

	else if (PINB & SAT_UP)		// Analogue pad
	{
		PORTB &= ~(SAT_TH);		// 2nd
		PORTB &= ~(SAT_TR);
		wait_tl_low();
		// 3rd

		if (PINB & SAT_UP) satmode = 1;		// 1 = analogue mode

		PORTB |= (SAT_TR);
		wait_tl_high();
		// 4th

		PORTB &= ~(SAT_TR);
		wait_tl_low();
		// 5th

		if (satmode == 0)	// digital mode
		{
			if (!(PINB & SAT_UP)) reportBuffer->y = -127;		// Up
			if (!(PINB & SAT_DN)) reportBuffer->y = 127;		// Down
			if (!(PIND & SAT_LF)) reportBuffer->x = -127;		// Left
			if (!(PIND & SAT_RT)) reportBuffer->x = 127;		// Right
		}
		else
		{
			if (!(PINB & SAT_UP)) temp |= (1<<0);				// Up
			if (!(PINB & SAT_DN)) temp |= (1<<1);				// Down
			if (!(PIND & SAT_LF)) temp |= (1<<2);				// Left
			if (!(PIND & SAT_RT)) temp |= (1<<3);				// Right
			reportBuffer->hat = pgm_read_byte(&hat_lut[temp]);
		}

		PORTB |= (SAT_TR);
		wait_tl_high();
		// 6th

		if (!(PINB & SAT_UP)) reportBuffer->b1 |= (1<<1);		// B
		if (!(PINB & SAT_DN)) reportBuffer->b1 |= (1<<2);		// C
		if (!(PIND & SAT_LF)) reportBuffer->b1 |= (1<<0);		// A
		if (!(PIND & SAT_RT)) reportBuffer->b2 |= (1<<3);		// Start

		PORTB &= ~(SAT_TR);
		wait_tl_low();
		// 7th

		if (!(PINB & SAT_UP)) reportBuffer->b1 |= (1<<5);		// Z
		if (!(PINB & SAT_DN)) reportBuffer->b1 |= (1<<4);		// Y
		if (!(PIND & SAT_LF)) reportBuffer->b1 |= (1<<3);		// X
		if (!(PIND & SAT_RT)) reportBuffer->b1 |= (1<<7);		// R

		PORTB |= (SAT_TR);
		wait_tl_high();
		// 8th

		if (!(PIND & SAT_RT)) reportBuffer->b1 |= (1<<6);		// L

		/* --- Analogue X axis --------------------------------------------------------------*/

		PORTB &= ~(SAT_TR);
		wait_tl_low();
		// 9th

		temp = 0;
		if (!(PINB & SAT_UP)) temp |= (1<<4);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<5);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<6);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<7);		// Right

		PORTB |= (SAT_TR);
		wait_tl_high();
		// 10th

		if (!(PINB & SAT_UP)) temp |= (1<<0);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<1);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<2);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<3);		// Right

		if (satmode == 1) reportBuffer->x = 255 - (temp + 0x80);

		/* --- Analogue y axis --------------------------------------------------------------*/

		PORTB &= ~(SAT_TR);
		wait_tl_low();
		// 11th

		temp = 0;
		if (!(PINB & SAT_UP)) temp |= (1<<4);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<5);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<6);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<7);		// Right

		PORTB |= (SAT_TR);
		wait_tl_high();
		// 12th

		if (!(PINB & SAT_UP)) temp |= (1<<0);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<1);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<2);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<3);		// Right

		if (satmode == 1) reportBuffer->y = 255 - (temp + 0x80);

		/* --- Right shoulder ---------------------------------------------------------------*/

		PORTB &= ~(SAT_TR);
		wait_tl_low();
		// 11th

		temp = 0;
		if (!(PINB & SAT_UP)) temp |= (1<<4);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<5);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<6);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<7);		// Right

		PORTB |= (SAT_TR);
		wait_tl_high();
		// 12th

		if (!(PINB & SAT_UP)) temp |= (1<<0);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<1);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<2);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<3);		// Right

		if (satmode == 1) reportBuffer->ry = temp - 128;

		/* --- Left shoulder ----------------------------------------------------------------*/

		PORTB &= ~(SAT_TR);
		wait_tl_low();
		// 11th

		temp = 0;
		if (!(PINB & SAT_UP)) temp |= (1<<4);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<5);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<6);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<7);		// Right

		PORTB |= (SAT_TR);
		wait_tl_high();
		// 12th

		if (!(PINB & SAT_UP)) temp |= (1<<0);		// Up
		if (!(PINB & SAT_DN)) temp |= (1<<1);		// Down
		if (!(PIND & SAT_LF)) temp |= (1<<2);		// Left
		if (!(PIND & SAT_RT)) temp |= (1<<3);		// Right

		if (satmode == 1) reportBuffer->rx = temp - 128;

		/* --- End of data transfer ---------------------------------------------------------*/

		PORTB |= (SAT_TR|SAT_TH);
	}
}

/* Wait for TL line to change state. Timeout prevents lock-up. */

void wait_tl_low(void)
{
	uchar	timer = 0;
	while (PINB & SAT_TL)
	{
		timer++;
		if (timer == 200) return;		// should only ever take max 2us
	}
}

void wait_tl_high(void)
{
	uchar	timer = 0;
	while (!(PINB & SAT_TL))
	{
		timer++;
		if (timer == 200) return;		// should only ever take max 2us
	}
}
