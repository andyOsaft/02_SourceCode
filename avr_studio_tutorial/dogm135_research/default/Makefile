###############################################################################
# Makefile for the project dogm135_research
###############################################################################

## General Flags
PROJECT = dogm135_research
MCU = atmega32
TARGET = dogm135_research.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                      -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=dogm135_research.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"E:\Buero_02_05_10\Mikrocontroller\2. Quellcode\avr_studio_tutorial\dogm135_research\Fonts" 

## Library Directories
LIBDIRS = -L"E:\Buero_02_05_10\Mikrocontroller\2. Quellcode\avr_studio_tutorial\dogm135_research\Fonts" 

## Objects that must be built in order to link
OBJECTS = dogm-graphic.o main.o font.o font_fixed_8px.o pwm_init.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) dogm135_research.hex dogm135_research.eep dogm135_research.lss size

## Compile
dogm-graphic.o: ../dogm-graphic.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font.o: ../font.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font_fixed_8px.o: ../Fonts/font_fixed_8px.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pwm_init.o: ../pwm_init.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

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
	-rm -rf $(OBJECTS) dogm135_research.elf dep/* dogm135_research.hex dogm135_research.eep dogm135_research.lss dogm135_research.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

