                    Retro Adapter Firmware Updater V1.0
                    ===================================

1. Updating the firmware
========================

The updatefw.exe utility is used to update the Retro Adapter's firmware over
USB. Place the RetroAdapter_X.X.hex file in the same directory.

There are several ways to enter firmware update mode. Do one of the
following while plugging the Retro Adapter in:

- Hold down fire button 1 on an Atari/Amiga style joystick
- Hold down button B on a Sega Master System/Megadrive (Genesis) pad
- Hold down button A on a Neo Geo joystick/pad
- Connect the special Firmware Update Connector.

The first time you do this, Windows will detect the Retro Adapter as "RA
Update Mode". Wait a few seconds until it is fully recognised and then let
go of the button or remove the Firmware Update Connector. Finally run
update.bat to load the latest firmware.

If something goes wrong during an update, don't worry as you can simply
repeat the process starting from the beginning again.

Once the update is complete the Retro Adapter will enter normal mode and is
ready to use.


2. About the Firmware Updater
=============================

The Firmware Updater is a simple modification of the Objective Development
bootloadHID program, modified to use the Retro Adapter's VID/PID pair and ID
strings.

The AVR bootloader is based on the same reference implementation with minor
changes. Aside from the VID/PID pair, it was modified to remain in the
bootloader even after the fire button is released to save the user from
having to hold it down during the update. As such, the only way to exit the
bootloader is to either update the firmware with the -r (reboot) option or
to unplug/reconnect it.

All debug code was commented out too, which saves quite a few bytes.


3. Combined.hex
===============

The combined.hex file includes both the firmware and the bootloader in one.
If you are programming your own AVR you can use this to avoid having to
first flash the firmware and then the bootloader separately.

4. History
==========

V1.0    -   First release, no known issues.