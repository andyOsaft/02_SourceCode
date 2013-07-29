################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
LIB_AS_ARGS :=
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
HEX_FLASH_FILE_PATH :=
HEX_FLASH_FILE_PATH_AS_ARGS :=
HEX_EEPROM_FILE_PATH :=
HEX_EEPROM_FILE_PATH_AS_ARGS :=
LSS_FILE_PATH :=
LSS_FILE_PATH_AS_ARGS :=
MAP_FILE_PATH :=
MAP_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=C:/Program Files (x86)/Atmel/AVR Studio 5.0/AVR ToolChain/bin/
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../initFiles \
../lcdFiles \
../lcdFonts \
../barometricSensor \
../twiBus


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../barometricSensor/bmp085.c \
../main.c \
../globals.c \
../initFiles/adcInit.c \
../initFiles/interrupt.c \
../initFiles/myInit.c \
../initFiles/pwmInit.c \
../initFiles/timerInit.c \
../lcdFiles/dogm-graphic.c \
../lcdFiles/font.c \
../lcdFonts/digits_24px.c \
../lcdFonts/font_fixed_8px.c \
../lcdFonts/font_proportional_16px.c \
../lcdFonts/font_proportional_8px.c \
../lcdFonts/symbols_16px.c \
../lcdFonts/symbols_8px.c \
../lcdFonts/template_simplefont.c \
../twiBus/TWI_Master.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
barometricSensor/bmp085.o \
main.o \
globals.o \
initFiles/adcInit.o \
initFiles/interrupt.o \
initFiles/myInit.o \
initFiles/pwmInit.o \
initFiles/timerInit.o \
lcdFiles/dogm-graphic.o \
lcdFiles/font.o \
lcdFonts/digits_24px.o \
lcdFonts/font_fixed_8px.o \
lcdFonts/font_proportional_16px.o \
lcdFonts/font_proportional_8px.o \
lcdFonts/symbols_16px.o \
lcdFonts/symbols_8px.o \
lcdFonts/template_simplefont.o \
twiBus/TWI_Master.o


OBJS_AS_ARGS +=  \
barometricSensor/bmp085.o \
main.o \
globals.o \
initFiles/adcInit.o \
initFiles/interrupt.o \
initFiles/myInit.o \
initFiles/pwmInit.o \
initFiles/timerInit.o \
lcdFiles/dogm-graphic.o \
lcdFiles/font.o \
lcdFonts/digits_24px.o \
lcdFonts/font_fixed_8px.o \
lcdFonts/font_proportional_16px.o \
lcdFonts/font_proportional_8px.o \
lcdFonts/symbols_16px.o \
lcdFonts/symbols_8px.o \
lcdFonts/template_simplefont.o \
twiBus/TWI_Master.o


C_DEPS +=  \
barometricSensor/bmp085.d \
main.d \
globals.d \
initFiles/adcInit.d \
initFiles/interrupt.d \
initFiles/myInit.d \
initFiles/pwmInit.d \
initFiles/timerInit.d \
lcdFiles/dogm-graphic.d \
lcdFiles/font.d \
lcdFonts/digits_24px.d \
lcdFonts/font_fixed_8px.d \
lcdFonts/font_proportional_16px.d \
lcdFonts/font_proportional_8px.d \
lcdFonts/symbols_16px.d \
lcdFonts/symbols_8px.d \
lcdFonts/template_simplefont.d \
twiBus/TWI_Master.d


C_DEPS_AS_ARGS +=  \
barometricSensor/bmp085.d \
main.d \
globals.d \
initFiles/adcInit.d \
initFiles/interrupt.d \
initFiles/myInit.d \
initFiles/pwmInit.d \
initFiles/timerInit.d \
lcdFiles/dogm-graphic.d \
lcdFiles/font.d \
lcdFonts/digits_24px.d \
lcdFonts/font_fixed_8px.d \
lcdFonts/font_proportional_16px.d \
lcdFonts/font_proportional_8px.d \
lcdFonts/symbols_16px.d \
lcdFonts/symbols_8px.d \
lcdFonts/template_simplefont.d \
twiBus/TWI_Master.d


OUTPUT_FILE_PATH +=femtoOsTest.elf

OUTPUT_FILE_PATH_AS_ARGS +=femtoOsTest.elf

HEX_FLASH_FILE_PATH +=femtoOsTest.hex

