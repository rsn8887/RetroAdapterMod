# RetroAdapterMod
Mod of Paul Qureshi's Mojo RetroAdapter Firmware

Paul Qureshi RetroAdapter Firmware v2.1a Mod v1.1
--------------------------------------------------

References
----------

License GPL (see License.txt)

- Original source for "USB Retropad Adapter" from Bruno Freitas acquired via
http://www.brunofreitas.com/node/41
(included for reference in folder original_Files)

- Original source for "Retro Adapter v2.1a" from Paul Qureshi acquired via
http://denki.world3.net/retro_v2.html
(incuded for reference in folder original_Files)

- This mod by rsn8887 [![Flattr this git repo](http://api.flattr.com/button/flattr-badge-small.png)](https://flattr.com/submit/auto?user_id=rsn8887&url=https://github.com/rsn8887/RetroAdapterMod&title=RetroAdapterMod&language=C&tags=github&category=software)

What is it?
-----------

- Included are three modified versions of the original Mojo Retroadapter firmware, as hex and source files: "Xbox mode, arcade mode, and normal mode"

- All three versions include the addition of NegCon controller support, amongst other enhancements.

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

- Normal mode: 
This is for regular use and functions identical to the original RetroAdapter. However, it is improved for operation under Android OS. Under Android, the original RetroAdapter was not reporting directions up and left for NES and some other controller types. Note: The Android fixes are also incorporated into the Arcade mode version.

- Button config for Normal and XBox Modes:

Normal Mode (Android) | XBox Mode | Description    | SNES   | PSX     | NegCon | NES    | NeoGeo | Genesis | Gamecube | N64       | PCE    | Saturn
--------------------- | --------- | -------------- | ------ | ------- | ------ | ------ | ------ | ------- | -------- | --------- | ------ | ------  
Button 1 (A)          | A         | Bottom button  | B      | Cross   | I      | B      | A      | A       | A        | B         | II     | A
Button 2 (B)          | B         | Right button   | A      | Circle  | A      | A      | B      | B       | X        | A         | I      | B
Button 4 (X)          | X         | Left button    | Y      | Square  | II     | -      | C      | C       | B        | Y. Down   | III    | C 
Button 5 (Y)          | Y         | Upper button   | X      | Triangle| B      | -      | D      | X       | Y        | Y. Up     | IV     | X
Button 7 (L)          | L         | Left shoulder  | L      | L1      | L      | -      | -      | Y       | L        | L         | V      | Y
Button 8 (R)          | R         | Right shoulder | R      | R1      | R      | -      | -      | Z       | R        | R         | VI     | Z
Button 9 (L2)         | White     | Misc 1         | -      | L2      | -      | -      | -      | -       | -        | Y. Left   | -      | L
Button 10 (R2)        | Black     | Misc 2         | -      | R2      | -      | -      | -      | -       | -        | Y. Right  | -      | R
Button 11 (Select)    | Select    | Select         | Select | Select  | -      | Select | Select | -       | -        | -         | Select | -
Button 12 (Start)     | Start     | Start          | Start  | Start   | -      | Start  | Start  | Start   | Start    | Start     | Play   | Start
Button 14 (L. Thumb)  | L. Thumb  | Misc 3         | -      | L3      | -      | -      | -      | -       | Z        | Z         | -      | -
Button 15 (R. Thumb)  | R. Thumb  | Misc 4         | -      | R3      | -      | -      | -      | -       | -        | -         | -      | -


- Analog axes in Normal Mode:

Controller     | Host
-------------- | -------
Left Joystick  | x,y
Right Joystick | z,Rz

For NegCon (identifies as "Mojo Retro Adapter NegCon" instead of "Mojo Retro Adapter" when NegCon is detected):

NegCon        | Host
------------- | -------
Steering      | x
Button I      | z (0..255 in NegCon mode instead of -128..127) (Android gas)
Button II     | "Accelerator" axis (Android analog right shoulder)
Button L      | "Brake axis" (Android analog left shoulder)

- For more information, please see readme.txt
