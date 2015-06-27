#include <avr/io.h>
#include <avr/pgmspace.h>
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

const uchar n64gc_hat_lut[] PROGMEM  = { -1, 2, 6, -1, 4, 3, 5, -1, 0, 1, 7, -1, -1 };

extern	uchar readnintendo(report_t *reportBuffer);

void ReadN64GC(report_t *reportBuffer)
{
	uchar	dpad;

	PORTB	&= ~(1<<0);		// data bus no pull-up
	DDRB	&= ~(1<<0);		// data bus input

	PORTD	&= ~(1<<7);		// debug
	DDRD	|= (1<<7);

	dpad = readnintendo(reportBuffer);
	reportBuffer->hat = pgm_read_byte(&n64gc_hat_lut[dpad]);
}