HEX_FLASH_FILE_PATH_AS_ARGS +=femtoOsTest.hex

HEX_EEPROM_FILE_PATH +=femtoOsTest.eep

HEX_EEPROM_FILE_PATH_AS_ARGS +=femtoOsTest.eep

LSS_FILE_PATH +=femtoOsTest.lss

LSS_FILE_PATH_AS_ARGS +=femtoOsTest.lss

MAP_FILE_PATH =femtoOsTest.map

MAP_FILE_PATH_AS_ARGS =femtoOsTest.map

LIB_AS_ARGS +=libfemtoOsTest.a

ADDITIONAL_DEPENDENCIES:= $(HEX_FLASH_FILE_PATH) $(LSS_FILE_PATH) $(HEX_EEPROM_FILE_PATH) size

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR/GNU C Compiler





































barometricSensor/%.o: ../barometricSensor/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=8000000  -I".." -I"../lcdFiles" -I"../lcdFonts" -I"../osHeaders" -I"../osSources" -I"../initFiles" -I"../twiBus"  -O1 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99  -mmcu=atmega32   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=8000000  -I".." -I"../lcdFiles" -I"../lcdFonts" -I"../osHeaders" -I"../osSources" -I"../initFiles" -I"../twiBus"  -O1 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99  -mmcu=atmega32   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

initFiles/%.o: ../initFiles/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=8000000  -I".." -I"../lcdFiles" -I"../lcdFonts" -I"../osHeaders" -I"../osSources" -I"../initFiles" -I"../twiBus"  -O1 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99  -mmcu=atmega32   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

lcdFiles/%.o: ../lcdFiles/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=8000000  -I".." -I"../lcdFiles" -I"../lcdFonts" -I"../osHeaders" -I"../osSources" -I"../initFiles" -I"../twiBus"  -O1 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99  -mmcu=atmega32   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

lcdFonts/%.o: ../lcdFonts/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=8000000  -I".." -I"../lcdFiles" -I"../lcdFonts" -I"../osHeaders" -I"../osSources" -I"../initFiles" -I"../twiBus"  -O1 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99  -mmcu=atmega32   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<

twiBus/%.o: ../twiBus/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler
	$(QUOTE)$(AVR_APP_PATH)avr-gcc.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DF_CPU=8000000  -I".." -I"../lcdFiles" -I"../lcdFonts" -I"../osHeaders" -I"../osSources" -I"../initFiles" -I"../twiBus"  -O1 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99  -mmcu=atmega32   -MD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<



# AVR/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

# AVR/GNU C/C++ Linker
$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU C/C++ Linker
	$(QUOTE)$(AVR_APP_PATH)avr-gcc.exe$(QUOTE)  -mmcu=atmega32  -Wl,-Map=$(MAP_FILE_PATH_AS_ARGS) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@



$(HEX_FLASH_FILE_PATH): $(OUTPUT_FILE_PATH)
	$(QUOTE)$(AVR_APP_PATH)avr-objcopy.exe$(QUOTE) -O ihex -R .eeprom -R .fuse -R .lock -R .signature  $(QUOTE)$<$(QUOTE) $(QUOTE)$@$(QUOTE)

$(HEX_EEPROM_FILE_PATH): $(OUTPUT_FILE_PATH)
	-$(QUOTE)$(AVR_APP_PATH)avr-objcopy.exe$(QUOTE) -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex $(QUOTE)$<$(QUOTE) $(QUOTE)$@$(QUOTE) || exit 0

$(LSS_FILE_PATH): $(OUTPUT_FILE_PATH)
	$(QUOTE)$(AVR_APP_PATH)avr-objdump.exe$(QUOTE) -h -S $(QUOTE)$<$(QUOTE) > $(QUOTE)$@$(QUOTE)

size: $(OUTPUT_FILE_PATH)
	@$(QUOTE)$(AVR_APP_PATH)avr-size.exe$(QUOTE) -C --mcu=atmega32 $(OUTPUT_FILE_PATH_AS_ARGS)

# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) $(LIB_AS_ARGS) $(HEX_FLASH_FILE_PATH_AS_ARGS) $(HEX_EEPROM_FILE_PATH_AS_ARGS) $(LSS_FILE_PATH_AS_ARGS) $(MAP_FILE_PATH_AS_ARGS)

