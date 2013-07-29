###############################################################################
# Makefile for the project imu3000Test
###############################################################################

## General Flags
PROJECT = imu3000Test
MCU = atmega32
TARGET = imu3000Test.elf
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
LDFLAGS +=  -Wl,-Map=imu3000Test.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Libraries
LIBS = -lm 

## Objects that must be built in order to link
OBJECTS = avr.o imu.o main.o fusion.o imuMldl.o imuSetup.o twi.o imuMlsl.o imuMlos.o dmpDefault.o mlcontrol.o imuFIFO.o imuCompatibility.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) imu3000Test.hex imu3000Test.eep imu3000Test.lss size

## Compile
avr.o: ../avr.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imu.o: ../imu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

fusion.o: ../fusion.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imuMldl.o: ../imuMldl.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imuSetup.o: ../imuSetup.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

twi.o: ../twi.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imuMlsl.o: ../imuMlsl.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imuMlos.o: ../imuMlos.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dmpDefault.o: ../dmpDefault.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mlcontrol.o: ../mlcontrol.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imuFIFO.o: ../imuFIFO.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

imuCompatibility.o: ../imuCompatibility.c
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
	-rm -rf $(OBJECTS) imu3000Test.elf dep/* imu3000Test.hex imu3000Test.eep imu3000Test.lss imu3000Test.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

