###############################################################################
# Makefile for the project droneSensorTest
###############################################################################

## General Flags
PROJECT = droneSensorTest
MCU = atmega32
TARGET = droneSensorTest.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99         -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=droneSensorTest.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneSensorTest_121012\barometricSensor" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneSensorTest_121012\initFiles" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneSensorTest_121012\lcdFiles" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneSensorTest_121012\lcdFonts" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneSensorTest_121012\twiBus" -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneSensorTest_121012\." -I"E:\Buero_120801\Mikrocontroller\02_Quellcode\avrStudio4projects\droneSensorTest_121012\imuFiles" 

## Libraries
LIBS = -lm 

## Objects that must be built in order to link
OBJECTS = main.o globals.o atmega_bmp085_lib.o adcInit.o interrupt.o myInit.o pwmInit.o timerInit.o twi_lib.o imu3000.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) droneSensorTest.hex droneSensorTest.eep droneSensorTest.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

globals.o: ../globals.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

atmega_bmp085_lib.o: ../barometricSensor/atmega_bmp085_lib.c
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

twi_lib.o: ../twiBus/twi_lib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imu3000.o: ../imuFiles/imu3000.c
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
	-rm -rf $(OBJECTS) droneSensorTest.elf dep/* droneSensorTest.hex droneSensorTest.eep droneSensorTest.lss droneSensorTest.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

