###############################################################################
# Makefile for the project RetroAdapterV2 **** changed 06/15/2015 by rsn8887
###############################################################################

## General Flags
PROJECT = RetroAdapterV2
MCU = atmega168
TARGET = RetroAdapterV2.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)
INCLUDES = -I./ -I../usbdrv/ -I../

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99  -DF_CPU=15000000UL -O1 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
## CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=RetroAdapterV2.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Some objects that must be built in order to link
OBJECTSASM = amiga_m_asm.o dreamcast_asm.o n64gc_asm.o

OBJECTSC = amiga_mouse.o n64gc.o dreamcast.o analogue.o 3do.o atari_driving.o cd32.o pc-engine.o pc-fx.o sega_paddle.o dualdb9.o famicom.o psx.o saturn.o timers.o

OBJECTSCARCADE = RetroAdapterV2.o direct.o

ALLOBJECTS = $(OBJECTSASM) $(OBJECTSC) $(OBJECTSCARCADE) usbdrv.o usbdrvasm.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) RetroAdapterV2.hex RetroAdapterV2.eep RetroAdapterV2.lss size

## Compile
$(OBJECTSC): %.o: ../%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $<

$(OBJECTSASM): %.o: ../%.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $<

$(OBJECTSCARCADE): %.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $<
	
usbdrv.o: ../usbdrv/usbdrv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c $<

usbdrvasm.o: ../usbdrv/usbdrvasm.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c $< 


##Link
$(TARGET): $(ALLOBJECTS) 
	 $(CC) $(LDFLAGS) $(ALLOBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(ALLOBJECTS) RetroAdapterV2.elf dep/* RetroAdapterV2.hex RetroAdapterV2.eep RetroAdapterV2.lss RetroAdapterV2.map


## Other dependencies
##-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

