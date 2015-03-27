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
CC_FLAGS     = -I..
LD_FLAGS     =
AVRDUDE_PROGRAMMER = avrisp2
AVRDUDE_PORT = usb
AVRDUDE_FLAGS = -b 38400 -F

SRC = $(TARGET).c        \
	../KeyMatrix.c   \
	../USART.c       \
	../TWI.c         \
	ShiftRegister.c  \
	PCA9634.c        \
	LedMatrix.c

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

upload: all
	@../reenumerate --wait bootloader-left
	@echo r > /dev/ttyACM0
	@sleep 1
	@make avrdude
	@../reenumerate --wait keyboard

fuses:
	avrdude -p $(MCU) -U lfuse:w:0xce:m -U efuse:w:0xce:m

clean: mostlyclean
	@echo $(MSG_REMOVE_CMD) Removing output files of \"$(TARGET)\"
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).eep $(TARGET).map $(TARGET).lss $(TARGET).sym $(TARGET).a
