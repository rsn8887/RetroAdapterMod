#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "report.h"
#include "n64gc.h"

//	PB0		-	data

/*	UDLR
	0000	-
	0001	2
	0010	6
	0011	-
	0100	4
	0101	3
	0110	5
	0111	-
	1000	0
	1001	1
	1010	7
	1011	-
	1100	-
*/

/*		0
	7		1
6				2
	5		3
		4
*/

extern	uchar readnintendo(report_t *reportBuffer, reportAnalogButtons_t *reportBufferAnalogButtons);

void ReadN64GC(report_t *reportBuffer)
{
	uchar	dpad;

	PORTB	&= ~(1<<0);		// data bus no pull-up
	DDRB	&= ~(1<<0);		// data bus input

	PORTD	&= ~(1<<7);		// debug
	DDRD	|= (1<<7);

	// analog buttons unused on Switch at the moment
	reportAnalogButtons_t reportBufferAnalogButtons = { 0, 0, 0, 0, 0, 0, 0, 0};

	dpad = readnintendo(reportBuffer, &reportBufferAnalogButtons);

	if (dpad & (1<<3)) reportBuffer->hat |= HAT_UP;	// up
	if (dpad & (1<<2)) reportBuffer->hat |= HAT_DOWN;	// down
	if (dpad & (1<<1)) reportBuffer->hat |= HAT_LEFT;	// left
	if (dpad & (1<<0)) reportBuffer->hat |= HAT_RIGHT;	// right

	// Gamecube L and R analog buttons report values between 0 and 30 when not pressed, contrary to Xbox
	//if (reportBufferAnalogButtons->l < 35) reportBufferAnalogButtons->l = 0;
	//if (reportBufferAnalogButtons->r < 35) reportBufferAnalogButtons->r = 0;

	// y and ry-axes are inverted on N64 and Gamecube, fix this here in C
	if (reportBuffer->y==-128) reportBuffer->y=127;
	else reportBuffer->y= -reportBuffer->y;

	if (reportBuffer->ry==-128) reportBuffer->ry=127;
	else reportBuffer->ry= -reportBuffer->ry;
}
