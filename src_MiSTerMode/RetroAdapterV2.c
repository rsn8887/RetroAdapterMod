/* Retro Adapter V2
 * 
 * Target: ATTiny461 16MHz
 *
 * Parts (c) 2009 MoJo aka Paul Qureshi
 * Parts (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2
 */

/* Button mapping (used to conform with Android and Windows simultaneously):
    button 1:   bottom button (SNES B, PSX Cross, NegCon B, NES B, NeoGeo A, Genesis A, Gamecube A, N64 B, PCE II)
    button 2:   right button (SNES A, PSX Circle, NegCon A, NES A, Neogeo B, Genesis B, Gamecube X, N64 A, PCE I)
    button 4:   left button (SNES Y, PSX Square, NeoGeo C, Genesis C, Gamecube B, N64 Yellow Down, PCE III), 
    button 5:   upper button (SNES X, PSX Triangle, NeoGeo D, Genesis X, Gamecube Y, N64 Yellow Up, PCE IV)
    button 7:   Left Shoulder (SNES L, PSX L1, Genesis Y, Gamecube L, N64 L, PCE V)
    button 8:   Right Shoulder (SNES R, PSX R1 Genesis Z, Gamecube R, N64 R, PCE VI)
    button 9:   Misc 1 (PSX L2, N64 Yellow Left, Saturn L), 
    button 10:  Misc 2 (PSX R2, N64 Yellow Right, Saturn R),
    button 11:  Select
    button 12:  Start (PC Engine Play, Genesis Mode)
    button 14:  Misc 3 (PSX L3, N64 Z, Gamecube Z) 
    button 15:  Misc 4 (PSX R3)

    Analog:
    Left Joystick: x,y
    Right Joystick: z,Rz
    
    NegCon mode:
    Steering: x
    Button I: button 1 analog-to-digital (bottom button)
    Button II: Rz up (right stick up)
    Button L: button 7 analog-to-digital (left shoulder)
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
static	reportMouse_t		reportBufferMouse;
static  reportAnalogButtons_t	reportBufferAnalogButtons;

static const report_t emptyReportBuffer = {
	0,	// reportid
	0,	// x
	0,	// y
	0,	// rx
	0,	// ry
	-1,	// hat
	0,	// b1
	0	// b2
};

static const reportMouse_t emptyReportBufferMouse;

static const reportAnalogButtons_t emptyReportBufferAnalogButtons = {
	0,		// reportid
	0,	// x
	0,	// y
	0,	// rx
	0,	// ry
	-1,		// hat
	0,		// b1
	0,		// b2
	0,		// slider1
	0,		// slider2
	0,		// slider3
	0,		// slider4
	0,		// slider5
	0,		// slider6
	0,		// slider7
	0,		// slider8
	0,		// slider9
	0,		// slider10
	0,		// slider11
	0		// slider12
};
	
void*	reportBufferAddress;
uchar	reportBufferLength;
uchar	hidMode;
void*	hidReportDescriptorAddress;
int		hidReportDescriptorLength;
void*	usbDescriptorStringDeviceAddress;
int		usbDescriptorStringDeviceLength;
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
	cli(); // disable interrupts when reading controller
	
	uchar	skipdb9flag = 0;	// don't read DB9 when shared lines are in use by DB15
	uchar	pcinton	= 0;
	
	reportBuffer = emptyReportBuffer;
	reportBuffer.reportid = id;
	
	reportBufferMouse = emptyReportBufferMouse;
	reportBufferMouse.reportid = id;

	reportBufferAnalogButtons = emptyReportBufferAnalogButtons;
	reportBufferAnalogButtons.reportid = id;
	
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
				ReadPSX(&reportBuffer, &reportBufferAnalogButtons);
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
	
	sei(); // re-enable interrupts after reading controller
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
			hidReportDescriptorLength = sizeof(usbHidReportDescriptor1P);
			hidNumReports = 1;
			reportBufferAddress = &reportBuffer;
			reportBufferLength = sizeof(reportBuffer);
			usbDescriptorStringDeviceAddress = usbDescriptorStringDeviceDefault;
			usbDescriptorStringDeviceLength = sizeof(usbDescriptorStringDeviceDefault);
			break;
		case HIDM_2P:
			usbDeviceDescriptorAddress = usbDescriptorDeviceJoystick;
			usbDeviceDescriptorLength = sizeof(usbDescriptorDeviceJoystick);
			hidReportDescriptorAddress = usbHidReportDescriptor2P;
			hidReportDescriptorLength = sizeof(usbHidReportDescriptor2P);
			hidNumReports = 2;
			reportBufferAddress = &reportBuffer;
			reportBufferLength = sizeof(reportBuffer);
			usbDescriptorStringDeviceAddress = usbDescriptorStringDeviceDefault;
			usbDescriptorStringDeviceLength = sizeof(usbDescriptorStringDeviceDefault);
			break;
		case HIDM_MOUSE:
			usbDeviceDescriptorAddress = usbDescriptorDeviceMouse;
			usbDeviceDescriptorLength = sizeof(usbDescriptorDeviceMouse);
			hidReportDescriptorAddress = usbHidReportDescriptorMouse;
			hidReportDescriptorLength = sizeof(usbHidReportDescriptorMouse);
			hidNumReports = 2;
			reportBufferAddress = &reportBufferMouse;
			reportBufferLength = sizeof(reportBufferMouse);
			usbDescriptorStringDeviceAddress = usbDescriptorStringDeviceDefault;
			usbDescriptorStringDeviceLength = sizeof(usbDescriptorStringDeviceDefault);
			break;
		case HIDM_ANALOGBUTTONS:
			usbDeviceDescriptorAddress = usbDescriptorDeviceJoystick;
			usbDeviceDescriptorLength = sizeof(usbDescriptorDeviceJoystick);
			hidReportDescriptorAddress = usbHidReportDescriptorAnalogButtons;
			hidReportDescriptorLength = sizeof(usbHidReportDescriptorAnalogButtons);
			hidNumReports = 1;
			reportBufferAddress = &reportBufferAnalogButtons;
			reportBufferLength = sizeof(reportBufferAnalogButtons);
			usbDescriptorStringDeviceAddress = usbDescriptorStringDeviceDefault;
			usbDescriptorStringDeviceLength = sizeof(usbDescriptorStringDeviceDefault);
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
			case USBDESCR_STRING:
				switch (rq->wValue.bytes[0])
				{
					case 2:
						usbMsgPtr = (void*)usbDescriptorStringDeviceAddress;
						return usbDescriptorStringDeviceLength;
				}
		}
	}

	return 0;
}

/* ------------------------------------------------------------------------- */
void RemapController(char *x, char *y, char *rx, char *ry, uchar *b1, uchar *b2)
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
	
