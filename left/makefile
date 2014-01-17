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
AVRDUDE_PROGRAMMER = avr109
AVRDUDE_PORT = /dev/ttyACM0

SRC = $(TARGET).c                                                   \
	KeyMatrix.c                                                 \
	USART.c                                                     \

build: sync-shared-files all
	

sync-shared-files:
	rsync --update ../*.[ch] .

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
