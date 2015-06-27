#include <avr/io.h>
#include "report.h"
#include "dreamcast.h"

//	B5	-	data 0
//	B4	-	data 1
//	B3	-	sense

extern	void dreamcast(report_t *reportBuffer);

volatile uchar	dcbuffer[9];

void ReadDreamcast(report_t *reportBuffer)
{
	DDRB	&= ~(1<<DCS);					// sense as input
	DDRB	|= (1<<DC0)|(1<<DC1);			// d0/d1 as outputs
	PORTB	|= (1<<DC0)|(1<<DC1)|(1<<DCS);	// sense pull-up, d0/d1 start high

	dreamcast(reportBuffer);
}
