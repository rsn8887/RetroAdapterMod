/* Retro Adapter V2
 *
 * Target: ATTiny461 16MHz
 *
 * Parts (c) 2009 MoJo aka Paul Qureshi
 * Parts (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2
 */

/*
Updated bitmap used in all current controller subroutines:
b1	bit 0	bottom button
	bit 1	right button
	bit 2	left button
	bit 3	upper button
	bit 4	L2
	bit 5	R2
	bit 6	L1
	bit 7	R1
b2	bit 0	Select
	bit 1	Start
	bit 2	L3
	bit 3	R3
*/

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */

#include "usbdrv.h"
#include "report.h"
#include "descriptors.h"
#include "hid_modes.h"

#include "direct.h"
#include "saturn.h"
#include "psx.h"
#include "famicom.h"
#include "pc-engine.h"

#include "dreamcast.h"
#include "n64gc.h"
#include "atari_driving.h"
#include "analogue.h"
#include "timers.h"
#include "sega_paddle.h"
#include "3do.h"
#include "cd32.h"
#include "pc-fx.h"
#include "dreamcast.h"

static report_t reportBuffer;

static const report_t emptyReportBuffer = {
	0,	// b1
	0,	// b2
	0,	// hat
	0,	// x
	0,	// y
	0,	// rx
	0,	// ry
	0   // padding
};

uchar	idleRate;

const uchar hat_lut[] PROGMEM  = { -1, 0, 2, 1, 4, -1, 3, -1, 6, 7, -1, -1, 5, -1, -1, -1, -1 };

/* ------------------------------------------------------------------------- */

void HardwareInit()
{
	// See schmatic for connections

	DDRD	= 0b00000000;
	PORTD	= 0b11111001;	// All inputs with pull-ups except USB D+/D-

	DDRB	= 0b00000000;
	PORTB	= 0b00111111;	// All inputs with pull-ups except xtal

	DDRC	= 0b00000000;
	PORTC	= 0b00111111;	// All inputs except unused bits
}

/* ------------------------------------------------------------------------- */

void ReadController(uchar id)
{
	cli(); // disable interrupts when reading controller

	uchar	skipdb9flag = 0;	// don't read DB9 when shared lines are in use by DB15
	uchar	pcinton	= 0;

	reportBuffer = emptyReportBuffer;

	// Check 15 pin connector

	if (!(PINC & (1<<5)))			// Key 1 = L
	{
		if (!(PINC & (1<<4)))			// Key 2 = L
		{
			switch (PINC & 0b00011110)
			{
				case 0:						// LLLL
				ReadSaturn(&reportBuffer);
				skipdb9flag = 1;
				break;

				case (1<<1):				// LLLH
				ReadPSX(&reportBuffer);
				skipdb9flag = 1;
				break;

				case ((1<<1)|(1<<2)):		// LLHH
				ReadPCE(&reportBuffer);
				skipdb9flag = 1;
				break;

				case (1<<2):				// LLHL
				ReadPCFX(&reportBuffer);
				break;

				case ((1<<3)|(1<<1)):		// LHLH
				ReadCD32(&reportBuffer);
				skipdb9flag = 1;
				break;

				case (1<<3):				// LHLL
				switch (PINB & 0b110)
				{
					case 0b110:				// HH
					ReadAtariDriving(&reportBuffer);
					break;

					case 0b000:				// LL
					Read3DO(&reportBuffer);
					break;

					case 0b010:				// LH
					ReadSegaPaddle(&reportBuffer);
					break;
				}
				skipdb9flag = 1;
				break;
			}
		}
		else							// Key 2 = H
		{
			if (!(PINC & (1<<3)))
			{
				ReadTimers(&reportBuffer);		// Key 1 = H, DB15-10 = L, Timer based
				skipdb9flag = 1;
			}
		}
	}
	else if (!(PINC & (1<<3)))		// Key 1 = H, DB15-10 = L
	{
		ReadAnalogue(&reportBuffer, id);
		skipdb9flag = 1;
	}
	else ReadDB15(&reportBuffer);	// Key 1 = H, DB15-10 = H

	// Check 9 pin connector

	if (!skipdb9flag)
	{
		if ( (PIND & ((1<<0)|(1<<3))) == 0)		// UP/DN = 0
		{
			if (PIND & (1<<4))
			{
				// N64 / Gamecube
				ReadN64GC(&reportBuffer);
			}
			else
			{
				// Famicom
				ReadFamicom(&reportBuffer);
			}
		}
		else
		{
			ReadDB9(&reportBuffer);
		}
	}
	if (!pcinton) PCICR	&= ~(1<<PCIE0);

	sei(); //re-enable interrupts
}

