#ifndef __avr_h__
#define __avr_h__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

// Default clock speed to 16Mhz
#ifndef F_CPU
#define F_CPU 8000000
#endif

// clear bit, set bit macros
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void avr_timerInit();
uint32_t avr_millis();
uint64_t avr_ticks();
uint32_t avr_micros();
void avr_delay(unsigned long ms);

/*
void avr_serialInit(uint32_t baud);
void avr_serialPrint(char * str);
void avr_serialVPrint(char * format, va_list arg);
void avr_serialPrintf(char * format, ...);
void avr_serialWrite(unsigned char value);
*/
#endif
