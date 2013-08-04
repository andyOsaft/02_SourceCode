
#include "adc.h"
#include <avr/io.h>

void adcInit()
{
    uint16_t result;
    									

    /*  ADC Prescaler konfigurieren:
        -> es wird eine adc clock frequency zwischen 50kHz und 200kHz f¸r eine Auflˆsung
           von 10 Bit benˆtigt.

        CPU Frequenz: 8MHz
        -> Prescaler 64 -> ADC Frequenz 125kHz
            
    */
    
    ADCSRA |= (1<<ADEN);
    
    //Differential Test:
    // * negative input -> ADC1
    // * positive input -> ADC0
    ADMUX |= ( 1<<MUX4 );  
   
 
    
    ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADSC));
      
    /* Eine single conversion anstoﬂen um den ADC korrekt zu initialisieren */
    //implemented above ADCSRA |= (1<<ADSC);	
  

    /* Auf Abschluss der Konvertierung warten
       Das Register ADCSRA wird mit "1<<ADSC" maskiert. Da ADSC den Wert 6 hat, wartet das while 
       also solange, bis das Bit6 von ADCSRA == 0 wird.
    */
    while (ADCSRA & (1<<ADSC) ) {}
  	
    /* ADC - > Interrupt Enable */  
    //ADCSRA |= (1<<ADIE);				
  
    /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der n‰chsten
        Wandlung nicht ¸bernommen. 
    */
    result = ADCW;		 
   
}
 



/**************************************************************************************************************

Funktion: uint16_t adcRead(uint8_t channel)

Aufgabe:

W‰hlt den gew¸nschten Kanal im Register ADMUX aus, startet eine Wandlung und wartet auf das Ergebnis.

Die maximal angenommene SystemSpannung von 15V wird ¸ber externen Spannungsteiler auf 3V geteilt.


Umrechnung der Messspannung (0V - 3V) auf Realspannung (Aref = 3.28V):

1. Bitergebnis in Spannung umrechnen 

 Umess V	   y bits				           3.28V * y bits 
---------  =  --------  --->    Umess V  = -----------------------
  3.28V	       1023						          1023



2. Gemessene Spannung auf Realspannung umrechnen (Spannungsteilerformel)

__________________
|                |  
|                O R1                               R1 = 4.7k  Ohm
U                |--------------Umess               R2 = 1k    Ohm
|                O R2               
|________________|______________Masse               


U = Umess * [  (R1+R2) / R2  ]                      (R1+R2) / R2 = 5.7


U [mV] = Umess[mV] * 5.7



3.Beide Formeln zusammen:

U [mV] = ( y bits * 3280 mV * 5.7 ) / 1023 

U [mV] = ( y bits * 18696 mV ) / 1023 

-> maximal messbare Spannung:
    y bits = 1023 -> U = 18696 mV
    
-> minimal messbare Spannung:
    y bits = 1 -> U = 18.3 mV   



**************************************************************************************************************/
uint16_t adcRead(uint8_t channel)
{
	uint32_t result;
  
  
    //Single Ended
    //ADMUX:            00101
    //Absolute Input:   ADC5
    ADMUX = 0x05;

   
	/* Single Konvertierung anstoﬂen */
  	ADCSRA |= (1<<ADSC);	
    

  	/* Auf Abschluss der Konvertierung warten */
 	while (ADCSRA & (1<<ADSC) ) {}
	

	/* Ergebnis als Bitwert */
	result = (uint32_t) ADCW;

	/* Wandlungsergebnis in die Realspannung umrechnen 
        -> + 512 -> Korrekte Rundung durch Addition des halben Divisors    
    */
    result = ( (18696 * result ) + 512 ) / 1023; 
      
         
	return (uint16_t) result;
}


uint16_t measureBattVoltage()
{
    return( adcRead( BATTERY_VOLTAGE ) );    
}
