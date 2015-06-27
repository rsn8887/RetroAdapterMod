#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "cd32.h"

/*	B5	-	Up
	B4	-	Down
	B3	-	Left
	B2	-	Right
	B1	-	(Latch)
	D4	-	(Clock)
	D0	-	(Data)

	Latch	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0
	Clock	NA	1	0	1	0	1	0	1	0	1	0	1	0	1	0	1	0
	pin9: 		B	B	R	R	Y	Y	G	G	Rt	Lf	||	||	1	1	0	0
*/

#define	LAT	1
#define	CLK	4
#define	DAT	0

void ReadCD32(report_t *reportBuffer)
{
	DDRB	&= 0b11000011;		// see table above
	DDRB	|= (1<<LAT);
	PORTB	|= 0b00111110;

	DDRD	&= ~(1<<DAT);
	DDRD	|= (1<<CLK);
	PORTD	|= (1<<CLK)|(1<<DAT);

	PORTB &= ~(1<<LAT);										// load data into latch

	if (!(PINB & (1<<5))) reportBuffer->y = -127;			// up
	if (!(PINB & (1<<4))) reportBuffer->y = 127;			// down
	if (!(PINB & (1<<3))) reportBuffer->x = -127;			// left
	if (!(PINB & (1<<2))) reportBuffer->x = 127;			// right

	if (!CD32ReadBit()) reportBuffer->b1 |= (1<<1);			// Blue
	if (!CD32ReadBit()) reportBuffer->b1 |= (1<<0);			// Red
	if (!CD32ReadBit()) reportBuffer->b1 |= (1<<3);			// Yellow
	if (!CD32ReadBit()) reportBuffer->b1 |= (1<<2);			// Green
	if (!CD32ReadBit()) reportBuffer->b1 |= (1<<7);			// Right Shoulder
	if (!CD32ReadBit()) reportBuffer->b1 |= (1<<6);			// Left Shoulder
	if (!CD32ReadBit()) reportBuffer->b2 |= (1<<0);			// Pause

	PORTB |= (1<<LAT);										// reset latch
}

uchar CD32ReadBit(void)
{
	uchar	temp = 0;
	
	//if (PIND & (1<<DAT)) temp = 1;
	PORTD &= ~(1<<CLK);
	_delay_us(75);
	if (PIND & (1<<DAT)) temp = 1;
	PORTD |= (1<<CLK);
	return temp;
}
