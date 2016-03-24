# RetroAdapterMod
Mod of Paul Qureshi's Mojo RetroAdapter Firmware

Paul Qureshi RetroAdapter Firmware v2.1a Mod v1.4
-------------------------------------------------

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

- Included are three modified versions of the original Mojo Retroadapter firmware, as hex and source files: "Xbox mode, arcade mode, and normal mode"

- All three versions include the addition of NegCon controller support.

- Xbox mode:
I modified and recompiled the Retroadapter firmware, replacing the original PC support with Xbox classic support. 

 - A RetroAdapter updated with this modded Firmware only works on Xbox Classic (note: Update mode still works so you can always go back after modding)
 - I used the Xbox communication and initialization code from Bruno Freitas' Retropad Adapter and merged it into the codebase from Paul Qureshi
 - See http://www.brunofreitas.com/node/41 for more information on Bruno Freitas' adapter
 - I also updated the makefile and changed some of the button mappings to better reflect operation on Xbox
 - SNES mouse and dual player support was removed to comply with the limitations of the XBox classic
 - If you want to use a mouse, just hook up an original Qureshi RetroAdapter to your Xbox, because Xbox already supports standard usb mice
 - If you want 2-4 player support, just hook up multiple retroadapters to the xbox, one player per port

- Normal mode: 
This is for regular use and functions identical to the original RetroAdapter. However, it is improved for operation with Android devices. Under Android, the original RetroAdapter was not reporting directions up and left for NES and some other controller types. Note: The Android fixes are also incorporated into the Arcade mode version.

- Button config for normal mode (Windows/Android) and XBox mode:

Windows Host (Andr.) | XBox Host | Description    | SNES   | PSX     | NegCon | NES    | NeoGeo | Genesis | Gamecube | N64       | PCE    | Saturn
-------------------- | --------- | -------------- | ------ | ------- | ------ | ------ | ------ | ------- | -------- | --------- | ------ | ------  
Button 1 (A)         | A         | Bottom button  | B      | Cross   | I      | B      | A      | A       | A        | B         | II     | A
Button 2 (B)         | B         | Right button   | A      | Circle  | A      | A      | B      | B       | X        | A         | I      | B
Button 4 (X)         | X         | Left button    | Y      | Square  | II     | -      | C      | C       | B        | Y. Down   | III    | C 
Button 5 (Y)         | Y         | Upper button   | X      | Triangle| B      | -      | D      | X       | Y        | Y. Up     | IV     | X
Button 7 (L)         | L         | Left shoulder  | L      | L1      | L      | -      | -      | Y       | L        | L         | V      | Y
Button 8 (R)         | R         | Right shoulder | R      | R1      | R      | -      | -      | Z       | R        | R         | VI     | Z
Button 9 (L2)        | White     | Misc 1         | -      | L2      | -      | -      | -      | -       | -        | Y. Left   | -      | L
Button 10 (R2)       | Black     | Misc 2         | -      | R2      | -      | -      | -      | -       | -        | Y. Right  | -      | R
Button 11 (Select)   | Select    | Select         | Select | Select  | -      | Select | Select | -       | -        | -         | Select | -
Button 12 (Start)    | Start     | Start          | Start  | Start   | -      | Start  | Start  | Start   | Start    | Start     | Play   | Start
Button 14 (L. Thumb) | L. Thumb  | Misc 3         | -      | L3      | -      | -      | -      | -       | Z        | Z         | -      | -
Button 15 (R. Thumb) | R. Thumb  | Misc 4         | -      | R3      | -      | -      | -      | -       | -        | -         | -      | -

Analog axes in normal mode:

Host | Controller 
-----| ----------
x,y  | Left Joystick  
z,Rz | Right Joystick

For NegCon in normal mode (Windows/Android), the analog buttons are mapped as follows:

Windows Host (Android)         | NegCon
------------------------------ | ------ 
x                              | Twist
"Throttle" axis                | I
"Accelerator" axis (R2 Analog) | II      
"Brake axis" (L2 Analog)       | L

- Arcade mode: 
This version is suited for use in an arcade cabinet. It works on PCs just like the original RetroAdapter, but I replaced Neogeo stick support on the DB15 with an arcade mode that has just 8 firebuttons on the DB15, to give you a total of 10 buttons and 4 directions when you hook up your Arcade controls to pins on both DB9 and DB15.

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
updatefw.exe -r RetroAdapterV2_XBoxMode.hex 
```
or
```
updatefw.exe -r RetroAdapterV2_ArcadeMode.hex
```
or
```
updatefw.exe -r RetroAdapterV2_NormalMode.hex 
```
- As a test of the Xbox mode, you will have to hook the adapter up to an Xbox with a female usb to Xbox adapter cable and verify that it works.

- As a test of the arcade mode, connect DB15 pin 7 to GND. In the arcade mode, this is Firebutton #12, with the original firmware, this would be "Down"

- As a test of the normal mode, hook up the adapter to an Android device, and check if the directions up and left are working correctly for the NES/SNES controllers, PSX Analog controllers and N64/GC controllers. These will not work with the original firmware.

- To go back to the original firmware from either Xbox mode, arcade mode, or normal mode, use the .hex file from the original RetroAdapter archive (included under src_orig/PQureshi/Firmware):
updatefw.exe -r RetroAdapter_2.1a.hex

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

avrdude -c arduino -p atmega168 -P com3 -b 19200 -U flash:w:combined.hex

avrdude -c arduino -p atmega168 -P com3 -b 19200 -U lfuse:w:0xef:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m

- Now this should give you a working original RetroAdapter that can then be modded.

Note
----
The connector cables between RetroAdapter and old-school controllers should be kept short when using unshielded wires to reduce noise.

rsn8887, 03/23/2016
