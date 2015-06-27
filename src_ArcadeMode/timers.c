#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "report.h"
#include "timers.h"

/*	PC0	Button 1
	PB1	Button 2
	PD4	Button 3
	PD0	Button 4
	PB5	X1 (Paddle 1)
	PB4	Y1 (Paddle 2)
	PB3	X2
	PB4	Y2
	PC1	(55x timer trigger)

	Measure resistance of potentiometers using 556 Dual Timers or 558 Quad Timers.

	Each 555 timer is used in monostable mode to measure the position of a potentiometer.
	The resistance of the pot controls the pulse length. Based on early analogue systems
	such as the Atari 2600 and IBM PC Gameport.

	Idea: make two measurements and average them, or compare with the last reading and
	discard if the difference is too large to try to prevent jumping around due to
	poor wiper contact on the pots.
*/

void ReadTimers(report_t *reportBuffer)
{
	int		timer;
	uchar	flag;

	DDRB	&= 0b11000001;				// X1/Y1/X2/Y2/Button 2 as inputs
	PORTB	|= 0b00111110;				// Pull-ups

	DDRC	&= 0b11000010;				// B2/B3/B4 as inputs
	DDRC	|= 0b00000010;				// Trigger as output
	PORTC	|= 0b00111111;				// Pull-ups, trigger starts high

	flag = 0;

	PORTC	&= ~(1<<1);					// trigger low, starts timer
	PORTC	|= (1<<1);					// trigger high again

	for (timer = -128; timer < 127; timer++)
	{
		_delay_us(3);

		if (PINB & (1<<5))
			reportBuffer->x = timer;
		if (PINB & (1<<4))
			reportBuffer->y = timer;
		if (PINB & (1<<3))
			reportBuffer->rx = timer;
		if (PINB & (1<<2))
			reportBuffer->ry = timer;
	}

	// set unconnected axis to centre
	if (reportBuffer->x == 126) reportBuffer->x = 0;
	if (reportBuffer->y == 126) reportBuffer->y = 0;
	if (reportBuffer->rx == 126) reportBuffer->rx = 0;
	if (reportBuffer->ry == 126) reportBuffer->ry = 0;

	if (!(PINC & (1<<0))) reportBuffer->b1 |= (1<<0);	// Button 1
	if (!(PINB & (1<<1))) reportBuffer->b1 |= (1<<1);	// Button 2
	if (!(PIND & (1<<4))) reportBuffer->b1 |= (1<<2);	// Button 3
	if (!(PIND & (1<<0))) reportBuffer->b1 |= (1<<3);	// Button 4
}
