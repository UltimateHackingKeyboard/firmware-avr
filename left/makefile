#
#             LUFA Library
#     Copyright (C) Dean Camera, 2013.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega88
ARCH         = AVR8
F_CPU        = 16000000
F_USB        = 16000000
OPTIMIZATION = s
TARGET       = uhk-left
LUFA_PATH    = ../right/lufa/LUFA
CC_FLAGS     =
LD_FLAGS     =
AVRDUDE_PROGRAMMER = stk500v1
AVRDUDE_PORT = /dev/ttyACM0
AVRDUDE_FLAGS = -b 38400 -F

SRC = $(TARGET).c                                                   \
	KeyMatrix.c                                                 \
	USART.c                                                     \

build: sync-shared-files all

sync-shared-files:
	rsync --update ../*.[ch] .

upload: build
	../jump-to-bootloader left enter
	@echo 'Reenumerating as the left bootloader...'
	@sleep 3
	@echo 'Reset the left half and quickly after press enter!'
	@read enter
	make avrdude
	../jump-to-bootloader left leave
	@echo 'Reenumerating as the keyboard...'

fuses:
	avrdude -p $(MCU) -U lfuse:w:0xce:m -U efuse:w:0xce:m

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk

clean: mostlyclean
	@echo $(MSG_REMOVE_CMD) Removing output files of \"$(TARGET)\"
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).eep $(TARGET).map $(TARGET).lss $(TARGET).sym $(TARGET).a
	rm `cat .gitignore`
