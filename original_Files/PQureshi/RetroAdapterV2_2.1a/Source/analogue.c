#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "analogue.h"
#include "hid_modes.h"

/*	--- Analogue controller input ---

	Measurement of potentiometer wipers connected to ADC pins provides 4 analogue inputs
	(X, Y, Rx, Ry) with either 2 buttons directly connected or 12 buttons in a 3x4 matrix.

	Designed for connection of BBC Micro / BBC Master analogue joysticks, it is also
	possible to connect almost any analogue controller which uses potentiometer wiper
	outputs. The reference voltage is always 5V and ideally the range of output should
	span 0-5V too, but smaller ranges are acceptable due to being able to calibrate joysticks
	on most systems, with some loss of accuracy. Accuracy is 8 bits over the full 0-5V range.

	For 12 button mode, the 3x4 matrix is read by grounding each column in turn and a row
	of four buttons read by the BBC Micro. For compatibility and authenticity (e.g. with
	ghosting due to the keypad matrix) the Retro Adapter does it this way too.

*/

extern	uchar	hidMode;

void ReadAnalogue(report_t *reportBuffer, uchar id)
{
	DDRD	&= ~((1<<0)|(1<<4));		// pin 15 (2/12 button selector switch), pin 14 (row3/b1)
	PORTD	|= (1<<0)|(1<<4);			// pull-ups

	DDRB	|= (1<<5)|(1<<4)|(1<<3);	// columns 0-2 or 0V outputs
	DDRB	&= ~(1<<1);					// row2/b2 input
	PORTB	|= (1<<1);					// row2/b2 pull-up

	DDRC	&= 0b11100000;				// all inputs
	PORTC	|= 0b00011111;				// analogue pins have no pull-ups but they are enabled for
										// when in digital mode

	if (PIND & (1<<0))								// high = 2 button mode
	{
		DDRB	|= (1<<2);							// pin 6 output
		PORTB	&= ~((1<<5)|(1<<4)|(1<<3)|(1<<2));	// 0v
	}
	else											// low = 12 button mode
	{
		DDRB	&= ~(1<<2);							// pin 6 row1 input
		PORTB	|= (1<<5)|(1<<4)|(1<<3)|(1<<2);		// columns start high, pull up on row1
		DDRC	&= ~(1<<0);							// pin 13 row0 input
		PORTC	|= (1<<0);							// pull-up
	}

	// Set up the ADC
	DIDR0	= 0b00010111;				// disbale digital inputs on ADC pins
	ADCSRA	= 5;						// prescaler 32

	ADMUX	= (1<<REFS0)|(1<<ADLAR)|4;	// AVcc (5V) reference voltage, left justified result
										// PC4/ADC4 - Y
	ADCSRA	|= (1<<ADEN);				// enable ADC
	while (ADCSRA & (1<<ADSC));			// wait for initialization, required as ADC is turned off
										// after use in case user changes Connector

	// Voltmace or standard
	if (id == 1)
	{
		ADCSRA	|= (1<<ADSC);				// start conversion
		while (ADCSRA & (1<<ADSC));			// wait for conversion to finish
		reportBuffer->y = 127 - ADCH;

		ADMUX	= (1<<REFS0)|(1<<ADLAR)|2;	// PC2/ADC2 - X
		ADCSRA	|= (1<<ADSC);				// start conversion
		while (ADCSRA & (1<<ADSC));			// wait for conversion to finish
		reportBuffer->x = 127 - ADCH;
	}


	if (PIND & (1<<0))					// high = 2 button, 4 axis mode
	{
		hidMode = HIDM_2P;

		if (id == 2)
		{
			ADMUX	= (1<<REFS0)|(1<<ADLAR)|1;	// PC1/ADC1 - Rx
			ADCSRA	|= (1<<ADSC);				// start conversion
			while (ADCSRA & (1<<ADSC));			// wait for conversion to finish
			reportBuffer->x = 127 - ADCH;

			ADMUX	= (1<<REFS0)|(1<<ADLAR)|0;	// PC0/ADC0 - Ry
			ADCSRA	|= (1<<ADSC);				// start conversion
			while (ADCSRA & (1<<ADSC));			// wait for conversion to finish
			reportBuffer->y = 127 - ADCH;
		}
	}

	ADCSRA	= 0;						// disable ADC
	DIDR0	= 0;						// enable digital inputs on ADC pins

	// Buttons
	if (PIND & (1<<0))					// high = 2 button mode
	{
		if (!(PIND & (1<<4)) && id == 1) reportBuffer->b1 |= (1<<0);
		if (!(PINB & (1<<1)) && id == 2) reportBuffer->b1 |= (1<<0);
	}
	else
	{
		hidMode = HIDM_1P;
		// Read 3 columns of 4 buttons. Button numbering is from the Voltmace 14B manual and
		// since USB numbers buttons starting from 1 the USB button is 14B+1
		PORTB &= ~(1<<3);				// column 0
		_delay_us(1);
		if (!(PINC & (1<<0))) reportBuffer->b1 |= (1<<1);	// 0
		if (!(PINB & (1<<2))) reportBuffer->b1 |= (1<<4);	// 3
		if (!(PINB & (1<<1))) reportBuffer->b1 |= (1<<7);	// 6
		if (!(PIND & (1<<4))) reportBuffer->b2 |= (1<<2);	// 9
		PORTB |= (1<<3);

		PORTB &= ~(1<<4);				// column 1
		_delay_us(1);
		if (!(PINC & (1<<0))) reportBuffer->b1 |= (1<<2);	// 1
		if (!(PINB & (1<<2))) reportBuffer->b1 |= (1<<5);	// 4
		if (!(PINB & (1<<1))) reportBuffer->b2 |= (1<<0);	// 7
		if (!(PIND & (1<<4))) reportBuffer->b1 |= (1<<0);	// 10
		PORTB |= (1<<4);

		PORTB &= ~(1<<5);				// column 2
		_delay_us(1);
		if (!(PINC & (1<<0))) reportBuffer->b1 |= (1<<3);	// 2
		if (!(PINB & (1<<2))) reportBuffer->b1 |= (1<<6);	// 5
		if (!(PINB & (1<<1))) reportBuffer->b2 |= (1<<1);	// 8
		if (!(PIND & (1<<4))) reportBuffer->b2 |= (1<<3);	// 11
		PORTB |= (1<<5);
	}
}
