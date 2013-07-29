
#include "adc.h"
#include <avr/io.h>

void adcInit()
{
    uint16_t result;
    									

    /*  ADC Prescaler konfigurieren:
        -> es wird eine adc clock frequency zwischen 50kHz und 200kHz für eine Auflösung
           von 10 Bit benötigt.

        CPU Frequenz: 8MHz
        -> Prescaler 64 -> ADC Frequenz 125kHz
            
    */
    
    ADCSRA |= (1<<ADEN);
    
    //Differential Test:
    // * negative input -> ADC1
    // * positive input -> ADC0
    ADMUX |= ( 1<<MUX4 );  
   
 
    
    ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADSC));
      
    /* Eine single conversion anstoßen um den ADC korrekt zu initialisieren */
    //implemented above ADCSRA |= (1<<ADSC);	
  

    /* Auf Abschluss der Konvertierung warten
       Das Register ADCSRA wird mit "1<<ADSC" maskiert. Da ADSC den Wert 6 hat, wartet das while 
       also solange, bis das Bit6 von ADCSRA == 0 wird.
    */
    while (ADCSRA & (1<<ADSC) ) {}
  	
    /* ADC - > Interrupt Enable */  
    //ADCSRA |= (1<<ADIE);				
  
    /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der nächsten
        Wandlung nicht übernommen. 
    */
    result = ADCW;		 
   
}
 



/**************************************************************************************************************

Funktion: uint16_t adcRead(uint8_t channel)

Aufgabe:

Wählt den gewünschten Kanal im Register ADMUX aus, startet eine Wandlung und wartet auf das Ergebnis.

Die maximal angenommene SystemSpannung von 15V wird über externen Spannungsteiler auf 3V geteilt.


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
  
  
    if( channel == BATTERY_VOLTAGE )
    {
        //Single Ended
        //ADMUX:            00101
        //Absolute Input:   ADC5
        ADMUX = 0x05;
    }
    else if( channel == TEMPERATURE_SENSOR_VOLTAGE )
    {
        //Single Ended
        //ADMUX:            00110
        //Absolute Input:   ADC6
        ADMUX = 0x06;        
    }
    else if( channel == SOLAR_REGULATOR_CURRENT )
    {
        //Differential Ended
        //ADMUX:            10000
        //Negative Input:   ADC1
        //Positive Input:   ADC0
        ADMUX = 0x10;   
    }   
    else if( channel == WIND_REGULATOR_CURRENT )
    {
        //Differential Ended
        //ADMUX:            10011
        //Negative Input:   ADC1
        //Positive Input:   ADC3
        ADMUX = 0x13;       
    }   
    else if( channel == CONSUMER_CURRENT )          
    {
        //Differential Ended
        //ADMUX:            11100   
        //Negative Input:   ADC2
        //Positive Input:   ADC4
        ADMUX = 0x1C;
    }
   
    //start measurement only if correct channel has been selected
    if( channel >= BATTERY_VOLTAGE && channel <= CONSUMER_CURRENT)
    {
	    /* Single Konvertierung anstoßen */
  	    ADCSRA |= (1<<ADSC);	
    

  	    /* Auf Abschluss der Konvertierung warten */
 	    while (ADCSRA & (1<<ADSC) ) {}
	

	    /* Ergebnis als Bitwert */
	    result = (uint32_t) ADCW;

	    /* Wandlungsergebnis in die Realspannung umrechnen 
            -> + 512 -> Korrekte Rundung durch Addition des halben Divisors    
        */
        result = ( (18696 * result ) + 512 ) / 1023; 
    }
    else
    {
        result = 0xFFFF;
    }        
         
	return (uint16_t) result;
}


uint16_t measureBattVoltage()
{
    return( adcRead( BATTERY_VOLTAGE ) );    
}

uint16_t measureSolarChargeCurrent()
{
    uint32_t shuntVoltage;
    uint32_t shuntCurrent;
    
    shuntVoltage = (uint32_t) adcRead( SOLAR_REGULATOR_CURRENT );
    
    //calculate current flow through shunt resistor [mA]
    //-> *1000 because result has to be in mA (otherwise the 10^-3 of mV and mOhm would cancel each other)
    //shuntCurrent = ( 1000 * shuntVoltage + ( SOLAR_REGULATOR_SHUNT_mOHM >> 2 ) ) / SOLAR_REGULATOR_SHUNT_mOHM; 
    
    shuntCurrent = ( 1000 * shuntVoltage );
    shuntCurrent = ( shuntCurrent + ( SOLAR_REGULATOR_SHUNT_mOHM >> 2 ) ) / SOLAR_REGULATOR_SHUNT_mOHM;
    
    return( (uint16_t) shuntCurrent );  
}

uint16_t measureConsumerCurrent()
{
    uint16_t shuntVoltage;
    uint32_t shuntCurrent;
    
    shuntVoltage = adcRead( CONSUMER_CURRENT );
    
    //calculate current flow through shunt resistor [mA]
    //-> *1000 because result has to be in mA (otherwise the 10^-3 of mV and mOhm would cancel each other)    
    shuntCurrent = ( 1000 * shuntVoltage + ( CONSUMER_CURRENT_SHUNT_mOHM >> 2 ) ) / CONSUMER_CURRENT_SHUNT_mOHM;
    
    return( (uint16_t) shuntCurrent );
}