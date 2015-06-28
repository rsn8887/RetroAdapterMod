/* Retro Adapter V2
 * 
 * Target: ATTiny461 16MHz
 *
 * Parts (c) 2009 MoJo aka Paul Qureshi
 * Parts (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2
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
#include "dualdb9.h"
#include "timers.h"
#include "amiga_mouse.h"
#include "sega_paddle.h"
#include "3do.h"
#include "cd32.h"
#include "pc-fx.h"
#include "dreamcast.h"

static	void*			usbDeviceDescriptorAddress;
static	int				usbDeviceDescriptorLength;
static	report_t		reportBuffer;
static	reportMouse_t	reportBufferMouse;
static 	reportWheel_t	reportBufferWheel;
void*	reportBufferAddress;
uchar	reportBufferLength;
uchar	hidMode;
void*	hidReportDescriptorAddress;
int		hidReportDescriptorLength;
uchar	hidNumReports;
uchar	idleRate;

const uchar hat_lut[] PROGMEM  = { -1, 0, 4, -1, 6, 7, 5, -1, 2, 1, 3, -1, -1, -1, -1, -1 };

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
	uchar	skipdb9flag = 0;	// don't read DB9 when shared lines are in use by DB15
	uchar	pcinton	= 0;

	reportBuffer.y = reportBuffer.x = reportBuffer.b1 = reportBuffer.b2 = 0;
	reportBuffer.rx = reportBuffer.ry = 0;
	reportBuffer.hat = -1;
	reportBuffer.reportid = id;

	reportBufferMouse.x = reportBufferMouse.y = reportBufferMouse.w = 0;
	reportBufferMouse.b1 = 0;
	reportBufferMouse.reportid = id;

	reportBufferWheel.x = reportBufferWheel.y = 0;
	reportBufferWheel.slider1 = reportBufferWheel.slider2 = reportBufferWheel.slider3 = 0;
	reportBufferWheel.b1 = reportBufferWheel.b2 = 0;
	reportBufferWheel.hat = -1;
	reportBufferWheel.reportid = id;
	
	hidMode = HIDM_1P;
	//ReadDreamcast(&reportBuffer);

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
				ReadPSX(&reportBuffer, &reportBufferWheel);
				skipdb9flag = 1;
				break;
				
				case ((1<<1)|(1<<2)):		// LLHH
				ReadPCE(&reportBuffer);
				skipdb9flag = 1;
				break;
				
				case (1<<2):				// LLHL
				ReadPCFX(&reportBuffer);
				break;

				case ((1<<2)|(1<<3)):		// LHHL
				hidMode = HIDM_MOUSE;
				pcinton = 1;
				if (id == 1)
				{
					ReadAmigaMouse(&reportBufferMouse);
					skipdb9flag = 1;
				}
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
			else
			{
				if (id == 2)
				{
					ReadDualDB9(&reportBuffer);		// Key 1 = H, DB15-10 = H, 2P mode
					skipdb9flag = 1;
				}
				hidMode = HIDM_2P;
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
				ReadFamicom(&reportBuffer, &reportBufferMouse);
			}
		}
		else
		{
			ReadDB9(&reportBuffer);
		}
	}

	// Mouse mode joystick support
	if ((id == 2) & (hidMode == HIDM_MOUSE))
	{
		reportBufferMouse.b1 = reportBuffer.x;
		reportBufferMouse.x = reportBuffer.y;
		reportBufferMouse.y = reportBuffer.b1;
		reportBufferMouse.w = reportBuffer.b2;
	}

	if (!pcinton) PCICR	&= ~(1<<PCIE0);
}

/* ------------------------------------------------------------------------- */

