/* Retro Adapter V2
 Xbox Mod V0.1* 
 * Target: ATTiny461 16MHz
 *

 * Parts (c) 2009 MoJo aka Paul Qureshi
 * Parts (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2
 

 

 * NOTE by rsn8887:

 * Modified 06/26/2015 by rsn8887 to replace PC compatibility with XBOX Classic compatibility

 * This version of the source only works on Xbox Classic

 * I used the Xbox communication and initialization code from Bruno Freitas' Retropad Adapter and merged it into 

 * this codebase from Paul Qureshi

 * See http://www.brunofreitas.com/node/41 for more information on Bruno Freitas' Adapter

 * I also updated the makefile and changed some of the button mappings to better reflect operation on Xbox

 * SNES mouse and dual player support was removed to comply with the limitations of the XBox classic

 * If you want to use a mouse, just hook up an original Qureshi RetroAdapter to your Xbox, because Xbox already supports standard usb mice

 * If you want 2-4 player support, just hook up multiple retroadapters to the xbox, one player per port

*/


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


//#include <WProgram.h>


#if defined (__cplusplus)
extern "C" {
#endif

#include "usbdrv.h"

#if defined (__cplusplus)
}
#endif

#include "XBOXPad.h"


#include <avr/io.h>


#include <avr/interrupt.h>  /* for sei() */


#include <util/delay.h>     /* for _delay_ms() */



#include <avr/pgmspace.h>   /* required by usbdrv.h */

#include "report.h"

//#include "descriptors.h"

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


extern const uchar hat_lut[] PROGMEM  = { -1, 0, 4, -1, 6, 7, 5, -1, 2, 1, 3, -1, -1, -1, -1, -1 };




//static	void*			usbDeviceDescriptorAddress;

//static	int				usbDeviceDescriptorLength;

static	report_t		reportBuffer;

static	reportMouse_t	reportBufferMouse;

void*	reportBufferAddress;

uchar	reportBufferLength;

uchar	hidMode;

//void*	hidReportDescriptorAddress;

//int		hidReportDescriptorLength;

uchar	hidNumReports;

uchar	idleRate;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



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

void setup() {
	// Initialize USB joystick driver
	xbox_init(true);

	// Delay for waiting XBOX pad to be detected...
	while(!xbox_pad_detected()) {
		xbox_reset_watchdog();
		//delayMicroseconds(10000); // 10ms delay
		_delay_ms(10);
		xbox_send_pad_state();
	}
}


/* ------------------------------------------------------------------------- */



void ReadController(uchar id)

{

	uchar	skipdb9flag = 0;	// don't read DB9 when shared lines are in use by DB15

	uchar	pcinton	= 0;



	reportBuffer.y = reportBuffer.x = reportBuffer.b1 = reportBuffer.b2 = 0;

	reportBuffer.rx = reportBuffer.ry = 0;

	reportBuffer.hat = 0;

	reportBuffer.reportid = id;



	reportBufferMouse.x = reportBufferMouse.y = reportBufferMouse.w = 0;

	reportBufferMouse.b1 = 0;

	reportBufferMouse.reportid = id;



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

}
// This function below translates the report structure used by the Mojo RetroAdapterV2 sources into the gamepad_state structure 
// used by the XBox code from Bruno Freitas' RetroPad Adapter that is used here to implement Xbox Support
void UpdateGamePadState(report_t *reportBuffer) {
		((reportBuffer->hat & HAT_UP) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		((reportBuffer->hat & HAT_DOWN) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		((reportBuffer->hat & HAT_LEFT) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		((reportBuffer->hat & HAT_RIGHT) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.a = ((reportBuffer->b1 & (1<<0)) > 0) * 0xFF; //A bottom button
		gamepad_state.x = ((reportBuffer->b1 & (1<<1)) > 0) * 0xFF; //X left button	
		gamepad_state.y = ((reportBuffer->b1 & (1<<2)) > 0) * 0xFF; //Y top button
		gamepad_state.b = ((reportBuffer->b1 & (1<<3)) > 0) * 0xFF; //B right button
		
		gamepad_state.l = ((reportBuffer->b1 & (1<<4)) > 0) * 0xFF; // left shoulder
		gamepad_state.r = ((reportBuffer->b1 & (1<<5)) > 0) * 0xFF; // right shoulder
		gamepad_state.black = ((reportBuffer->b1 & (1<<6)) > 0) * 0xFF; //black (aka L2)
		gamepad_state.white = ((reportBuffer->b1 & (1<<7)) > 0) * 0xFF; //white (aka R2)

		((reportBuffer->b2 & (1<<3)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START); // start
		((reportBuffer->b2 & (1<<2)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK); // back
		((reportBuffer->b2 & (1<<0)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_LEFT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_LEFT_STICK); // left stick press (aka L3)
		((reportBuffer->b2 & (1<<1)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK); // right stick press (aka R3)
		
		//lx = (reportBuffer.x >= 128) ? button_data[2] - 128 : button_data[2] + 128);
		//ly = ~((button_data[3] >= 128) ? button_data[3] - 128 : button_data[3] + 128);

		gamepad_state.l_x = map(reportBuffer->x, -128, 127, -32768, 32767);
		gamepad_state.l_y = map(reportBuffer->y, -128, 127, 32767, -32768);

		gamepad_state.r_x = map(reportBuffer->rx, -128, 127, -32768, 32767);
		gamepad_state.r_y = map(reportBuffer->ry, -128, 127, 32767, -32768);
}


int main(void)
{


	HardwareInit();

	//init();
	setup(); // From Bruno Freitas
	

	ReadController(1);



    for(;;){                /* main event loop */
        xbox_reset_watchdog(); //From Bruno Freitas
		ReadController(1);
        UpdateGamePadState(&reportBuffer); //Translate from Mojo PC USB Controller button structure to Xbox structure 
		xbox_send_pad_state(); //From Bruno Freitas
		//usbPoll();
        //if(usbInterruptIsReady()){
        //    /* called after every poll of the interrupt endpoint */
		//	ReadController(i);

        //    usbSetInterrupt(reportBufferAddress, reportBufferLength);

		//	i++;

		//	if (i > hidNumReports) i = 1;

		//	if (hidCurrentMode != hidMode)

		//	{

		//		SetHIDMode();

		//		hidCurrentMode = hidMode;

		//	}
    }


    return 0;
}

/* ------------------------------------------------------------------------- */
