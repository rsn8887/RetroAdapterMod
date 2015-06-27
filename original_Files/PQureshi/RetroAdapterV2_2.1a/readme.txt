                             Retro Adapter V2 2.0
                                       
                         Copyright 2010 Paul Qureshi
                                       
							   http://keio.dk
                           http://retro.world3.net

All code, schematics, PCB designs, firmware and other files included with this 
archive are licensed under the GNU Public License V3, see license.txt for more 
details. 



1. Introduction
=============== 

The Retro Adapter lets you connect almost any game controller to USB. Play 
emulators with the original controllers, or use your favorite controller in
modern games. 

Fully tested with Windows, Linux*, MacOS and the Playstation 3.

* For Linux you need kernel version 2.6.35-RC4 or later to support 2 Player
mode.



2. Supported controllers
========================

Game controllers:

- 3DO gamepads
- Amstrad CPC464, CPC6128 and CPC6128 Plus Digital Joysticks
- Atari 2600 joystick
- Atari 2600 Driving controller
- Atari 2600 Paddles
- Atari 400 / 800 / 1200XL / 600XL / 800XL / 65XE / 130XE / 800XE / ST
- BBC Micro 2x Analogue (standard) and single Voltmace 14B joysticks
- Commodore VIC-20 / MAX / C64 / Amiga
- Commodore C16 / C116 / Plus4 Joystick
- Commodore CD32
- Dragon 32 / 64
- MSX Joystick
- NEC PC-Engine gamepad
- Neo Geo joystick / gamepad
- Nintendo Famicom / NES
- Nintendo Gamecube
- Nintendo N64
- Nintendo Super Famicom / SNES
- Nintendo Virtual Boy
- PC Gameport
- Sega Master System / SG 1000 / SG 1000 II / Mark III
- Sega MegaDrive (Genesis) (3 button and 6 button)
- Sega Saturn standard 6 button pad / arcade stick
- Sega Saturn Analogue (NiGHTS) Controller
- Sinclair ZX Spectrum Plus3 Joystick
- Sony Playstation 1 (PSX) original / Dual Shock / arcade stick / dance mat
- Sony Playstation 2 (PS2) Dual Shock 2 / arcade stick / dance mat
- Sony Playstation Densha de Go! (train) controller
- Tandy TRS-80 CoCo (Colour Computer)
- TurboGrafx-16 gamepad
- X68000 Joystick


Mice:

- Amiga 2/3 button mouse
- Nintendo Famicom tackball
- Nintendo SNES mouse



3. Hardware
===========

See included schematic in PNG or Eagle format. PCB layout is also included in 
PNG (600DPI) and Eagle format. 



4. Firmware
===========

Firmware source code for AVR Studio and compiled .hex file are included.



5. History
==========

2.1a	- Fixed NES/SNES/N64/GC 2 player support
		- Included combined.hex in archive

2.1		- Changed Mouse Mode to support a joystick simultaniously for
		  computer emulators
		- Reduced polling interval from 10ms to 8ms (120Hz or 2x 60Hz)
		- Nintendo Virtual Boy support
		- SNES mouse support
		- BBC Micro Voltmace button mapping changed so main red button is 1
		  and others start from 2.
		- Fixed N64 and Gamecube controllers (broken in 2.0)

2.0		- Firmware 2.0 released
		- 2 Player Mode now works are two separate joysticks for improved emulator support
		- Mouse support added, Amiga mice supported
		- Famicom trackball supported
		- Sega Arkanoid (paddle) controller supported
		- 3D0 gamepads supported
		- CD32 gamepads supported
		- Japanese Famicom Connector added
		- BBC joystick support improved
		- BBC Voltmace button mapping fixed
		- Compatibility improvement for Playstation controllers
		- Button mappings fixed
		- NES support improved
		- Atari Driving Controller improved, works with MAME/Arkanoid
		- Various bug fixes

1.4		- Added support for 555 timer + potentiometer based controllers such
		  as Atari Paddles
		- Improved compatibility of Sega Megadrive 6 button pads
		- Fixed a couple of minor bugs

1.3     - Added support for 2P Connector
		- Changed button mapping to be more consistent
		- Various bug fixes
		- Fixed updater script

1.2     - Added support for analogue joysticks including BBC Micro standard and
		  Voltmace 14 button controllers

1.1     - Better support for Atari Driving controllers, more compatible with
		  emulators

1.0		- Fixed N64 connection, was polling controller too fast
		- Added fuse settings to firmware directory

0.9		- First release
		- Atari / Commodore 9 pin joysticks working
		- PC-Engine / TurboGrafx gamepads working
		- Neo Geo joysticks / gamepads working
		- Nintendo 64 / Gamecube controllers working
		- Nintendo NES / SNES gamepads working
		- Sega 8 and 16 bit controllers working
		- Sega Saturn controllers working
		- Spectrum joysticks working
		- Sony Playstation controllers working
