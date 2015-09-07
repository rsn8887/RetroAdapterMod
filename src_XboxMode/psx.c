#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "report.h"
#include "psx.h"

#define PSXCLK	12	//  14 us from falling edge to rising edge this low clock values ensures that the adapter works with PSX, Dualshock and NegCon without any bitflips and jitter.
#define PSXBYTEDELAY 3 // 3 us between bytes
#define PSXCLKHIGH	12 // 14 us from rising to falling edge NegCon needs this to be 9 us at least the other controllers function with delays as low as 3us

// ps2 pressure button support, preliminary
//#define PS2PRESSURE

static uchar enter_config[]={0x01,0x43,0x00,0x01,0x00};
static uchar set_bytes_large[]={0x01,0x4F,0x00,0xFF,0xFF,0x03,0x00,0x00,0x00};
static uchar exit_config[]={0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};

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

void ReadPSX(report_t *reportBuffer, reportAnalogButtons_t *reportBufferAnalogButtons)
{
	uchar	data, id;

	DDRB	&= 0b11011101;			// See table above for I/O directions
	DDRB	|= 0b00011100;
	PORTB	|= 0b00111110;			// Pull-ups prevent floating inputs, all outputs start high
	
	PORTB &= ~ATT;					// Attention line low for duration of comms
	_delay_us(PSXBYTEDELAY);

	data = PSXCommand(0x01);		// Issue start command, data is discarded
	
	id = PSXCommand(0x42);			// Request controller ID
	
#ifdef PS2PRESSURE
	// pressure sensitive PS2 Button support WIP, currently disabled because those buttons are horrible in gameplay
	// in addition, this way of trying to force pressure sensitive buttons is too slow. It slows down a regular dualshock in analog mode
	// because we try to switch it into analog mode every single cycle
	if (id == PSX_ID_A_RED) //if reported as analog controller, then try to switch to PS2 pressure button mode (id 0x79)
	{
		PORTB |= ATT;				// ATT high again
		_delay_us(PSXBYTEDELAY);	// wait a few us
		
		PSXSendCommandString(enter_config, sizeof(enter_config));
  		PSXSendCommandString(set_bytes_large, sizeof(set_bytes_large));
  		PSXSendCommandString(exit_config, sizeof(exit_config));
		
		_delay_us(100);
		
  		PORTB &= ~ATT;					// Attention line low for duration of comms
		_delay_us(PSXBYTEDELAY);

		data = PSXCommand(0x01);		// Issue start command, data is discarded
	
		id = PSXCommand(0x42);			// Request controller ID, it could now be supporting pressure buttons if it is indeed a PS2 controller!	
    }
#endif
	if ((id == PSX_ID_DIGITAL) | (id == PSX_ID_A_RED) | (id == PSX_ID_A_GREEN) | (id == PSX_ID_PS2_PRESSURE))
	{
		data = PSXCommand(0xff);	// expect 0x5a from controller
	
		data = PSXCommand(0xff);
		if (!(data & (1<<0))) reportBuffer->b2 |= (1<<0);	// Select
		if (!(data & (1<<3))) reportBuffer->b2 |= (1<<1);	// Start

		if (id == PSX_ID_DIGITAL)
		{
			if (!(data & (1<<4))) reportBuffer->hat |= HAT_UP;		// Up
			if (!(data & (1<<6))) reportBuffer->hat |= HAT_DOWN;	// Down
			if (!(data & (1<<7))) reportBuffer->hat |= HAT_LEFT;	// Left
			if (!(data & (1<<5))) reportBuffer->hat |= HAT_RIGHT;	// Right
		}
		else if((id == PSX_ID_A_RED))
		{
			if (!(data & (1<<1))) reportBuffer->b2 |= (1<<2);	// L3 Left joystick
			if (!(data & (1<<2))) reportBuffer->b2 |= (1<<3);	// R3 Right joystick
			reportBuffer->hat = ~(data>>4)&0x0f;
		}

		data = PSXCommand(0xff);

		if (!(data & (1<<0))) reportBuffer->b1 |= (1<<4);	// L2
		if (!(data & (1<<1))) reportBuffer->b1 |= (1<<5);	// R2
		if (!(data & (1<<2))) reportBuffer->b1 |= (1<<6);	// L1
		if (!(data & (1<<3))) reportBuffer->b1 |= (1<<7);	// R1
		if (!(data & (1<<4))) reportBuffer->b1 |= (1<<3);	// /\ Triangle
		if (!(data & (1<<5))) reportBuffer->b1 |= (1<<1);	// O  Circle
		if (!(data & (1<<6))) reportBuffer->b1 |= (1<<0);	// X  Cross
		if (!(data & (1<<7))) reportBuffer->b1 |= (1<<2);	// [] Square
		
		if ((id == PSX_ID_A_RED) | (id == PSX_ID_A_GREEN) | (id == PSX_ID_PS2_PRESSURE))
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
		
		if (id==PSX_ID_PS2_PRESSURE)
		{
			// here come the pressure sensitive button values
			data = PSXCommand(0xff); //byte 9: right
			//discard because xbox does not support pressure sensitive dpad buttons
			data = PSXCommand(0xff); //byte 10: left
			//discard
			data = PSXCommand(0xff); //byte 11: up
			//discard
			data = PSXCommand(0xff); //byte 12: down
			//discard
				
			data = PSXCommand(0xff); //byte 13: Triangle
			reportBufferAnalogButtons->y = data;
				
			data = PSXCommand(0xff); //byte 14: Circle
			reportBufferAnalogButtons->b = data;
				
			data = PSXCommand(0xff); //byte 15: Cross
			reportBufferAnalogButtons->a = data;
				
			data = PSXCommand(0xff); //byte 16: Square
			reportBufferAnalogButtons->x = data;
				
			data = PSXCommand(0xff); //byte 17: L1
			reportBufferAnalogButtons->l = data;
				
			data = PSXCommand(0xff); //byte 18: R1
			reportBufferAnalogButtons->r = data;
				
			data = PSXCommand(0xff); //byte 19: L2
			reportBufferAnalogButtons->white = data;
				
			data = PSXCommand(0xff); //byte 20: R2
			reportBufferAnalogButtons->black = data; 
		}
	}
	if (id == PSX_ID_NEGCON)
	{
		data = PSXCommand(0xff);	// expect 0x5a from controller
		
		data = PSXCommand(0xff);
		
		if (!(data & (1<<3))) reportBuffer->b2 |= (1<<1);	// Start
		reportBuffer->hat = ~(data>>4)&0x0f;

		data = PSXCommand(0xff);
		
		if (!(data & (1<<3))) reportBuffer->b1 |= (1<<7);	// R1
		if (!(data & (1<<4))) reportBuffer->b1 |= (1<<3);	// /\ Triangle (A on Negcon)
		if (!(data & (1<<5))) reportBuffer->b1 |= (1<<1);	// O  Circle (B on Negcon)
		
		data = PSXCommand(0xff); //Steering axis 0x00 = left: 
		//tested on a real psx: moving the right half away from you turns the Wipeout vehicle to the right!
		reportBuffer->x = -128+(char)data;
		
		data = PSXCommand(0xff); //I button (bottom button analog)
		reportBufferAnalogButtons->a = data;
			
		data = PSXCommand(0xff); //II button (left button analog)
		reportBufferAnalogButtons->x = data;

		data = PSXCommand(0xff); //L1 Button analog
		reportBufferAnalogButtons->l = data;
	}
	
	PORTB |= ATT;				// ATT high again
	_delay_us(PSXBYTEDELAY);
}

uchar PSXCommand(uchar command)
{
	uchar i = 0;
	uchar data = 0;

	_delay_us(PSXBYTEDELAY);

	for (i = 0; i < 8; i++)
	{
		// set command line
		if (command & 1) PORTB |= CMD;
		else PORTB &= ~CMD;
		command >>= 1;

		PORTB &= ~CLK;				// clock falling edge this is when data is changed by both host and controller
		
		_delay_us(PSXCLK);
		
		data >>= 1;		
	
		PORTB |= CLK;				// clock rising edge this is when data is read by both host and controller

		if (PINB & DAT) data |= (1<<7); //(1<<i); // if this command is done after the next command, there are some random bit flips most notably on NegCon
		
		_delay_us(PSXCLKHIGH);
	
	}
	_delay_us(PSXBYTEDELAY);
	
	return data;
}

void PSXSendCommandString(uchar string[], uchar len) 
{
	// low enable joysticks
	PORTB &= ~ATT;
	for (int y=0; y<len; y++)
	{
		PSXCommand(string[y]);
	};
	PORTB |= ATT;
	_delay_us(PSXBYTEDELAY);
}