Mapping required by Android:
	buttons in parentheses are non-standard, but seem to be supported in android
	bit 0	button 1: 	Android A (bottom)
	bit 1	button 2: 	Android B (right)
	bit 2	button 3: 	(Android C)
	bit 3	button 4: 	Android X (left)
	bit 4	button 5: 	Android Y (top)
	bit 5	button 6: 	(Android Z)
	bit 6	button 7: 	Android L1 
	bit 7	button 8: 	Android R1 
	bit 0	button 9: 	Android L2 
	bit 1	button 10: 	Android R2
	bit 2	button 11: 	(Android Select) (Select)
	bit 3	button 12: 	(Android Start) (Start)
	bit 4	button 13:	??
	bit 5	button 14: 	Android Left Thumb Stick Press
	bit 6	button 15: 	Android Right Thumb Stick Press
	bit 7	button 16: 	??
*/
	*x+=128;
	*y+=128;
	*rx+=128;
	*ry+=128;

	// So we have to map b1 bit 2 to 3 etc to conform with android
	if ((*b1 | 0x00) | (*b2 | 0x00))
	{	
			uchar oldb1 = *b1;
			uchar oldb2 = *b2;
			*b1=0;
			*b2=0;
			
			if (oldb1 & (1<<0)) *b1 |= (1<<0); // bottom
			if (oldb1 & (1<<1)) *b1 |= (1<<1); // right
			if (oldb1 & (1<<2)) *b1 |= (1<<3); // left
			if (oldb1 & (1<<3)) *b1 |= (1<<4); // top
			if (oldb1 & (1<<4)) *b2 |= (1<<0); // L2
			if (oldb1 & (1<<5)) *b2 |= (1<<1); // R2
			if (oldb1 & (1<<6)) *b1 |= (1<<6); // L1
			if (oldb1 & (1<<7)) *b1 |= (1<<7); // R1
			
			if (oldb2 & (1<<0)) *b2 |= (1<<2); // Select 
			if (oldb2 & (1<<1)) *b2 |= (1<<3); // Start
			if (oldb2 & (1<<2)) *b2 |= (1<<5); // L3
			if (oldb2 & (1<<3)) *b2 |= (1<<6); // R3
			if (oldb2 & (1<<4)) *b2 |= (1<<4); // 
			if (oldb2 & (1<<5)) *b2 |= (1<<7); // 
			if (oldb2 & (1<<6)) *b1 |= (1<<2); // 
			if (oldb2 & (1<<7)) *b1 |= (1<<5); // 
	}
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

//	uchar j = 1; //for speed test only
	
    for(;;){                /* main event loop */
        usbPoll();
        if(usbInterruptIsReady()){
            /* called after every poll of the interrupt endpoint */
			ReadController(i);
			
			switch (hidCurrentMode)
			{
				case HIDM_1P:
					RemapController(&(reportBuffer.x), &(reportBuffer.y), 
						&(reportBuffer.rx), &(reportBuffer.ry),
						&(reportBuffer.b1), &(reportBuffer.b2));
					break;
				case HIDM_2P:
					RemapController(&(reportBuffer.x), &(reportBuffer.y), 
						&(reportBuffer.rx), &(reportBuffer.ry),
						&(reportBuffer.b1), &(reportBuffer.b2));
					break;
			}

			remainingData=reportBufferLength;
			offset=0;

// For speed test, uncommnent the next three lines and the line "uchar j=0" above
//			reportBuffer.x=(j%4)*10; //for speed test only
//			j++; //for speed test only
		
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
