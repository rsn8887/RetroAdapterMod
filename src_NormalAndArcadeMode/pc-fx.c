#include <avr/io.h>
#include <util/delay.h>
#include "report.h"
#include "pc-fx.h"

/*	B5	-	Data
	B4	-	Data Direction
	B3	-	Latch
	B2	-	Clock
*/

#define	FX_DAT	5
#define	FX_DD	4
#define	FX_LAT	3
#define	FX_CLK	2

void ReadPCFX(report_t *reportBuffer)
{
	uchar	byte0;

	PORTB	&= ~((1<<FX_DD)|(FX_LAT));
	PORTB	|= (1<<FX_CLK)|(1<<FX_DAT);
	DDRB	&= ~(1<<FX_DAT);
	DDRB	|= (1<<FX_DD)|(1<<FX_LAT)|(1<<FX_CLK);

	PORTB |= (1<<FX_LAT);
	_delay_us(50);

	byte0 = PCFXRead();


	PORTB &= ~(1<<FX_LAT);

	reportBuffer->b2 = 0b10101010;
}

uchar PCFXRead()
{
	uchar	i, j = 0;

	for (i = 0; i < 8; i++)
	{
		PORTB &= ~(1<<FX_CLK);
		_delay_us(20);
		j = j >> 1;
		if (PINB & (1<<FX_DAT)) j |= (1<<7);
		PORTB |= (1<<FX_CLK);
		_delay_us(20);
	}
	
	return j;
}
