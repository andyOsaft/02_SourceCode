###############################################################################
# Makefile for the project droneOS
###############################################################################

## General Flags
PROJECT = droneOS
MCU = atmega32
TARGET = droneOS.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99   -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=droneOS.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneOS\barometricSensor" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneOS\initFiles" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneOS\lcdFiles" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneOS\lcdFonts" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneOS\twiBus" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneOS\." 

## Objects that must be built in order to link
OBJECTS = main.o globals.o adcInit.o interrupt.o myInit.o pwmInit.o timerInit.o dogm-graphic.o font.o font_fixed_8px.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) droneOS.hex droneOS.eep droneOS.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

globals.o: ../globals.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adcInit.o: ../initFiles/adcInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

interrupt.o: ../initFiles/interrupt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

myInit.o: ../initFiles/myInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pwmInit.o: ../initFiles/pwmInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timerInit.o: ../initFiles/timerInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dogm-graphic.o: ../lcdFiles/dogm-graphic.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font.o: ../lcdFiles/font.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

font_fixed_8px.o: ../lcdFonts/font_fixed_8px.c
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
	-rm -rf $(OBJECTS) droneOS.elf dep/* droneOS.hex droneOS.eep droneOS.lss droneOS.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

