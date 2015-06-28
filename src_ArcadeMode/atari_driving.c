#include <avr/io.h>
#include "report.h"
#include "atari_driving.h"

/*	Decode Atari driving controller.

	The controller is just a rotary encoder using 2 bit Gray code. It is possible to
	decode by just watching for falling edges on one line, but to get more resolution
	we do a full decode here. It isn't clear which method the 2600 uses.

	The driving controller produces left/right joystick movements as it rotates. This change
	allows it be used with MAME and other emulators in Arkanoid style games.

	The old comments are left here for reference in case the old system needs to be re-implemented:

	*** OLD COMMENTS ***

	For compatibility with the /other/ driving controller adapter, the Retro Adapter
	makes the joystick spin in the sequence up, right, down, left for clockwise rotation.
	This should be compatible with all current emulators.

	*** END OF OLD COMMENTS ***

	TO DO: It is possible to support a second driving controller. A simple wiring adapter
	is all that is required. It could be attached to the 9 pin port, or to reduce the parts
	required it could use the 15 pin port.
*/

void ReadAtariDriving(report_t *reportBuffer)
{
	static uchar	pos = 0;
	static uchar	last = 0;
	static uchar	bounce = 0;
	uchar			cur;

	DDRD	&= 0b00000110;				// All inputs except USB D+/-
	PORTD	|= 0b11111001;				// Pull-ups
	
	DDRB	&= 0b11000000;				// All inputs
	PORTB	|= 0b00111111;				// Pull-ups

	cur = PIND & (1<<0);				// read current position
	if (PIND & (1<<3)) cur |= (1<<1);

	if (cur != last)
	{
		switch (cur)
		{
			case 0b00:
				if (last == 0b01) reportBuffer->x = 127;
				if (last == 0b10) reportBuffer->x = -128;
				break;
			case 0b01:
				if (last == 0b11) reportBuffer->x = 127;
				if (last == 0b00) reportBuffer->x = -128;
				break;
			case 0b11:
				if (last == 0b10) reportBuffer->x = 127;
				if (last == 0b01) reportBuffer->x = -128;
				break;
			case 0b10:
				if (last == 0b00) reportBuffer->x = 127;
				if (last == 0b11) reportBuffer->x = -128;
				break;
		}
		last = cur;
		bounce = 3;
		pos = reportBuffer->x;
	}
	else
	{
		if (bounce > 0)
		{
			bounce--;
			reportBuffer->x = pos;
		}
	}

/*
	if (cur != last)
	{
		switch (cur)
		{
			case 0b00:
				if (last == 0b01) pos++;
				if (last == 0b10) pos--;
				break;
			case 0b01:
				if (last == 0b11) pos++;
				if (last == 0b00) pos--;
				break;
			case 0b11:
				if (last == 0b10) pos++;
				if (last == 0b01) pos--;
				break;
			case 0b10:
				if (last == 0b00) pos++;
				if (last == 0b11) pos--;
				break;
		}

		if (pos > 4) pos = 1;		// wrap around
		if (pos < 1) pos = 4;

		last = cur;
	}

	switch (pos)
	{
		case 1:
			reportBuffer->y = -127;
			break;
		case 2:
			reportBuffer->x = 127;
			break;
		case 3:
			reportBuffer->y = 127;
			break;
		case 4:
			reportBuffer->x = -127;
			break;
	}
*/
	if (!(PIND & (1<<6))) reportBuffer->b1 |= (1<<0);
}
