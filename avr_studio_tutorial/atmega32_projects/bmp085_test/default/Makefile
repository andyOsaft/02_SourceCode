###############################################################################
# Makefile for the project bmp085_test
###############################################################################

## General Flags
PROJECT = bmp085_test
MCU = atmega32
TARGET = bmp085_test.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=bmp085_test.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"E:\Buero_111030\Mikrocontroller\02_Quellcode\avr_studio_tutorial\atmega32_projects\bmp085_test\femtoOsTest\initFiles" -I"E:\Buero_111030\Mikrocontroller\02_Quellcode\avr_studio_tutorial\atmega32_projects\bmp085_test\femtoOsTest\twiBus" -I"E:\Buero_111030\Mikrocontroller\02_Quellcode\avr_studio_tutorial\atmega32_projects\bmp085_test\." -I"E:\Buero_111030\Mikrocontroller\02_Quellcode\avr_studio_tutorial\atmega32_projects\bmp085_test\femtoOsTest\lcdFiles" -I"E:\Buero_111030\Mikrocontroller\02_Quellcode\avr_studio_tutorial\atmega32_projects\bmp085_test\femtoOsTest\lcdFonts" -I"E:\Buero_111030\Mikrocontroller\02_Quellcode\avr_studio_tutorial\atmega32_projects\bmp085_test\femtoOsTest" 

## Objects that must be built in order to link
OBJECTS = TWI_Master.o globals.o main.o interrupt.o myInit.o dogm-graphic.o font.o digits_24px.o font_fixed_8px.o font_proportional_8px.o font_proportional_16px.o symbols_8px.o symbols_16px.o template_simplefont.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) bmp085_test.hex bmp085_test.eep bmp085_test.lss size

## Compile
TWI_Master.o: ../femtoOsTest/twiBus/TWI_Master.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

globals.o: ../femtoOsTest/globals.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../femtoOsTest/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

interrupt.o: ../femtoOsTest/initFiles/interrupt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

myInit.o: ../femtoOsTest/initFiles/myInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dogm-graphic.o: ../femtoOsTest/lcdFiles/dogm-graphic.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font.o: ../femtoOsTest/lcdFiles/font.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

digits_24px.o: ../femtoOsTest/lcdFonts/digits_24px.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font_fixed_8px.o: ../femtoOsTest/lcdFonts/font_fixed_8px.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font_proportional_8px.o: ../femtoOsTest/lcdFonts/font_proportional_8px.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font_proportional_16px.o: ../femtoOsTest/lcdFonts/font_proportional_16px.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

symbols_8px.o: ../femtoOsTest/lcdFonts/symbols_8px.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

symbols_16px.o: ../femtoOsTest/lcdFonts/symbols_16px.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

template_simplefont.o: ../femtoOsTest/lcdFonts/template_simplefont.c
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
	-rm -rf $(OBJECTS) bmp085_test.elf dep/* bmp085_test.hex bmp085_test.eep bmp085_test.lss bmp085_test.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

