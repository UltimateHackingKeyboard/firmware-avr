Ultimate Hacking Keyboard firmware
==================================

[![Changelog #186](https://img.shields.io/badge/changelog-%23186-lightgrey.svg)](https://changelog.com/186)

## Fetch the firmware

Please make sure to clone this repo with:

`git clone --recursive git@github.com:UltimateHackingKeyboard/firmware.git`

This way the LUFA submodule will be cloned too, which is a necessary dependecy.

## Customize the keymap

Switch to the `mac` vs `pc` branch if you use a Macintosh vs a Linux/Windows PC/laptop. This will set up a reasonable default keymap.

`git checkout mac` or `git checkout pc`

Now, edit the array in [right/DefaultKeyMap.c](right/DefaultKeyMap.c) to customize your keymap.

## Build the firmware

Install the necessary buld tools on your system such as AVR GCC and AVR Libc. Then go into the `right` directory and type `make`. The resulting firmware image is `uhk-right.hex`.

## Upload the firmware

