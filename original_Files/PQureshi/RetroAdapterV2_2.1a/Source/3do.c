#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "3do.h"

/*	D0	-	GND			OUT
	D3	-	5V			OUT
	D4	-	Audio.1		IN
	D5	-	Audio.2		IN
	D6	-	P/S			OUT
	D7	-	Clock		OUT
	B0	-	Data		IN
*/

#define	PS	(1<<6)
#define	CLK	(1<<7)
#define	DAT	(1<<0)

void Read3DO(report_t *reportBuffer)
{
	uchar	temp;

	PORTD	|= 0b11111000;		// See table above
	PORTD	&= 0b11111110;
	DDRD	|= 0b11001001;
	DDRD	&= 0b11001111;

	DDRB	&= 0b11111110;		// Data as input with pull-up
	PORTB	|= 0b00000001;

	PORTD	&= ~PS;				// P/S low to start reading bits

	//	Bit		0	1	2	3	4	5	6	7
	//			Lf	Rt	Up	Dn	0	0	1	0

	temp = TDOReadByte();

	// Check for controller ID
	if ((temp & 0xf0) != 0b01000000)
		return;

	if (temp & (1<<0))	reportBuffer->x = -127;
	if (temp & (1<<1))	reportBuffer->x = 127;
	if (temp & (1<<2))	reportBuffer->y = -127;
	if (temp & (1<<3))	reportBuffer->y = 127;

	//	Bit		0	1	2	3	4	5	6	7
	//			0	L	R	X	P	C	B	A

	temp = TDOReadByte();
	if (temp & (1<<1))	reportBuffer->b1 |= (1<<6);
	if (temp & (1<<2))	reportBuffer->b1 |= (1<<7);
	if (temp & (1<<3))	reportBuffer->b2 |= (1<<1);
	if (temp & (1<<4))	reportBuffer->b2 |= (1<<0);
	if (temp & (1<<5))	reportBuffer->b1 |= (1<<2);
	if (temp & (1<<6))	reportBuffer->b1 |= (1<<1);
	if (temp & (1<<7))	reportBuffer->b1 |= (1<<0);

	PORTD	|= PS;				// End of data transfer
}

uchar TDOReadByte()
{
	uchar	i, temp;

	temp = 0;

	for (i = 0; i<8; i++)
	{
		temp = temp << 1;
		if (PINB & DAT) temp |= 1;
		PORTD &= ~CLK;
		_delay_us(4);
		PORTD |= CLK;
		_delay_us(4);
	}

	return temp;
}