/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    /* The following requests are never used. But since they are required by
     * the specification, we implement them in this example.
     */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){
		/* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){
			/* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
			//ReadJoystick(rq->wValue.bytes[0]);
            usbMsgPtr = (void *)&reportBuffer;
            return sizeof(reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];

        }
    }else{
        /* no vendor specific requests implemented */
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

void RemapController(char *x, char *y, char *rx, char *ry, uchar *b1, uchar *b2, uchar *hat)
{
/*
Updated mapping used in all current subroutines:
b1	bit 0	bottom button
	bit 1	right button
	bit 2	left button
	bit 3	upper button
	bit 4	L2
	bit 5	R2
	bit 6	L1
	bit 7	R1
b2	bit 0	Select
	bit 1	Start
	bit 2	L3
	bit 3	R3

HID	SWITCH button
0	Y (left button)
1	B (bottom button)
2	A (right button)
3	X (upper button)
4	L
5	R
6	ZL
7	ZR
8	Minus
9	Plus
10	L3
11	R3
12	Home
13	Capture
*/
	*x+=128;
	*y+=128;
	*rx+=128;
	*ry+=128;

	if (*hat != 0)
		*hat = pgm_read_byte(&hat_lut[(*hat & 0x0f)]);
	else
		*hat = -1;

	// So we have to re-map to conform with Switch
	if ((*b1 | 0x00) | (*b2 | 0x00))
	{
			uchar oldb1 = *b1;
			uchar oldb2 = *b2;
			*b1=0;
			*b2=0;

			if (oldb1 & (1<<0)) *b1 |= (1<<1); // bottom
			if (oldb1 & (1<<1)) *b1 |= (1<<2); // right
			if (oldb1 & (1<<2)) *b1 |= (1<<0); // left
			if (oldb1 & (1<<3)) *b1 |= (1<<3); // top
			if (oldb1 & (1<<4)) *b1 |= (1<<6); // L2
			if (oldb1 & (1<<5)) *b1 |= (1<<7); // R2
			if (oldb1 & (1<<6)) *b1 |= (1<<4); // L1
			if (oldb1 & (1<<7)) *b1 |= (1<<5); // R1

			if (oldb2 & (1<<0)) *b2 |= (1<<0); // Select
			if (oldb2 & (1<<1)) *b2 |= (1<<1); // Start
			if (oldb2 & (1<<2)) *b2 |= (1<<2); // L3
			if (oldb2 & (1<<3)) *b2 |= (1<<3); // R3
	}
}

/* ------------------------------------------------------------------------- */

int main(void)
{
	char remainingData=0;
	uchar offset=0;

	HardwareInit();
	usbInit();

//	uchar j = 1; //for speed test only

    for(;;){                /* main event loop */
        usbPoll();
        if(usbInterruptIsReady()){
            /* called after every poll of the interrupt endpoint */
			ReadController(1);

			RemapController(&(reportBuffer.x), &(reportBuffer.y),
				&(reportBuffer.rx), &(reportBuffer.ry),
				&(reportBuffer.b1), &(reportBuffer.b2), &(reportBuffer.hat));

			remainingData=sizeof(reportBuffer);
			offset=0;

// For speed test, uncommnent the next three lines and the line "uchar j=0" above
//			reportBuffer.x=(j%4)*10; //for speed test only
//			reportBufferNegCon.x=(j%4)*10; //for speed test only
//			j++; //for speed test only

			// handle report with more than 8 byte length (for NegCon and future expansion)
			do {
				if (remainingData<=8) {
					usbSetInterrupt((void *)&reportBuffer + offset, remainingData);
					remainingData=0;
				}
				else {
					usbSetInterrupt((void *)&reportBuffer + offset, 8);
					offset+=8;
					remainingData-=8;
					do {
						usbPoll();
					} while (!usbInterruptIsReady());
				}
			} while (remainingData>0);
        }
    }

    return 0;
}

/* ------------------------------------------------------------------------- */
