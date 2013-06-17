# TODO: What compiler flags to use?

MCU = atmega48
AVRDUDE_MCU = m48
F_CPU = 16000000
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
REMOVE = rm -f
FORMAT = ihex
TARGET = uhk-left
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -std=c99 -O

all: build

build: $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o$(TARGET).elf
	$(OBJCOPY) -O $(FORMAT) $(TARGET).elf $(TARGET).hex

program-fuses:
	$(AVRDUDE) -p $(AVRDUDE_MCU) -c usbtiny -U lfuse:w:0xce:m

program: build
	$(AVRDUDE) -p $(AVRDUDE_MCU) -c usbtiny -U flash:w:$(TARGET).hex

clean:
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).hex

.PHONY: all build program-fuses program clean

upload:
	rootify avrdude -p m48 -c usbtiny -U flash:w:uhk-left.hex
