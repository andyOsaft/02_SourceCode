
#include "adc.h"

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
    
    /* interne Referenzspannung nutzen (1.1V)	*/
    ADMUX |=  (1<<REFS1);
    
    ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADSC));
    //ADCSRA |= (1<<ADSC);

    
    //ADMUX &= ~(1<<ADLAR);

    
    
    /*  ADC enable 
        -> single conversion mode (ADATE = 0)
    */	
  
  
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

Die maximal angenommene SystemSpannung von 6.6V wird ¸ber externen Spannungsteiler auf 1.1V geteilt.


Umrechnung der Messspannung (0V - 1.1V) auf Realspannung:

1. Bitergebnis in Spannung umrechnen 

 Umess V	   y bits				            1.1V * y bits  * 100
---------  =  --------  --->    Umess V * 100 = -----------------------
 1.1V	       1023						               1023



2. Gemessene Spannung auf Realspannung umrechnen (Spannungsteilerformel)

__________________
|                |  
|                O R1                               R1 = 16  kOhm
U                |--------------Umess               R2 = 3,3 kOhm
|                O R2               
|________________|______________Masse               


U = Umess * [  (R1+R2) / R2  ]                      (R1+R2) / R2 = 5,85


U [mV] = (Umess * 5850) / 100

-> die durch 100 kommen noch von Schritt 1



**************************************************************************************************************/
uint16_t adcRead(uint8_t channel)
{
	uint32_t result;
  
    	
	/* 	ADC Kanal ausw‰hlen   
         -> dabei das REFS1 bit immer gesetzt lassen*/
    ADMUX = (channel + 0x80);              

    //ADMUX |=  (1<<REFS0);

	/* Single Konvertierung anstoﬂen */
  	ADCSRA |= (1<<ADSC);	
    

  	/* Auf Abschluss der Konvertierung warten */
 	while (ADCSRA & (1<<ADSC) ) {}
	

	/* Ergebnis als Bitwert */
	result = (uint32_t) ADCW;

	/* Wandlungsergebnis in die Realspannung umrechnen 
        -> + 512 -> Korrekte Rundung durch Addition des halben Divisors    
    */
    //result = (((uint32_t)110 * result) + (uint32_t) 512) / 1023; 

    //result = (result * 5850) / 100;
    result = ((643500 * result)+51150) / 102300; 

	return (uint16_t) result;
}


