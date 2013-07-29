//Dieses Source-File dient zur vereinfachten Nutzung des integrierten ADCs des ATmega8
//Der A/D Wandler verfügt über 6 Kanäle. Diese liegen auf auf PortC.0 bis PortC.5
//

#include <avr/io.h>
#include "adc_single.h"

float ReadChannel(uint8_t mux)
{
  
  float result;
 
  ADMUX = mux;                      // 	Kanal waehlen(durch Parameterübergabe)
  									
								
  ADMUX &= ~((1<<REFS1) | (1<<REFS0)); // externe Referenzspannung nutzen (typischerweise Vcc-Pegel)


  ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);    	// ADC Prescaler wählen. BIT0 bis BII 2 in ADCSRA
  													// ADC-Takt muss im Bereich 50 - 200 kHz liegen
													// ---> Prescaler = 32 ---> ADC-Takt = 115 kHz
                               					    // UND ADC aktivieren

 
   /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
  ADCSRA |= (1<<ADSC);              // eine ADC-Wandlung starten
  while ( ADCSRA & (1<<ADSC) ) {
     ;     // auf Abschluss der Konvertierung warten (wenn das "Start-ADC"-Bit wieder auf 0 ist)
  }
  result = ADCW;  // ADCW muss einmal gelesen werden,
                  // sonst wird Ergebnis der nächsten Wandlung
                  // nicht übernommen.
 
  /* Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen */
  result = 0; 

  ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
  while ( ADCSRA & (1<<ADSC) ) {
     ;     // auf Abschluss der Konvertierung warten (wenn das "Start-ADC"-Bit wieder auf 0 ist)
  }
  result = ADCW;
 
  return result;
}
 
