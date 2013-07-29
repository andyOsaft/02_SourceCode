

#ifndef	__ADCINIT_H
#define __ADCINIT_H

#include "globals.h"

//Funktionsprototypen
//void adcInit();

uint16_t adcRead(uint8_t channel);

/* Used ADC Pins:

    PB5 / ADC0 -> Battery voltage
    PB4 / ADC2 -> Sensor voltage
    PB3 / ADC3 -> Solar voltage

*/


#define BATTERY_VOLTAGE 0
#define SENSOR_VOLTAGE  2
#define SOLAR_VOLTAGE   3


//Register ADCSRA (ADCSR) ---> Betriebsart AD-Wandler
#define ADPS0 0	 //Prescaler select bits 0-2
#define ADPS1 1
#define ADPS2 2	
#define ADIE  3	 //ADC Interrupt Enable	
#define ADIF  4  //ADC Interrupt Flag
#define ADATE 5  //ADC Auto Trigger Enable
#define ADSC  6	 //ADC Start Conversion (stays high until the 
                 //    conversion is completed)
#define ADEN  7	 //ADC enable
                 //-> switch off before entering sleep mode
                 //   -> then the ADC does not consume power


//Register ADMUX ---> Kanalwahl und Referenzspannung
#define MUX0  0	
#define MUX1  1
#define ADLAR 5  // 0 -> ADC data right adjusted
#define REFS0 6  // 0 -> Vcc used as analog reference
			     // 1 -> Internal 1.1V reference

									
				 /* Belegung MUX-Bits: 	PORTB
				 						Kanal 0 (PB5) =  0 0
										Kanal 1 (PB2) =  0 1
										Kanal 2 (PB4) =  1 0 
										Kanal 3 (PB3) =  1 1 
				 */

#endif
