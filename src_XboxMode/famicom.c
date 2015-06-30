#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "report.h"
#include "famicom.h"
//#include "hid_modes.h"

/*	D5	Data
	D7	Latch
	B0	Clock

	The Famicom controller uses a TTL 4021 chip, which seems to be quite slow. The
	timings used in this routine are based on the actual timing used by the Famicom.
	Although the real controllers may work with shorter delays, this ensures
	compatibility.

	See protocol doc for details of device detection
*/

extern	uchar	hat_lut[];

#define FAMDELAY	24 //12

uchar ReverseByte(uchar byte)
{
	uchar	temp;
	uchar	bits = 7;
	
	temp = byte;
	for (byte >>= 1; byte; byte >>=1)
	{
		temp <<= 1;
		temp |= byte & 1;
		bits--;
	}
	temp <<= bits;
	
	return temp;
}

void ReadFamicom(report_t *reportBuffer)
{
	static	uchar	vbmode = 0;			// Virtual Boy mode flag

	uchar	temp;
	uchar	byte0, byte1, byte2;

	DDRD	&= ~(FAM_DAT);				// Data as input
	PORTD	|= FAM_DAT;					// with pull-up

	DDRD	|= FAM_LAT;					// Latch as output
	PORTD	|= FAM_LAT;					// starting high

	DDRB	|= FAM_CLK;					// Clock as output
	PORTB	|= FAM_CLK;					// starting high

	_delay_us(FAMDELAY * 2);			// latch pulse
	PORTD	&= ~(FAM_LAT);				// latch low again
	_delay_us(FAMDELAY);				// settle time

	byte0 = FamicomReadByte();
	byte1 = FamicomReadByte();
	byte2 = FamicomReadByte();
	//byte2 &= 0x0f;						// ignore upper 4 bits

	//reportBuffer->b1 = byte1;
	//reportBuffer->b2 = byte2;
	//return;

	// Common to all controllers
	//if (byte0 & (1<<7)) reportBuffer->x = 127;			// Right
	//if (byte0 & (1<<6)) reportBuffer->x = -128;			// Left
	//if (byte0 & (1<<5)) reportBuffer->y = 127;			// Down
	//if (byte0 & (1<<4)) reportBuffer->y = -128;			// Up
	
	if (byte0 & (1<<7)) reportBuffer->hat |= HAT_RIGHT;	// right
	if (byte0 & (1<<6)) reportBuffer->hat |= HAT_LEFT;	// left
	if (byte0 & (1<<5)) reportBuffer->hat |= HAT_DOWN;	// down
	if (byte0 & (1<<4)) reportBuffer->hat |= HAT_UP;	// up
	
	
	
	if (byte0 & (1<<3)) reportBuffer->b2 |= (1<<3);		// Start
	if (byte0 & (1<<2)) reportBuffer->b2 |= (1<<2);		// Select

	if ((byte2 & 0x0f) == (1<<3))		// Famicom Trackball
	{
		// Move d-pad to hat
		temp = (byte0 >> 4) & 0x0f;
		//reportBuffer->hat = pgm_read_byte(&hat_lut[temp]);
		//0001 up
		//0010 down
		//0100 left
		//1000 right
		if (temp & (1<<0)) reportBuffer->hat |= HAT_UP;	// up
		if (temp & (1<<1)) reportBuffer->hat |= HAT_DOWN;	// down
		if (temp & (1<<2)) reportBuffer->hat |= HAT_LEFT;	// left
		if (temp & (1<<3)) reportBuffer->hat |= HAT_RIGHT;	// right
		
		// Y
		temp = ~byte1 & 0x0f;
		if (temp > 7) temp = -16 + temp;
		reportBuffer->y = 0 - (temp * 4);
		// X
		temp = (~byte1 & 0xf0) >> 4;
		if (temp > 7) temp = -16 + temp;
		reportBuffer->x = 0 - (temp * 4);

		if (byte0 & (1<<3)) reportBuffer->b2 |= (1<<3);	// Start
		if (byte0 & (1<<2)) reportBuffer->b2 |= (1<<2);	// Select
		if (byte0 & (1<<1)) reportBuffer->b1 |= (1<<1);	// B
		if (byte0 & (1<<0)) reportBuffer->b1 |= (1<<0);	// A
	}
	else if ((byte1 & 0xC0) == 0xC0)	// Famicom mode
	{
		if (byte1 != 0xff) vbmode = 1;	// Virtual Boy detection
		
		if (vbmode == 0)				// Famicom
		{
			if (byte0 & (1<<1)) reportBuffer->b1 |= (1<<1);	// B (left)
			if (byte0 & (1<<0)) reportBuffer->b1 |= (1<<0);	// A (right)
		}
		else
		{
			temp = 0;
			if (byte0 & (1<<0)) reportBuffer->ry = 127;		// R-Down
			if (byte0 & (1<<1)) reportBuffer->rx = -128;	// R-Left
			if (byte1 & (1<<0)) reportBuffer->rx = 127;		// R-Right
			if (byte1 & (1<<1)) reportBuffer->ry = -128;	// R-Up

			if (byte1 & (1<<2)) reportBuffer->b1 |= (1<<4);	// L
			if (byte1 & (1<<3)) reportBuffer->b1 |= (1<<5);	// R
			if (byte1 & (1<<4)) reportBuffer->b1 |= (1<<1);	// B (left)
			if (byte1 & (1<<5)) reportBuffer->b1 |= (1<<0);	// A (right)
		}
	}
	else if ((byte1 & 0xf0) == 0)		// Super Famicom mode or no pad
	{
		vbmode = 0;
		if (byte0 & (1<<1)) reportBuffer->b1 |= (1<<1);	// Y (left)
		if (byte0 & (1<<0)) reportBuffer->b1 |= (1<<0);	// B (bottom)
		if (byte1 & (1<<0)) reportBuffer->b1 |= (1<<3);	// A (right)
		if (byte1 & (1<<1)) reportBuffer->b1 |= (1<<2);	// X (top)
		if (byte1 & (1<<2)) reportBuffer->b1 |= (1<<4);	// L
		if (byte1 & (1<<3)) reportBuffer->b1 |= (1<<5);	// R
	}
//	else if ((byte1 & 0xf0) == 0x80)	// SNES Mouse
//	{
//		hidMode = HIDM_MOUSE;
//
//		temp = ReverseByte(byte2) & 0x7f;
//		if (byte2 & (1<<0)) temp = 0-temp;
//		reportBufferMouse->y = temp;
//
//		byte2 = FamicomReadByte();
//		temp = ReverseByte(byte2) & 0x7f;
//		if (byte2 & (1<<0)) temp = 0-temp;
//		reportBufferMouse->x = temp;
//
//		if (byte1 & (1<<1)) reportBufferMouse->b1 |= (1<<0);		// LMB
//		if (byte1 & (1<<0)) reportBufferMouse->b1 |= (1<<1);		// RMB
//	}
	else vbmode = 0;

}

uchar FamicomRead(void)
{
	uchar i = 0;

	if (!(PIND & FAM_DAT)) i = 1;	// button pressed
	PORTB &= ~(FAM_CLK);			// clock low
	_delay_us(FAMDELAY);
	PORTB |= FAM_CLK;				// clock high
	_delay_us(FAMDELAY);
	return i;
}

uchar FamicomReadByte(void)
{
	uchar i = 0, j;

	for (j = 0; j < 8; j++)
	{
		i = i >> 1;
		if (!(PIND & FAM_DAT)) i |= (1<<7);	// button pressed
		PORTB &= ~(FAM_CLK);				// clock low
		_delay_us(FAMDELAY);
		PORTB |= FAM_CLK;					// clock high
		_delay_us(FAMDELAY);
	}

	return i;
}
