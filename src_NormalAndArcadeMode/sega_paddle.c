#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "sega_paddle.h"

void ReadSegaPaddle(report_t *reportBuffer)
{
	uchar	temp, i;
	// static	uchar	last = 0;

	DDRD	&= 0b00000110;				// All inputs, don't touch USB D+/-
	PORTD	|= 0b11111001;				// Pull-ups
	
	DDRB	&= ~(1<<0);					// Pin 9 input
	PORTB	|= (1<<0);					// Pull-up

	if (!(PIND & (1<<6))) reportBuffer->b1 |= (1<<0);	// Fire button

	i = 0;

	temp = 0;
	while (PINB & (1<<0))
	{
		temp++;
		if (temp > 250) return;
	}
	temp = PIND;
	if (!(temp & (1<<0))) i |= (1<<0);
	if (!(temp & (1<<3))) i |= (1<<1);
	if (!(temp & (1<<4))) i |= (1<<2);
	if (!(temp & (1<<5))) i |= (1<<3);

	temp = 0;
	while (!(PINB & (1<<0)))
	{
		temp++;
		if (temp > 250) return;
	}
	temp = PIND;
	if (!(temp & (1<<0))) i |= (1<<4);
	if (!(temp & (1<<3))) i |= (1<<5);
	if (!(temp & (1<<4))) i |= (1<<6);
	if (!(temp & (1<<5))) i |= (1<<7);

	reportBuffer->x = 127 - i;
	//reportBuffer->x = (last - i); //(127 - i);
	//last = i;
}
