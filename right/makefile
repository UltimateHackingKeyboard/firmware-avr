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

MCU          = atmega32u2
ARCH         = AVR8
BOARD        = NONE
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = uhk-right
LUFA_PATH    = lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =
AVRDUDE_PROGRAMMER = avr109
AVRDUDE_PORT = /dev/ttyACM0

SRC = $(TARGET).c                                                   \
	../KeyMatrix.c                                              \
	../USART.c                                                  \
	BootloaderJump.c                                            \
	DefaultLayout.c                                             \
	Descriptors.c                                               \
	Keyboard.c                                                  \
	KeyboardReports.c                                           \
	KeyboardDescriptors.c                                       \
	USBtoSerial.c                                               \
	USBtoSerialDescriptors.c                                    \
	$(LUFA_SRC_USB)                                             \
	$(LUFA_SRC_USBCLASS)

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
