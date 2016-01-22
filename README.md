Ultimate Hacking Keyboard firmware
==================================

[![Changelog #186](https://img.shields.io/badge/changelog-%23186-lightgrey.svg)](https://changelog.com/186)

## Fetch the firmware

Please make sure to clone this repo with:

`git clone --recursive git@github.com:UltimateHackingKeyboard/firmware.git`

This way the LUFA submodule will be cloned too, which is a necessary dependecy.

## Customize the keymap

Switch to the `mac` vs `pc` branch if you use a Mac vs a Linux/Windows PC/laptop. This will set up a reasonable default keymap.

`git checkout mac` or `git checkout pc`

Now, edit the array in [right/DefaultKeyMap.c](right/DefaultKeyMap.c) to customize your keymap.

## Build the firmware

Install the necessary buld tools on your system such as AVR GCC and AVR Libc. Then go into the `right` directory and type `make`. The resulting firmware image is `uhk-right.hex`.

## Upload the firmware

*Please note that during this process the UHK will become temporarily unusable, so be sure to have another keyboard at your disposal to be able to enter commands.*

1. Install avrdude on your system. You should use avrdude version 6 or later because earlier versions may not work properly.
 * On Linux: `apt-get install avrdude` (or something else if you use another package manager)
 * On Mac: `brew install avrdude --with-usb`
2. Switch the right keyboard half to bootloader mode. For this, you must put [a magnet](https://ultimatehackingkeyboard.com/blog/2015/07/20/upgrading-the-firmware-with-a-neodymium-magnet) near the back of the right keyboard half around the bottom outside corner. It can be a fridge magnet or even the magnet of the other keyboard half.
 * If pressing the keys on the keyboard doesn’t do anything anymore then you’ve succeeded.
 * Don’t worry, you can unplug the USB cable and plug it back any time to make the keyboard usable again.
3. Locate the device file that popped up when you switched the keyboard to bootloader mode. It should be something like `/dev/ttyUSB0` on Linux and `/dev/cu.usbmodem1411` on Mac.
4. Substitute `YourDeviceFile` in the following command with the actual values and enter it: `avrdude -p atmega32u4 -P YourDeviceFile -c avr109 -U flash:w:uhk-left.hex`
5. If everything went well then the UHK reenumerated as a keyboard device running the new firmware.
