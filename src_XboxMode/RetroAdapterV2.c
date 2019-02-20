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

/* Button mapping:
    A:   bottom button (SNES B, PSX Cross, NegCon I, NES B, NeoGeo A, Genesis A, Gamecube A, N64 B, PCE II)
    B:   right button (SNES A, PSX Circle, NegCon A, NES A, Neogeo B, Genesis B, Gamecube X, N64 A, PCE I)
    X:   left button (SNES Y, PSX Square, NegCon II, NeoGeo C, Genesis C, Gamecube B, N64 Yellow Down, PCE III) 
    Y:   upper button (SNES X, PSX Triangle, NegCon B, NeoGeo D, Genesis X, Gamecube Y, N64 Yellow Up, PCE IV)
    L:   Left Shoulder (SNES L, PSX L1, NegCon L, Genesis Y, Gamecube L, N64 L, PCE V)
    R:   Right Shoulder (SNES R, PSX R1, NegCon R, Genesis Z, Gamecube R, N64 R, PCE VI)
    White:   Misc 1 (PSX L2, N64 Yellow Left, Saturn L)
    Black:  Misc 2 (PSX R2, N64 Yellow Right, Saturn R)
    Select:  Select
    Start:  Start (PC Engine Play, Genesis Mode)
    Left Thumb Click:  Misc 3 (PSX L3, N64 Z, Gamecube Z) 
    Right Thumb Click:  Misc 4 (PSX R3)
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

const uchar hat_lut[] PROGMEM  = { -1, 0, 4, -1, 6, 7, 5, -1, 2, 1, 3, -1, -1, -1, -1, -1 };
extern gamepad_state_t gamepad_state;

static	report_t		reportBuffer;
static	reportMouse_t	reportBufferMouse;
static  reportAnalogButtons_t	reportBufferAnalogButtons;

static const report_t emptyReportBuffer;
static const reportMouse_t emptyReportBufferMouse;
static const reportAnalogButtons_t emptyReportBufferAnalogButtons;

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
	cli(); //disable interrupts when reading controller
	
	uchar	skipdb9flag = 0;	// don't read DB9 when shared lines are in use by DB15
	uchar	pcinton	= 0;

	//reportBuffer.y = reportBuffer.x = reportBuffer.b1 = reportBuffer.b2 = 0;
	//reportBuffer.rx = reportBuffer.ry = 0;
	//reportBuffer.hat = 0;
	
	reportBuffer = emptyReportBuffer;
	reportBuffer.reportid = id;

	//reportBufferMouse.x = reportBufferMouse.y = reportBufferMouse.w = 0;
	//reportBufferMouse.b1 = 0;
	
	reportBufferMouse = emptyReportBufferMouse;
	reportBufferMouse.reportid = id;
	
	//reportBufferAnalogButtons.a = reportBufferAnalogButtons.b
	//= reportBufferAnalogButtons.x = reportBufferAnalogButtons.y 
	//= reportBufferAnalogButtons.l = reportBufferAnalogButtons.r
	//= reportBufferAnalogButtons.black = reportBufferAnalogButtons.white 
	//= 0x00;
	
	reportBufferAnalogButtons = emptyReportBufferAnalogButtons;
	
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
				ReadN64GC(&reportBuffer, &reportBufferAnalogButtons);
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
// This function below translates the report structure used by the Mojo RetroAdapterV2 sources into the gamepad_state structure 
// used by the XBox code from Bruno Freitas' RetroPad Adapter that is used here to implement Xbox Support
void UpdateGamePadState(report_t *reportBuffer, reportAnalogButtons_t *reportBufferAnalogButtons) {

		//Dpad
		((reportBuffer->hat & HAT_UP) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		((reportBuffer->hat & HAT_DOWN) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		((reportBuffer->hat & HAT_LEFT) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		((reportBuffer->hat & HAT_RIGHT) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);
		
		//Digital Buttons
		((reportBuffer->b2 & (1<<1)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START); // start
		((reportBuffer->b2 & (1<<0)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK); // back
		((reportBuffer->b2 & (1<<2)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_LEFT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_LEFT_STICK); // left stick press (aka L3)
		((reportBuffer->b2 & (1<<3)) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK); // right stick press (aka R3)
		
		//Analog buttons as digital Buttons
		gamepad_state.a = ((reportBuffer->b1 & (1<<0)) > 0) * 0xFF; //A bottom button
		gamepad_state.b = ((reportBuffer->b1 & (1<<1)) > 0) * 0xFF; //B right button	
		gamepad_state.x = ((reportBuffer->b1 & (1<<2)) > 0) * 0xFF; //X left button
		gamepad_state.y = ((reportBuffer->b1 & (1<<3)) > 0) * 0xFF; //Y top button
		
		gamepad_state.l = ((reportBuffer->b1 & (1<<6)) > 0) * 0xFF; // left shoulder
		gamepad_state.r = ((reportBuffer->b1 & (1<<7)) > 0) * 0xFF; // right shoulder
		gamepad_state.black = ((reportBuffer->b1 & (1<<5)) > 0) * 0xFF; //black (aka R2 on psx)
		gamepad_state.white = ((reportBuffer->b1 & (1<<4)) > 0) * 0xFF; //white (aka L2 on psx)

		//Analog buttons as true analog values, if non-zero
		if (reportBufferAnalogButtons->a != 0) gamepad_state.a = reportBufferAnalogButtons->a; //A analog
		if (reportBufferAnalogButtons->x != 0) gamepad_state.x = reportBufferAnalogButtons->x; //X analog	
		if (reportBufferAnalogButtons->y != 0) gamepad_state.y = reportBufferAnalogButtons->y; //Y analog
		if (reportBufferAnalogButtons->b != 0) gamepad_state.b = reportBufferAnalogButtons->b; //B analog
		if (reportBufferAnalogButtons->l != 0) gamepad_state.l = reportBufferAnalogButtons->l; //left shoulder analog		
		if (reportBufferAnalogButtons->r != 0) gamepad_state.r = reportBufferAnalogButtons->r; //right shoulder analog
		if (reportBufferAnalogButtons->black != 0) gamepad_state.black = reportBufferAnalogButtons->black; //black analog
		if (reportBufferAnalogButtons->white != 0) gamepad_state.white = reportBufferAnalogButtons->white; //white analog
		
		//Left analog stick
		gamepad_state.l_x = map(reportBuffer->x, -128, 127, -32768, 32767);
		gamepad_state.l_y = map(reportBuffer->y, -128, 127, 32767, -32768);

		//Right analog stick
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
	    UpdateGamePadState(&reportBuffer, &reportBufferAnalogButtons); //Translate from Mojo PC USB Controller button structure to Xbox structure 
		xbox_send_pad_state(); //From Bruno Freitas
		
		//usbPoll();
        //if(usbInterruptIsReady3()){
        //    /* called after every poll of the interrupt endpoint */
		//	xbox_reset_watchdog();
		//	ReadController(1);
		//	UpdateGamePadState(&reportBuffer, &reportBufferAnalogButtons); //Translate from Mojo PC USB Controller button structure to Xbox structure 
		//	usbSetInterrupt3((unsigned char *) &gamepad_state, 20);
		//}
    }
    return 0;
}

/* ------------------------------------------------------------------------- */
