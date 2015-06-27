#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "report.h"
#include "psx.h"

#define PSXDELAY	10	// 16 for 12MHz

/*	LDRU
	0000	-
	0001	0
	0010	2
	0011	1
	0100	4
	0101	-
	0110	3
	0111	-
	1000	6
	1001	7
	1010	-
	1011	-
	1100	5
*/

const uchar psx_hat_lut[] PROGMEM  = { -1, 0, 2, 1, 4, -1, 3, -1, 6, 7, -1, -1, 5 };

/*
PB5		DAT		In
PB4		CMD		Out
PB3		ATT		Out
PB2		CLK		Out
PB1		ACK		In
*/

void ReadPSX(report_t *reportBuffer)
{
	uchar	data, id;

	DDRB	&= 0b11011101;			// See table above for I/O directions
	DDRB	|= 0b00011100;
	PORTB	|= 0b00111110;			// Pull-ups prevent floating inputs, all outputs start high

	PORTB &= ~ATT;					// Attention line low for duration of comms
	_delay_us(1);

	data = PSXCommand(0x01);		// Issue start command, data is discarded
	if (!(PSXWaitACK()))			// Wait for ACK, if not received then no pad connected
	{
		PORTB |= ATT;				// ATT high again
		return;
	}

	id = PSXCommand(0x42);			// Request controller ID

	if ((id == PSX_ID_DIGITAL) | (id == PSX_ID_A_RED) | (id == PSX_ID_A_GREEN))
	{
		data = PSXCommand(0xff);	// expect 0x5a from controller
		if (data == 0x5a)
		{
			data = PSXCommand(0xff);
			if (!(data & (1<<0))) reportBuffer->b2 |= (1<<1);	// Select
			if (!(data & (1<<3))) reportBuffer->b2 |= (1<<0);	// Start

			if (id == PSX_ID_DIGITAL)
			{
				if (!(data & (1<<4))) reportBuffer->y = -127;		// Up
				if (!(data & (1<<6))) reportBuffer->y = 127;		// Down
				if (!(data & (1<<7))) reportBuffer->x = -127;		// Left
				if (!(data & (1<<5))) reportBuffer->x = 127;		// Right
			}
			else if(id == PSX_ID_A_RED)
			{
				if (!(data & (1<<1))) reportBuffer->b2 |= (1<<2);	// L3 Left joystick
				if (!(data & (1<<2))) reportBuffer->b2 |= (1<<3);	// R3 Right joystick
				reportBuffer->hat = pgm_read_byte(&psx_hat_lut[(~(data>>4)&0x0f)]);
			}

			data = PSXCommand(0xff);
			if (!(data & (1<<0))) reportBuffer->b1 |= (1<<4);	// L2
			if (!(data & (1<<1))) reportBuffer->b1 |= (1<<5);	// R2
			if (!(data & (1<<2))) reportBuffer->b1 |= (1<<6);	// L1
			if (!(data & (1<<3))) reportBuffer->b1 |= (1<<7);	// R1
			if (!(data & (1<<4))) reportBuffer->b1 |= (1<<0);	// /\ Triangle
			if (!(data & (1<<5))) reportBuffer->b1 |= (1<<1);	// O  Circle
			if (!(data & (1<<6))) reportBuffer->b1 |= (1<<2);	// X  Cross
			if (!(data & (1<<7))) reportBuffer->b1 |= (1<<3);	// [] Square

			if ((id == PSX_ID_A_RED) | (id == PSX_ID_A_GREEN))
			{
				data = PSXCommand(0xff);
				reportBuffer->rx = -128+(char)data;
				data = PSXCommand(0xff);
				reportBuffer->ry = -128+(char)data;
				data = PSXCommand(0xff);
				reportBuffer->x = -128+(char)data;
				data = PSXCommand(0xff);
				reportBuffer->y = -128+(char)data;
			}
		}
	}

	PORTB |= ATT;				// ATT high again
}

char PSXWaitACK()
{
	uchar	count = 0;

	// wait approximately 50us for ACK.
	while (count < 50)
	{
		if (!(PINB & ACK)) return 1;
		count++;
		_delay_us(1);
	}
	return 0;
}

uchar PSXCommand(uchar command)
{
	uchar i = 0;
	uchar data = 0;

	// wait up to approx 50us for ACK line to go high
	while ((PINB & ACK) == 0)
	{
		i++;
		_delay_us(1);
		if (i > 50) return 0;		// for some reason ACK is stuck high, should never happen
	}								// but testing prevents an infinite loop

	_delay_us(2);

	for (i = 0; i < 8; i++)
	{
		// set command line
		if (command & 1) PORTB |= CMD;
		else PORTB &= ~CMD;
		command >>= 1;

		PORTB &= ~CLK;				// clock falling edge

		_delay_us(PSXDELAY);
		
		data >>= 1;
		PORTB |= CLK;				// clock rising edge

		_delay_us(PSXDELAY - 1);

		if (PINB & DAT) data |= (1<<7); //(1<<i);

#ifdef DEBUG
	PORTA |= (1<<3);
	PORTA &= ~(1<<3);
	if (data & (1<<7)) PORTA |= (1<<3);
	else PORTA &= ~(1<<3);
#endif
	}
	
	return data;
}