void SetHIDMode()
{
	//uchar	i;

	switch(hidMode)
	{
		case HIDM_1P:
			usbDeviceDescriptorAddress = usbDescriptorDeviceJoystick;
			usbDeviceDescriptorLength = sizeof(usbDescriptorDeviceJoystick);
			hidReportDescriptorAddress = usbHidReportDescriptor1P;
			hidReportDescriptorLength = usbHidReportDescriptor1PLength;
			hidNumReports = 1;
			reportBufferAddress = &reportBuffer;
			reportBufferLength = sizeof(reportBuffer);
			break;
		case HIDM_2P:
			usbDeviceDescriptorAddress = usbDescriptorDeviceJoystick;
			usbDeviceDescriptorLength = sizeof(usbDescriptorDeviceJoystick);
			hidReportDescriptorAddress = usbHidReportDescriptor2P;
			hidReportDescriptorLength = usbHidReportDescriptor2PLength;
			hidNumReports = 2;
			reportBufferAddress = &reportBuffer;
			reportBufferLength = sizeof(reportBuffer);
			break;
		case HIDM_MOUSE:
			usbDeviceDescriptorAddress = usbDescriptorDeviceMouse;
			usbDeviceDescriptorLength = sizeof(usbDescriptorDeviceMouse);
			hidReportDescriptorAddress = usbHidReportDescriptorMouse;
			hidReportDescriptorLength = usbHidReportDescriptorMouseLength;
			hidNumReports = 2;
			reportBufferAddress = &reportBufferMouse;
			reportBufferLength = sizeof(reportBufferMouse);
			break;
		case HIDM_WHEEL:
			usbDeviceDescriptorAddress = usbDescriptorDeviceJoystick;
			usbDeviceDescriptorLength = sizeof(usbDescriptorDeviceJoystick);
			hidReportDescriptorAddress = usbHidReportDescriptorWheel;
			hidReportDescriptorLength = usbHidReportDescriptorWheelLength;
			hidNumReports = 1;
			reportBufferAddress = &reportBufferWheel;
			reportBufferLength = sizeof(reportBufferWheel);
			break;		
	}
	usbDescriptorConfiguration[25] = hidReportDescriptorLength;

	cli();						// disable interrupts
    usbDeviceDisconnect();
	DDRD |= (1<<1) | (1<<2);	// USB reset

	_delay_ms(255);				// disconnect for >250ms

    usbDeviceConnect();
	DDRD &= ~((1<<1) | (1<<2));	// clear reset
	sei();						// restart interrupts
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
            usbMsgPtr = reportBufferAddress; //(void *)&reportBuffer;
            return reportBufferLength; //sizeof(reportBuffer);
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

/* ------------------------------------------------------------------------- */

uchar	usbFunctionDescriptor(struct usbRequest *rq)
{
	if (rq->bRequest == USBRQ_GET_DESCRIPTOR)
	{
		// USB spec 9.4.3, high byte is descriptor type
		switch (rq->wValue.bytes[1])
		{
			case USBDESCR_HID_REPORT:
				usbMsgPtr = (void*)hidReportDescriptorAddress;
				return hidReportDescriptorLength;
			case USBDESCR_CONFIG:
				usbMsgPtr = (void*)usbDescriptorConfiguration;
				return sizeof(usbDescriptorConfiguration);
			case USBDESCR_DEVICE:
				usbMsgPtr = usbDeviceDescriptorAddress;
				return usbDeviceDescriptorLength;
		}
	}

	return 0;
}

/* ------------------------------------------------------------------------- */

int main(void)
{
	uchar   i = 1;
	uchar	hidCurrentMode = 255;
	char remainingData=0;
	uchar offset=0;

	HardwareInit();
	usbInit();

	// Set up descriptor
	hidMode = HIDM_1P;
	ReadController(1);
	SetHIDMode();

    for(;;){                /* main event loop */
        usbPoll();
        if(usbInterruptIsReady()){
            /* called after every poll of the interrupt endpoint */
			ReadController(i);
			
			remainingData=reportBufferLength;
			offset=0;
			// handle report with more than 8 byte length (for NegCon and future expansion)
			do {
				if (remainingData<=8) {
					usbSetInterrupt(reportBufferAddress+offset, remainingData);
					remainingData=0;
				}
				else {	
					usbSetInterrupt(reportBufferAddress+offset, 8);				
					offset+=8;
					remainingData-=8;
					do {
						usbPoll();
					} while (!usbInterruptIsReady());	
				}
			} while (remainingData>0);				
				
			i++;
			if (i > hidNumReports) i = 1;
			if (hidCurrentMode != hidMode)
			{
				SetHIDMode();
				hidCurrentMode = hidMode;
			}
        }
    }

    return 0;
}

/* ------------------------------------------------------------------------- */
