###############################################################################
# Makefile for the project lcd_ansteuerung
###############################################################################

## General Flags
PROJECT = lcd_ansteuerung
MCU = atmega8
TARGET = lcd_ansteuerung.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=3686400UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=lcd_ansteuerung.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = main.o wait.o lcd_send.o lcd_cmd.o lcd_clear.o lcd_home.o lcd_on.o lcd_off.o lcd_goto.o lcd_light.o lcd_dimmer.o lcd_write.o lcd_init.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) lcd_ansteuerung.hex lcd_ansteuerung.eep lcd_ansteuerung.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wait.o: ../wait.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_send.o: ../lcd_send.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_cmd.o: ../lcd_cmd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_clear.o: ../lcd_clear.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_home.o: ../lcd_home.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_on.o: ../lcd_on.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_off.o: ../lcd_off.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_goto.o: ../lcd_goto.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_light.o: ../lcd_light.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_dimmer.o: ../lcd_dimmer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_write.o: ../lcd_write.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd_init.o: ../lcd_init.c
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
	-rm -rf $(OBJECTS) lcd_ansteuerung.elf dep/* lcd_ansteuerung.hex lcd_ansteuerung.eep lcd_ansteuerung.lss lcd_ansteuerung.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

