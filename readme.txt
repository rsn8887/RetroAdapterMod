Paul Qureshi RetroAdapter Firmware v2.1a Mod v0.94
--------------------------------------------------

References
----------

License GPL (see License.txt)

- Original source for "USB Retropad Adapter" from Bruno Freitas acquired via
http://www.brunofreitas.com/node/41
(included for reference in folder src_orig)

- Original source for "Retro Adapter v2.1a" from Paul Qureshi acquired via
http://denki.world3.net/retro_v2.html
(incuded for reference in folder src_orig)

- This mod by rsn8887

What is it?
-----------

- Included are three modified versions of the original Mojo Retroadapter firmware, as hex and source files: "Xbox mode, arcade mode, and normal mode"

- all three versions include the addition of NegCon controller support.

- Xbox mode:
I modified and recompiled the Retroadapter firmware, replacing the original PC support with Xbox classic support. 

 - A RetroAdapter updated with this modded Firmware only works on Xbox Classic (note: Update mode still works so you can always go back after modding)
 - I used the Xbox communication and initialization code from Bruno Freitas' Retropad Adapter and merged it into the codebase from Paul Qureshi
 - See http://www.brunofreitas.com/node/41 for more information on Bruno Freitas' Adapter
 - I also updated the makefile and changed some of the button mappings to better reflect operation on Xbox
 - SNES mouse and dual player support was removed to comply with the limitations of the XBox classic
 - If you want to use a mouse, just hook up an original Qureshi RetroAdapter to your Xbox, because Xbox already supports standard usb mice
 - If you want 2-4 player support, just hook up multiple retroadapters to the xbox, one player per port

- Arcade mode: 
This version is suited for use in an Arcade cabinet. It works on PCs just like the original RetroAdapter, but I replaced Neogeo stick support on the DB15 with an arcade mode that has just 8 firebuttons on the DB15, to give you a total of 10 buttons and 4 directions when you hook up to DB9 and DB15 both.

The pinout for the Arcade mode:
db15
pin 3 - button 7
pin 4 - button 4
pin 5 - button 6
pin 6 - button 9
pin 7 - button 10
pin 11 - button 8
pin 12 - button 3
pin 13 - button 5

db9 (unchanged)
pin 1 - up
pin 2 - down
pin 3 - left
pin 4 - right
pin 6 - button 1
pin 9 - button 2


- Normal mode: 
This is for regular use and functions identical to the original RetroAdapter. However, it is improved for operation under Android OS. Under Android, the original RetroAdapter was not reporting directions up and left for NES and some other controller types. Note: The Android fixes are also incorporated into the Arcade mode version.

How to install
--------------

- You can upload the modded firmware to an existing working RetroAdapter by holding button 1 pressed while plugging it in. It will then be recognized as "RA Update Mode."

- Then use the following command to update firmware (see the folder "src_orig/PQureshi/Firmware", run windows shell as administrator):
updatefw.exe -r RetroAdapterV2_XBoxMode.hex 

or

updatefw.exe -r RetroAdapterV2_ArcadeMode.hex

or

updatefw.exe -r RetroAdapterV2_NormalMode.hex 
 
- As a test of the Xbox mode, you will have to hook the adapter up to an Xbox with a female usb to Xbox adapter cable and verify that it works.

- As a test of the arcade mode, connect DB15 pin 7 to GND. In the arcade mode, this is Firebutton #10, with the original firmware, this would be "Down"


- As a test of the normal mode, hook up the adapter to an Android device, and check if the directions up and left are working correctly for the NES/SNES controllers, PSX Analog controllers and N64/GC controllers. These will not work with the original firmware.


- To go back to the original firmware from either Xbox mode, arcade mode, or normal mode, use the .hex file from the original RetroAdapter archive (included under src_orig/PQureshi/Firmware):
updatefw.exe -r RetroAdapter_2.1a.hex

To Compile from Source
----------------------

- Install WINAVR compiler suite for Windows (or AVR Libc)

- Go to src folder of choice

- Type make and return

- For example, the output for the Xbox mode will end with something like this (sizes will change from version to version, this was recorded for v0.1)
"
AVR Memory Usage
----------------
Device: atmega168

Program:    9740 bytes (59.4% Full)
(.text + .data + .bootloader)

Data:        185 bytes (18.1% Full)
(.data + .bss + .noinit)
"

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

Cheers,
rsn8887, 06/26/2015
