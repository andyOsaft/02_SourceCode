

#ifndef	__ADCINIT_H
#define __ADCINIT_H

#include "globals.h"

//Funktionsprototypen
void adcInit();

uint16_t adcRead15(uint8_t channel);
uint16_t adcRead5(uint8_t channel);

//Register ADCSRA (ADCSR) ---> Betriebsart AD-Wandler
#define ADPS0 0	 //Prescaler select bits 0-2
#define ADPS1 1
#define ADPS2 2	
#define ADIE  3	 //ADC Interrupt Enable	
#define ADIF  4  //ADC Interrupt Flag
#define ADFR  5  //ADC Free Run Select
#define ADSC  6	 //ADC Start Conversion
#define ADEN  7	 //ADC enable


//Register ADMUX ---> Kanalwahl und Referenzspannung
#define MUX0  0	
#define MUX1  1
#define MUX2  2
#define MUX3  3
#define MUX4  4
#define ADLAR 5
#define REFS0 6  //0 0 ->externes AREF ; 0 1 ->AVCC als Referenz
#define REFS1 7  //1 0 ->reserviert    ; 1 1 ->interne Referenzspannung
									
				 /* Belegung MUX-Bits: 	PORTC.0 bis 5
				 						Kanal 0 = 0 0 0
										Kanal 1 = 0 0 1
										Kanal 2 = 0 1 0 
										Kanal 3 = 0 1 1 
										Kanal 4 = 1 0 0 
										Kanal 5 = 1 0 1   */

#endif
