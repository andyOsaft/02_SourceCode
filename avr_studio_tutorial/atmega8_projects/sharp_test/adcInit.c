//Dieses Source-File dient zur vereinfachten Nutzung des integrierten ADCs des ATmega8
//Der A/D Wandler verfügt über 6 Kanäle. Diese liegen auf auf PortC.0 bis PortC.5
//

#include <avr/io.h>
#include "adcInit.h"

void adc_init(uint8_t mux)
{
  
  ADMUX = mux;                      // 	Kanal waehlen(durch Parameterübergabe)
  									
								
  ADMUX &= ~((1<<REFS1) | (1<<REFS0)); // externe Referenzspannung nutzen (typischerweise Vcc-Pegel)


  ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);    	// ADC Prescaler wählen. BIT0 bis BII 2 in ADCSRA
  													// ADC-Takt muss im Bereich 50 - 200 kHz liegen
													// ---> Prescaler = 32 ---> ADC-Takt = 115 kHz
                               					    // UND ADC aktivieren
  ADCSRA |= (1<<ADSC);		// Vor Free Run muss einmal start conversion angestoßen werden 
  ADCSRA |= (1<<ADFR);		// ADC -> Free Run
  	
  ADCSRA |= (1<<ADIE);		// ADC - > Interrupt Enable				 
   
}


 
