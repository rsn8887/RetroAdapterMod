# RetroAdapterMod
Mod of Paul Qureshi's Mojo RetroAdapter

Paul Qureshi RetroAdapter Firmware v2.1a Mod v0.3
-------------------------------------------------

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

For more information, see readme.txt
