###############################################################################
# Makefile for the project t3_controller
###############################################################################

## General Flags
PROJECT = t3_controller
MCU = atmega32
TARGET = t3_controller.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=4000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=t3_controller.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main.o adcInit.o global.o heatController.o interrupt.o lcdRoutines.o menuScreens.o myInit.o pwmInit.o timerInit.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) t3_controller.hex t3_controller.eep t3_controller.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adcInit.o: ../adcInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

global.o: ../global.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

heatController.o: ../heatController.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

interrupt.o: ../interrupt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcdRoutines.o: ../lcdRoutines.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

menuScreens.o: ../menuScreens.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

myInit.o: ../myInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

pwmInit.o: ../pwmInit.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timerInit.o: ../timerInit.c
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
	-rm -rf $(OBJECTS) t3_controller.elf dep/* t3_controller.hex t3_controller.eep t3_controller.lss t3_controller.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

