# RetroAdapterMod
Mod of Paul Qureshi's Mojo RetroAdapter Firmware by rsn8887

Firmware v2.1a Mod v1.4

----------
![Photo of Retroadapter](https://s24.postimg.org/al02825gl/Retro_Adapter_Mod.jpg)

Retro Adapter Mod is a firmware mod for Paul Qureshi's Mojo Retro Adapter based on original Atmega168 code by Paul Qureshi and Bruno Freitas.

With the adapter one can connect old controllers to PC. This firmware mod adds support for using your old controllers on the Xbox classic and Android host systems, as well as support for the NegCon controller.

The bin archive just includes the .hex files needed to update the firmware.
The src archive includes all sources, binaries, and the original code this mod is based on.

See included readme.txt for more info.

References
----------

License GPL (see License.txt)

- Original source for "USB Retropad Adapter" from Bruno Freitas acquired via
http://www.brunofreitas.com/node/41
(included for reference in folder original_Files)

- Original source for "Retro Adapter v2.1a" from Paul Qureshi acquired via
http://denki.world3.net/retro_v2.html
(incuded for reference in folder original_Files)

- This mod by rsn8887 [![PayPayl donate button](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=72LHAQPSSKDGE&lc=US&item_name=RetroAdapter%20Firmware%20MOD%20Donation&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted "Donate once-off to this project using Paypal")

What is it?
-----------

- Included are four modified versions of the original Mojo Retroadapter firmware, as hex and source files. There's a version for Xbox, one for arcade cabinets, one for PC and Android, and one for Switch

- All four versions include the NegCon controller support.

- Xbox mode:
 - A RetroAdapter updated with this version of the modded Firmware only works on Xbox Classic (note: you can always go back after modding using the adapter's USB update feature)
 - I used the Xbox communication and initialization code from Bruno Freitas' Retropad Adapter and merged it into the codebase from Paul Qureshi
 - See http://www.brunofreitas.com/node/41 for more information on Bruno Freitas' adapter
 - SNES mouse and dual player support was removed to comply with the limitations of the XBox classic
 - For 2-4 player support, just hook up multiple retroadapters to the Xbox, one player per port

- Switch mode:
 - This is for use on Nintendo Switch. Note the Switch can support up to three simultaneous wired adapters running this version of the firmware (using a USB hub).

- Normal mode:
 - This is for use on PC and Android. In Android, the original RetroAdapter was not reporting directions up and left for NES and some other controller types.

- Arcade mode:
 - This version is suited for use in an arcade cabinet. It works on PCs just like the original RetroAdapter, but I replaced Neogeo stick support on the DB15 with an arcade mode that has just 8 firebuttons on the DB15, to give you a total of 10 buttons and 4 directions when you hook up your Arcade controls to pins on both DB9 and DB15.

- Button config:

Windows Host (Andr.) | XBox Host | Switch Host | Description    | SNES   | PSX     | NegCon | NES    | NeoGeo | Genesis | Gamecube | N64       | PCE    | Saturn
-------------------- | --------- | ----------- | -------------- | ------ | ------- | ------ | ------ | ------ | ------- | -------- | --------- | ------ | ------  
Button 1 (A)         | A         | B           | Bottom button  | B      | Cross   | I      | B      | A      | A       | A        | B         | II     | A
Button 2 (B)         | B         | A           | Right button   | A      | Circle  | A      | A      | B      | B       | X        | A         | I      | B
Button 4 (X)         | X         | Y           | Left button    | Y      | Square  | II     | -      | C      | C       | B        | Y. Down   | III    | C
Button 5 (Y)         | Y         | X           | Upper button   | X      | Triangle| B      | -      | D      | X       | Y        | Y. Up     | IV     | X
Button 7 (L)         | L         | L           | Left shoulder  | L      | L1      | L      | -      | -      | Y       | L        | L         | V      | Y
Button 8 (R)         | R         | R           | Right shoulder | R      | R1      | R      | -      | -      | Z       | R        | R         | VI     | Z
Button 9 (L2)        | White     | ZL          | Misc 1         | -      | L2      | -      | -      | -      | -       | -        | Y. Left   | -      | L
Button 10 (R2)       | Black     | ZR          | Misc 2         | -      | R2      | -      | -      | -      | -       | -        | Y. Right  | -      | R
Button 11 (Select)   | Select    | Minus       | Select         | Select | Select  | -      | Select | Select | -       | -        | -         | Select | -
Button 12 (Start)    | Start     | Start       | Start          | Start  | Start   | -      | Start  | Start  | Start   | Start    | Start     | Play   | Start
Button 14 (L. Thumb) | L. Thumb  | L. Thumb    | Misc 3         | -      | L3      | -      | -      | -      | -       | Z        | Z         | -      | -
Button 15 (R. Thumb) | R. Thumb  | R. Thumb    | Misc 4         | -      | R3      | -      | -      | -      | -       | -        | -         | -      | -

Analog axes in Windows/Android mode:

Host | Controller
-----| ----------
x,y  | Left Joystick  
z,Rz | Right Joystick

For NegCon in Windows/Android, the analog buttons are mapped as follows:

Windows Host (Android)         | NegCon
------------------------------ | ------
x                              | Twist
"Throttle" axis                | I
"Accelerator" axis (R2 Analog) | II      
"Brake axis" (L2 Analog)       | L

The pinout for the arcade mode (no button 3 and no button 6 to maintain Android compatibility):

DB15

	pin 3 - button 7 (Android L1)
	pin 4 - button 5 (Android Y)
	pin 5 - button 10 (Android R2)
	pin 6 - button 11 (Android Select)
	pin 7 - button 12 (Android Start)
	pin 11 - button 8 (Android R1)
	pin 12 - button 4 (Android X)
	pin 13 - button 9 (Android L2)
	pin 1 - GROUND

DB9 (unchanged)

	pin 1 - up
	pin 2 - down
	pin 3 - left
	pin 4 - right
	pin 6 - button 1 (Android A)
	pin 9 - button 2 (Android B)
	pin 8 - GROUND

How to install
--------------

- You can upload the modded firmware to an existing working RetroAdapter by holding button 1 pressed while plugging it in. It will then be recognized as "RA Update Mode."

- Then use the following command to update firmware (see the folder "src_orig/PQureshi/Firmware", run windows shell as administrator):
```
updatefw.exe -r RetroAdapterV2_Normal.hex
```
or
```
updatefw.exe -r RetroAdapterV2_XBox.hex
```
or
```
updatefw.exe -r RetroAdapterV2_Arcade.hex
```
or
```
updatefw.exe -r RetroAdapterV2_Switch.hex
```

- To hook the adapter up to an Xbox, a female usb to Xbox adapter cable is needed.

- To go back to the original firmware from either Xbox mode, arcade mode, normal mode, or Switch mode, use the .hex file from the original RetroAdapter archive (included under src_orig/PQureshi/Firmware):
`updatefw.exe -r RetroAdapter_2.1a.hex`

To Compile from Source
----------------------

- Install WINAVR compiler suite for Windows (or AVR Libc)

- Go to src folder of choice

- Type make and return

- For example, the output for the Xbox mode will end with something like this (sizes will change from version to version, this was recorded for v0.1)
```
AVR Memory Usage
----------------
Device: atmega168

Program:    9740 bytes (59.4% Full)
(.text + .data + .bootloader)

Data:        185 bytes (18.1% Full)
(.data + .bss + .noinit)
```

- to start a new compilation and erase all temp. files, use the command "make clean"

Details
-------

What I did in the beginning to program my Qureshi RetroAdapter using the original firmware and an Arduino board as ISP (my board was a Seeduino from HTink):

- Install Arduino package
- Connect Seeduino to USB
- Manually select usb driver from arduino folder, drivers
- If interested in ISP status, hook up pin 9 green led 220 ohm GND (status of arduinoISP)
- And pin 8 blue LED (error indicator of arduinoISP)
- And pin 7 Red LED (error indicator of arduinoISP)
- Select Board Dueminalove
- load Arduino software, select Examples and upload ArduinoISP (green heartbeat LED should pulsate on pin 9 if an LED is connected)
- On Seeduino, switch reset to manual 'M' using the physical switch on the board itself, otherwise out of sync error will occur

- Connections to make for programming the Adapter with an Arduino
Arduino 11 to DB15 Pin 5,
Arduino 12 to DB15 Pin 4,
Arduino 13 to DB15 Pin 3,
Arduino 10 to Reset hole on Retroadapter.
Arduino VCC to DB15 pin 8
Arduino GND to DB15 pin 1

- Command to use (make sure avrdude.conf and combined.hex are in the same directory, avrdude.conf is in arduino sofware etc directory and avrdude is in bin directory):
```
avrdude -c arduino -p atmega168 -P com3 -b 19200 -U flash:w:combined.hex
avrdude -c arduino -p atmega168 -P com3 -b 19200 -U lfuse:w:0xef:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m
```
- Now this should give you a working original RetroAdapter that can then be modded.

Note
----
The connector cables between RetroAdapter and old-school controllers should be kept short when using unshielded wires to reduce noise.

Changelog
---------
v1.5
- added Nintendo Switch support

v1.4
- (Android/Windows) Last update was completely broken, it broke analog directions and 2 player mode, all fixed again now.

- (Android/Windows) Changed the USB polling interval back to 4ms. It makes no difference under Windows, because Windows locks the polling interval for low speed USB devices at 8ms (125 Hz max). However, Linux does not. So a 4ms polling rate should increase the refresh speed under Linux and maybe even Android, while still keeping low cpu usage.

- (Android/Windows) Came up with exact speed test using freqmeter.exe. On Windows the refresh rate is now 124 Hz (62 Hz for NegCon and 2P mode). Under linux it could be even faster.
